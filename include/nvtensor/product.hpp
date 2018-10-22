#ifndef NVTENSOR_PRODUCT_HPP
#define NVTENSOR_PRODUCT_HPP

#include <nvtensor/tensor.hpp>
#include <nvtensor/meta.hpp>


namespace nvstd
{
	template<typename A,typename B,
        class = std::enable_if_t<nvstd::are_tensors_compatible<typename A::tensor_dims_t,typename B::tensor_dims_t>::value>>
	 tensor_product(A& tensor_a,B& tensor_b,)
	{
		using dims_a = typename A::tensor_dims_t;
		using dims_b = typename B::tensor_dims_t;
		using value_type = std::common_type_t<typename A::value_type,typename B::value_type>;
		using return_sequence = nvstd::tensor_result_sequence_t<dims_a, dims_b>;

		nvstd::tensor<nvstd::add_extents_t<value_type,return_sequence>> output{};
		nvstd::sequence_generator_t<return_sequence> generator{};

		auto sum = [&tensor_a,&tensor_b,&output](auto... values)
		{
			value_type result{0.0};
			for(std::size_t inner = 0; inner < nvstd::last_v<dims_a>; inner++)
			{
				std::size_t index_a = tensor_a.linearize(values...,inner);
				std::size_t index_b = tensor_b.linearize(values...,inner);
				result += tensor_a[index_a] * tensor_b[index_b];
			}
			std::size_t const index_c = output.linearize(values...);
			output[index_c] = result;
		};
		generator(sum);
		return c;
	}
}

#endif //NVTENSOR_PRODUCT_HPP
