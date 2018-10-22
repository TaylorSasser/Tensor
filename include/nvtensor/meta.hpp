#ifndef NVTENSOR_META_HPP
#define NVTENSOR_META_HPP

#include <type_traits>
#include <utility>

namespace nvstd
{
	template<typename T,T... Is>
	struct integer_sequence
	{
		using type = integer_sequence;
		using value_type = T;

		enum
		{
			size = sizeof...(Is)
		};
	};

	template<std::size_t... Dims>
	using dims_sequence = integer_sequence<std::size_t,Dims...>;

	namespace impl
	{
		template<typename SequenceA, typename SequenceB, typename... Rest>
		struct concat
		{
			using type = typename concat<typename concat<SequenceA, SequenceB>::type, Rest...>::type;
		};

		template<typename A, A... As, typename B, B... Bs>
		struct concat<integer_sequence<A, As...>, integer_sequence<B, Bs...>>
		{
			using type = integer_sequence<A, As..., Bs...>;
		};

		template<typename T, std::size_t Index, T... Elements>
		struct at_l;

		template<typename T, std::size_t Index, typename Sequence>
		struct at;

		template<typename T, T N, T... Is>
		struct at_l<T, 0, N, Is...>
		{
			static constexpr T value = N;
		};

		template<typename T, std::size_t I, T N, T... Is>
		struct at_l<T, I, N, Is...>
		{
			static_assert(0 <= I && I < sizeof...(Is) + 1, "Requested index out of range.");
			static constexpr T value = at_l<T, I - 1, Is...>::value;
		};

		template<typename T, std::size_t I, T... Is>
		struct at<T, I, integer_sequence<T, Is...>>
		{
			static constexpr T value = at_l<T, I, Is...>::value;
		};

		template<typename T, std::size_t Index, T Value, T... Rest>
		struct remove_at_l
		{
			static_assert(Index < sizeof...(Rest) + 1, "Requested index is out of range.");
			using super = typename remove_at_l<T, Index - 1, Rest...>::type;
			using type = typename concat<integer_sequence<T, Value>, super>::type;
		};

		template<typename T, T V, T... Is>
		struct remove_at_l<T, 0, V, Is...>
		{
			using type = integer_sequence<T, Is...>;
		};

		template<typename T, std::size_t Index, typename Sequence>
		struct remove_at;

		template<typename T, std::size_t I, T... Is>
		struct remove_at<T, I, integer_sequence<T, Is...>>
		{
			using type = typename remove_at_l<T, I, Is...>::type;
		};

		template<typename>
		struct sequence_iterator;

		template<typename T, T Head, T... Tail>
		struct sequence_iterator<integer_sequence<T, Head, Tail...>>
		{
			template<typename F, typename... X>
			constexpr void operator()(F const &function, X... x)
			{
				for (T i = 0; i < Head; ++i)
					sequence_iterator<integer_sequence<T, Tail...>>{}(function, x..., i);
			}
		};

		template<typename T, T Head>
		struct sequence_iterator<integer_sequence<T, Head>>
		{
			template<typename F, typename... X>
			constexpr void operator()(F const &function, X... x)
			{
				for (T i = 0; i < Head; ++i)
					function(x..., i);
			}
		};

		template<typename>
		struct reduce;

		template<typename T, T Head, T... Ts>
		struct reduce<integer_sequence<T, Head, Ts...>>
		{
			constexpr static std::size_t value = Head * reduce<integer_sequence<T, Ts...>>::value;
		};

		template<typename T, T Head>
		struct reduce<integer_sequence<T, Head>>
		{
			constexpr static std::size_t value = Head;
		};

		template<typename T, std::size_t...>
		struct extents_adder;

		template<typename T>
		struct extents_adder<T>
		{
			using type = T;
		};

		template<typename T, std::size_t E, std::size_t... Es>
		struct extents_adder<T, E, Es...>
		{
			using type = typename extents_adder<T[E], Es...>::type;
		};

		template<typename,typename>
		struct add_extents
		{};

		template<typename T,std::size_t... Ns>
		struct add_extents<T,integer_sequence<std::size_t,Ns...>>
		{
			using type = typename impl::extents_adder<T,Ns...>::type;
		};

		template<typename T,typename Seq>
		using add_extents_t = typename add_extents<T,Seq>::type;

		template<typename, typename Sequence = integer_sequence<std::size_t>>
		struct unpacked_array_type
		{
			using type = Sequence;
		};

		template<typename T, std::size_t N, std::size_t ... I>
		struct unpacked_array_type<T[N],integer_sequence<std::size_t,I...>>
			: public unpacked_array_type<T, integer_sequence<std::size_t,I..., N>>
		{};
	}

	template <typename SA, typename SB, typename... R>
	using concat_t = typename impl::concat<SA, SB, R...>::type;

	template <std::size_t I, typename S>
	static constexpr auto at_v = impl::at<typename S::value_type, I, S>::value;

	template <typename S>
	static constexpr auto first_v = at_v<0, S>;

	template <typename S>
	static constexpr auto last_v = at_v<S::size - 1, S>;

	template <std::size_t I, typename S>
	using remove_at = typename impl::remove_at<typename S::value_type, I, S>::type;

	template<typename S>
	using pop_front_t = typename impl::remove_at<typename S::value_type,0,S>::type;

	template<typename S>
	using pop_back_t = typename impl::remove_at<typename S::value_type,S::size - 1,S>::type;

	template<typename SA, typename SB>
	using are_tensors_compatible = std::conditional_t<last_v<SA> == first_v<SB>, std::true_type, std::false_type>;

	template<typename SA, typename SB>
	using tensor_result_sequence_t = concat_t<pop_back_t<SA>, pop_front_t<SB>>;

	template<typename S>
	using sequence_generator_t = impl::sequence_iterator<S>;

	template<typename S>
	using extract_dims_t = typename impl::unpacked_array_type<S>::type;

	template<typename T,typename Seq>
	using add_extents_t = typename impl::add_extents<T,Seq>::type;

	template<typename S>
	static constexpr auto reduce_v = impl::reduce<S>::value;
}

#endif //NVTENSOR_META_HPP