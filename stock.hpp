#pragma once
#include "enums.hpp"
#include <cstdint>
#include <string>

struct StockInfo {
	uint32_t stock_id_; // ie: AAPL = 221814
	uint32_t quantity_; // total | both sides
	uint16_t stock_price_; // avg price
	Status status_;
};

/*
8 bits: 0 to 255
16 bits: 0 to 65,535
32 bits: 0 to 4,294,967,295
64 bits: 0 to 18,444,674,407,370,955,161
*/