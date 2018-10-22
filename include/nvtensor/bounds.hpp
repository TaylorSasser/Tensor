#ifndef NVTENSOR_BOUNDS_HPP
#define NVTENSOR_BOUNDS_HPP

#include <nvtensor/meta.hpp>

namespace nvstd
{
	template<typename StorageType,StorageType Rank,class = std::enable_if_t<std::is_integral_v<StorageType>>>
	class bounds
	{
	public:
		template<StorageType... Ts>
		explicit constexpr bounds(integer_sequence<StorageType,Ts...> const&)
			: dims{Ts...}
		{}

		enum : StorageType
		{
			rank = Rank,
		};
	private:
		StorageType const dims[rank];
	};
}
#endif //NVTENSOR_BOUNDS_HPP
