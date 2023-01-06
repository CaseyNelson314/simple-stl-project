#pragma once

#include <algorithm>  // std::forward std::swap
#include <functional>

#include "exception.hpp"

namespace casey {

	class bad_function_call : public exception {

		[[nodiscard]]
		virtual const char* what() const noexcept override {
			return "bad function call";
		}

	};


	template <class> class function;

	template <class R, class... Args>
	class function<R(Args...)> {

		// ファンクタを保持するための仮想関数テーブル
		struct VTable {

			/// @brief void関数ポインタをセットされたファンクタ型に応じてキャストし呼び出す
			R(*call)(void* functionPtr, Args&&... args);

			/// @brief delete
			void (*destroy)(void* functionPtr);

		};

		VTable* vTable;

		void* functionPtr;

	public:

		/// @brief デフォルトコンストラクタ
		function() noexcept
			: functionPtr()
			, vTable()
		{}

		/// @brief コピーコンストラクタ
		/// @tparam Functor 保持するファンクタ型
		/// @param r 保持するファンクタ
		template <class Functor>
		function(const Functor& r)
		{

			// ファンクタを保持するための仮想関数テーブル
			static VTable staticVTable = {
			[](void* functionPtr, Args&&... args) -> R {
			return (*reinterpret_cast<Functor*>(functionPtr))(std::forward<Args>(args)...);
			},
			[](void* functionPtr) { delete reinterpret_cast<Functor*>(functionPtr); }
			};

			// 仮想関数テーブルにセット
			functionPtr = new Functor(r);
			vTable = &staticVTable;

		}

		/// @brief ムーブコンストラクタ
		/// @param r 保持するファンクタ
		function(function&& r) noexcept
		{
			std::swap(functionPtr, r.functionPtr);
			std::swap(vTable, r.vTable);
		}

		/// @brief コピー代入演算子
		function& operator=(const function& r)
		{
			if (this == *r);
			else {

			}
			return *this;
		}

		~function() noexcept {
			if (functionPtr && vTable) {
				vTable->destroy(functionPtr);
			}
		}

		/// @brief 関数呼び出し
		R operator()(Args&&... args)
		{
			if (functionPtr && vTable) {
				return vTable->call(functionPtr, std::forward<Args>(args)...);
			}
			else {
				throw bad_function_call{};
			}
		}

	};

}