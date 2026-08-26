#pragma once
#include "stock.hpp"
#include <cstdint>

struct Stats {
	uint32_t stock_id_; // ie: AAPL = 221814
	double mean_ = 0; // average price
	double std_dev_ = 0;
	uint32_t volume_ = 0; // total shares traded
	uint32_t trade_count_ = 0; // total completed matches
	double last_price_ = 0; // most recent fulfilled price
	double high_ = 0;
	double low_ = 0;
	double open_ = 0;
	double close_ = 0;
	
	Stats() = default;
};

/*
8 bits: 0 to 255
16 bits: 0 to 65,535
32 bits: 0 to 4,294,967,295
64 bits: 0 to 18,444,674,407,370,955,161
*/