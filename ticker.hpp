#pragma once
#include <array>

// 0 = '#' = buyer
// 1 = '*' = seller

namespace Ticker {
	std::array<char, 28> map = {'#', '*', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
															'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
															'U', 'V', 'W', 'X', 'Y', 'Z'};
}