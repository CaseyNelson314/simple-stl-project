#pragma once

#include "exception.hpp"

namespace casey {

	class bad_optional_access : exception {

		[[nodiscard]]
		const char* what() const noexcept override {
			return "bad optional access";
		}

	};

	template<class Ty>
	class optional {
		bool _hasValue;
		Ty _value;
	public:
		constexpr optional(const Ty& value) noexcept
			: _hasValue(true)
			, _value(value)
		{}

		constexpr optional() noexcept
			: _hasValue()
			, _value()
		{}

		explicit inline constexpr
			operator bool() const noexcept {
			return has_value();
		}

		inline constexpr bool has_value() const noexcept {
			return _hasValue;
		}

		inline constexpr const Ty& value() const {
			if (_hasValue)
				return _value;
			else
				throw bad_optional_access{};
		}
		inline constexpr Ty& value() {
			if (_hasValue)
				return _value;
			else
				throw bad_optional_access{};
		}

		inline constexpr const Ty& operator*() const {
			return value();
		}

		inline constexpr Ty& operator*() noexcept {
			return value();
		}

		inline constexpr void reset() noexcept {
			_hasValue = {};
			_value = {};
		}

	};

	template<class Ty>
	inline optional<Ty> make_optional(const Ty& value) {
		return { value };
	}

}