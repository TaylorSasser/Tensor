#ifndef NVTENSOR_TENSOR_HPP
#define NVTENSOR_TENSOR_HPP

#include <numeric>
#include "meta.hpp"

namespace nvstd
{
	template<int Rank>
	class bounds
	{
	public:
		template<typename T,T... Ts>
		explicit bounds(integer_sequence<T,Ts...> const& sequence)
			: dims{Ts...}
		{}

		enum
		{
			rank = Rank,
		};
		std::size_t const dims[Rank];
	};

	template<typename T,typename Allocator = std::allocator<std::remove_all_extents_t<T>>>
	class tensor
	{
	public:
		using allocator_t = Allocator;
		using tensor_dims_t = nvstd::extract_dims_t<T>;
		using value_type = std::remove_all_extents_t<T>;
		using bounds_t = bounds<tensor_dims_t::size>;
		using pointer = value_type *;
		using const_pointer = value_type const *;
		using reference = value_type &;
		using const_reference = value_type const &;

		tensor()
			: allocator_{},
			  data_{allocator_.allocate(reduce_v<tensor_dims_t>)},
			  bounds_{tensor_dims_t{}}
		{}

		pointer begin()
		{
			return &data_[0];
		}

		pointer end()
		{
			return data_ + reduce_v<tensor_dims_t>;
		}

		template<typename... Ts>
		std::size_t const at(Ts... indexes) const
		{
			unsigned int offsets[] = {indexes...};
			unsigned long res{0},k{0},dimension{bounds_.rank};

			for (k = 0; k < dimension - 1; ++k)
			{
				 res += offsets[k];
				 res *= bounds_.dims[k+1];
			}
			res += offsets[k];
			return res;
		}

		reference operator[](std::size_t const index)
		{
			return data_[index];
		}
	private:
		allocator_t allocator_;
		pointer const data_;
		bounds_t const bounds_;
	};
}

#endif //NVTENSOR_TENSOR_HPP
