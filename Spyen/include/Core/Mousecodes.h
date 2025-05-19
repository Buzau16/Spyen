#pragma once

#include <cstdint>

namespace Spyen {

	using MouseCode = uint32_t;

	enum : MouseCode {
		// From glfw3.h
		SPMB_0 = 0,
		SPMB_1 = 1,
		SPMB_2 = 2,
		SPMB_3 = 3,
		SPMB_4 = 4,
		SPMB_5 = 5,
		SPMB_6 = 6,
		SPMB_7 = 7,
		SPMB_LAST = SPMB_7,
		SPMB_LEFT = SPMB_0,
		SPMB_RIGHT = SPMB_1,
		SPMB_MIDDLE = SPMB_2
	};
}