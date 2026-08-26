#pragma once
#include "order.hpp"
#include <string>
#include <unordered_map>
#include <queue>
#include <utility>
#include <vector>

struct OrderQueue {
	std::priority_queue<OrderInfo> buy_;
  std::priority_queue<OrderInfo> sell_;
};

struct StopOrderVector {
	std::vector<OrderInfo> buy_;
	std::vector<OrderInfo> sell_;
};

struct StockEntry {
	Stats s_;
	StockInfo si_;
	OrderQueue oq_;
	StopOrderVector sov_;
};

class OrderBookCreation {
	public: 
		void LoadFiles(const std::string& file_directory);		
		void OrderBookAdd(uint8_t s, uint32_t s_id, uint32_t o_id, uint16_t q, uint16_t p, 
											uint8_t status_, uint8_t type_, uint64_t t);
		void MatchOrders(uint32_t stock_id_);
		
	private:
		std::unordered_map<uint32_t, StockEntry> book_;
};
