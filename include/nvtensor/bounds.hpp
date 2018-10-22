#ifndef NVTENSOR_BOUNDS_HPP
#define NVTENSOR_BOUNDS_HPP

#include <nvtensor/meta.hpp>

namespace nvstd
{
	template<typename T,T... Ts>
	struct bounds
	{
		using size_type = T;
		static size_type const depth = 0;
		static size_type const current_range = 1;
		static size_type const total_size = 1;

		bounds() noexcept = default;

		template<typename OtherRange>
		bounds(OtherRange const &, bool) {}

		template<typename S, std::size_t Dim>
		void serialize(S&) const {}

		template<typename S,size_type Dim>
		size_type linearize(S const&) const
		{
			return 0;
		}

		template<typename S, std::size_t Dim>
		size_type contains(S const &) const
		{
			return -1;
		}

		size_type element_count(std::size_t) const noexcept
		{
			return 0;
		}

		size_type size() const noexcept
		{
			return total_size;
		}
	};

	template<typename T,T Head,T... Tail>
	struct bounds<T,Head,Tail...> : bounds<T,Tail...>
	{
		using base_type = bounds<T,Tail...>;
		using size_type = T;
		static size_type const depth = base_type::depth + 1;
		static size_type const current_range = Head;
		static size_type const total_size = current_range * base_type::total_size;

		using base_type::linearize;
		using base_type::serialize;
		using base_type::contains;

		bounds() noexcept = default;

		template<typename OtherRange>
		bounds(OtherRange const & other, bool first)
			: base_type(other,false)
		{
			(void)first;
		}

		template<typename S, std::size_t Dim = 0>
		void serialize(T &array) const
		{
			array[Dim] = element_count();
			base_type::template serialize<T, Dim + 1>(array);
		}


		template<typename S,size_type Dim = 0>
		size_type linearize(T const& array) const
		{
			size_type const idx = base_type::size() * array[Dim];
			return idx + base_type::template linearize<T,Dim + 1>(array);
		}

		template<typename S, std::size_t Dim>
		size_type contains(T const & array) const
		{
			size_type const last = base_type::template contains<T, Dim + 1>(array);
			return base_type::size() * array[Dim] + last;
		}

		size_type element_count(std::size_t) const noexcept
		{
			return size() / base_type::size();
		}

		size_type size() const noexcept
		{
			return current_range * base_type::size();
		}

	};
}
#endif //NVTENSOR_BOUNDS_HPP
