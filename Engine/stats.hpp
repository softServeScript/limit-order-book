#pragma once
#include "stock_info.hpp"
#include <cstdint>
#include <array>

struct Stats {
	uint32_t stock_id_ = 0;			// ie: AAPL = 02021814
	double total_price_ = 0;			// total price sold
	double last_price_ = 0;				// most recent fulfilled price
	
	// basic stats
	double ovr_mean_ = 0; 				// overall mean
	double mean_ = 0; 					// standard mean
	double m2_ = 0;
	double std_dev_ = 0; 
	
	// quantities
	uint32_t volume_ = 0; 				// total shares traded
	uint32_t trade_count_ = 0; 			// total completed matches
	
	// OHLC graph
	double high_ = 0;
	double close_ = 0;
	double open_ = 0;
	double low_ = 0;

	Stats(const StockInfo& stock) {
		stock_id_ = stock.stock_id_;
	}
	
	std::array<double, 10> rec_price_;
	uint8_t index_ = 0;


	Stats() = default;
};