#pragma once
#include "order.hpp"
#include <string>
#include <unordered_map>
#include <queue>
#include <utility>

struct OrderBook {
	void CancelOrder();
};

struct OrderQueue {
	std::priority_queue<OrderInfo> buy_;
  std::priority_queue<OrderInfo> sell_;
};

class OrderBookCreation {
	void LoadFiles(const std::string& file_directory);
	std::unordered_map<uint32_t stock, OrderQueue> book_;
	OrderQueue oq;
	StockInfo si;
	OrderInfo oi(si);
	
	void OrderBookAdd(uint8_t s, uint32_t s_id, uint32_t o_id, uint16_t q, uint16_t p, uint8_t status_, uint8_t type_, uint64_t t) {
		if (!(book_.contains(s_id))) { // if stock/KEY doesnt exist
			si.stock_id_ = s_id;
			si.quantity_ = q;
	  //si.stock_price_ TBD LATER TIME
			si.status_ = Status::OPEN;	// OPEN
		} else {
			si.quantity_ += q;
		}
		
		// create order.hpp
		oi.side_ = static_cast<Side>(s);
		oi.stock_id_ = s_id;
		oi.order_id_ = o_id;
		oi.quantity_ = q;
		oi.encoded_price_ = p;
		oi.status_ = static_cast<Status>(status_);
		oi.type_ = static_cast<Type>(type_).
		oi.order_time_ = std::chrono::nanoseconds(t);			
		
		if (s == 0) {
			oq.buy_.push(oi);
		} else {
			oq.sell_.push(oi);
		}
			
		book_.insert({s_id, oq});
	}
};