#pragma once
#include "enums.hpp"
#include <cstdint>
#include <string>

struct StockInfo {
	uint32_t stock_id_; // ie: AAPL = 02021814
	uint32_t quantity_; // total of both sides
	Status status_;
};