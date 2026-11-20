#pragma once
#include "enums.hpp"
#include "stock_info.hpp"
#include <chrono>
#include <cstdint>

// prioritize user privacy. order_id_ tied to account
struct OrderInfo {
	uint32_t stock_id_ = 0;				// ie: AAPL = 02021814
	uint32_t order_id_ = 0;				// min 5 figures (10,000)
	uint16_t quantity_ = 0;
	uint16_t encoded_price_ = 0; 		// divide by 100 for display price
	Side side_{0}; 						// default to "BUY"
	Status status_{0}; 					// default to  "OPEN"
	Type type_{0}; 						// default to "LIMIT"

	std::chrono::nanoseconds order_time_;
	
	OrderInfo(const StockInfo& stock) {
		stock_id_ = stock.stock_id_; 
	}
	
	// this < other SMALLER, other < this LARGER
	bool operator<(const OrderInfo& other) const { // price-time stock market
		if (type_ != other.type_) {
			return type_ > other.type_;
		}
		if (encoded_price_ != other.encoded_price_) {
			if (side_ == Side::SELL) {
				return encoded_price_ > other.encoded_price_; // return lowest
			} 
			return encoded_price_ < other.encoded_price_; // return highest
		}
		return order_time_ > other.order_time_;
	}
};