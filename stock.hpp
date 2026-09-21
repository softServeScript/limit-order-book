#pragma once
#include "enums.hpp"
#include <cstdint>
#include <string>

struct StockInfo {
	uint32_t stock_id_ = 0; // ie: AAPL = 02021814
	uint32_t quantity_ = 0; // total of both sides
	Status status_ = Status::OPEN;
};