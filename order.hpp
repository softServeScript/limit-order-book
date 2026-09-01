#pragma once
#include "enums.hpp"
#include "stock.hpp"
#include <chrono>
#include <cstdint>

// prioritize user privacy. order_id_ tied to account
struct OrderInfo {
	uint32_t stock_id_; // ie: AAPL = 02021814
	uint32_t order_id_; // min 5 figures (10,000)
	uint16_t quantity_;
	uint16_t encoded_price_; // divide by 100 for display price
	Side side_;
	Status status_;
	Type type_;
	std::chrono::nanoseconds order_time_;
	
	OrderInfo(const StockInfo& stock) { // stock_id_ constructor
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

/*
8 bits: 0 to 255
16 bits: 0 to 65,535
32 bits: 0 to 4,294,967,295
64 bits: 0 to 18,444,674,407,370,955,161
*/