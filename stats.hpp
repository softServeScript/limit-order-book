#pragma once
#include "stock.hpp"
#include <cstdint>

struct Stats {
	uint32_t stock_id_; // ie: AAPL = 221814
	uint16_t mean_; // average price
	uint16_t std_dev_;
	uint32_t volume_; // total shares traded
	uint32_t trade_count_; // total completed matches
	uint16_t last_price_; // most recent fulfilled price
	uint16_t high_;
	uint16_t low_;
	uint16_t open_;
	uint16_t close_;
	
	Stats(const StockInfo& stock) {
		stock_id_ = stock.stock_id_;
	}
};

/*
8 bits: 0 to 255
16 bits: 0 to 65,535
32 bits: 0 to 4,294,967,295
64 bits: 0 to 18,444,674,407,370,955,161
*/