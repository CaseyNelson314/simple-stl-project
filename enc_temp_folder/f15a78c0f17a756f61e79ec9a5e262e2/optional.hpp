#pragma once

#include <exception>


class bad_optional_access : std::exception {
public:
	const char* what() const noexcept override {
		return "bad optional access";
	}
};

template<class Ty>
class optional {
	bool hasValue_;
	Ty value_;
public:
	constexpr optional(const Ty& value) noexcept
		: hasValue_(true)
		, value_(value)
	{}

	constexpr optional() noexcept
		: hasValue_()
		, value_()
	{}

	explicit inline constexpr
		operator bool() const noexcept {
		return has_value();
	}

	inline constexpr bool has_value() const noexcept {
		return hasValue_;
	}

	inline constexpr const Ty& value() const {
		if (hasValue_)
			return value_;
		else
			throw bad_optional_access{};
	}
	inline constexpr Ty& value() {
		if (hasValue_)
			return value_;
		else
			throw bad_optional_access{};
	}

	inline constexpr const Ty& operator*() const {
		return value();
	}

	inline constexpr Ty& operator*() {
		try {
			return value();
		}
		catch (const bad_optional_access& e) {
			throw e;
		}
	}

	inline constexpr void reset() noexcept {
		hasValue_ = {};
		value_ = {};
	}

};

template<class Ty>
inline optional<Ty> make_optional(const Ty& value) {
	return { value };
}
