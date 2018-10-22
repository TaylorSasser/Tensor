#include <iostream>
#include <nvtensor/tensor.hpp>
#include <nvtensor/product.hpp>

#include <catch.hpp>


template<typename T,T... As>
std::ostream& operator<<(std::ostream& os,nvstd::integer_sequence<T, As...> const& seq)
{
	os << "[";
	auto delimiter = "";
	for (T val : {As...})
	{
		os << delimiter << val;
		delimiter = ", ";
	}
	return os << "]";
}


TEST_CASE("Tensor Return Type Tests","[RETURN-TYPE]")
{
	nvstd::tensor<float[8][8][8]> a{};
	nvstd::tensor<float[8][8][8]> b{};

	std::fill(a.begin(), a.end(), 2);
	std::fill(b.begin(), b.end(), 2);

	auto c = nvstd::tensor_product(a, b);

	auto delimiter{""};
	for (auto val : c)
	{
		std::cout << delimiter << val;
		delimiter = ", ";
	}

	std::cin.get();
}