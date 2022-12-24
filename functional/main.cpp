#include <iostream>

#include "functional.hpp"

int main() {

	{
		/// 最小構成ラムダ式
		function<void()> f = []() {
			std::cout << "a";
		};
		f();

		std::cout << std::endl;
	}

	{
		/// キャプチャ有りラムダ式

		int i = 1234;
		function<void()> f = [&]() {
			std::cout << i;
		};
		f();

		std::cout << std::endl;
	}

	{
		/// 引数ありラムダ式
		function<void(int)> f = [](int value) {
			std::cout << value;
		};
		f(100);

		std::cout << std::endl;
	}


	{
		/// 戻り値ありラムダ式
		function<int()> f = [&]() {
			return 200;
		};
		std::cout << f();

		std::cout << std::endl;
	}
}