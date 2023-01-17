#pragma once

#include <algorithm>  // std::forward std::swap


#include "memory.hpp"  // casey::shared_ptr
#include <memory>
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
			R(*call)(void*, Args&&...);
			void (*destroy)(void*);
		};

		VTable* vTable;

		std::shared_ptr<void> functionPtr;

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

			// 仮想関数テーブル構築
			static VTable table = {
				[](void* functionPtr, Args&&... args) -> R
				{
					return (*reinterpret_cast<Functor*>(functionPtr))(std::forward<Args>(args)...);
				},
				[](void* functionPtr)
				{
					delete reinterpret_cast<Functor*>(functionPtr);
				}
			};

			// 仮想関数テーブルにセット
			functionPtr = reinterpret_cast<void*>(new Functor(r));
			vTable = &table;

		}

		/// @brief ムーブコンストラクタ
		/// @param r 保持するファンクタ
		function(function&& r) noexcept
		{
			vTable = r.vTable;
			functionPtr = std::move(r.functionPtr);
		}

		/// @brief コピー代入演算子
		function& operator=(const function& r)
		{
			if (*this == &r);
			else {
				vTable = r.vTable;
				functionPtr = r.functionPtr;
			}
			return *this;
		}

		~function() noexcept {
			if (functionPtr && vTable) {
				vTable->destroy(functionPtr.get());
			}
		}

		/// @brief 関数呼び出し
		R operator()(Args&&... args)
		{
			if (functionPtr && vTable) {
				return vTable->call(functionPtr.get(), std::forward<Args>(args)...);
			}
			else {
				throw bad_function_call{};
			}
		}

	};

}