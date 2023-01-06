#pragma once

namespace casey {

	class exception {

	public:

		[[nodiscard]]
		virtual const char* what() const noexcept = 0;

	};

}