//
// Created by taylo on 10/20/2018.
//

#ifndef NVTENSOR_TENSOR_VIEW_HPP
#define NVTENSOR_TENSOR_VIEW_HPP

#include <nvtensor/bounds.hpp>
#include <nvtensor/meta.hpp>

#include <type_traits>


namespace nvstd
{
	template<typename T>
	class tensor_span
	{
		using tensor_dims_t = nvstd::extract_dims_t<T>;
		using value_type = std::remove_all_extents_t<T>;
		using bounds_t = bounds<std::size_t,tensor_dims_t::size>;
		using pointer = value_type *;
		using const_pointer = value_type const *;
		using reference = value_type &;
		using const_reference = value_type const &;

		enum : std::size_t
		{
			size = reduce_v<tensor_dims_t>,
			rank = std::rank_v<T>
		};
		using sliced_type = std::conditional_t<rank == 1, value_type, tensor_span<std::remove_extent_t<T>>>;


		tensor_span(pointer data)
			: data_{data},
			  bounds_{tensor_dims_t{}}
		{}

		pointer begin()
		{
			return &data_[0];
		}

		template <bool Enabled = (rank > 1), typename Ret = std::enable_if_t<Enabled, sliced_type>>
		constexpr Ret operator[](std::size_t const idx)
		{
			const std::size_t index = idx * bounds_[0];
			return Ret{data_ + index};
		}

		reference operator[](std::size_t const idx)
		{
			return data_[idx];
		}

		constexpr bounds_t bounds() const noexcept
		{
			return bounds_;
		}

		pointer end()
		{
			return data_ + reduce_v<tensor_dims_t>;
		}
	private:
		pointer data_;
		bounds_t bounds_;
	};
}

#endif //NVTENSOR_TENSOR_VIEW_HPP
