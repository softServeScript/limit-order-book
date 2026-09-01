#pragma once
#include "order.hpp"
#include "stats.hpp"
#include "user.hpp"
#include <mutex>
#include <string>
#include <unordered_map>
#include <queue>
#include <utility>
#include <vector>

struct OrderQueue {
	std::priority_queue<OrderInfo> buy_;
  std::priority_queue<OrderInfo> sell_;
};

struct StopOrderQueue {
	std::priority_queue<OrderInfo> buy_;
	std::priority_queue<OrderInfo> sell_;
};

struct StockEntry {
	Stats s_;
	StockInfo si_;
	OrderQueue oq_;
	StopOrderQueue soq_;
};

class OrderBook {
	public: 
		void LoadFiles(const std::string& file_directory);		
		void OrderBookAdd(uint8_t s, uint32_t s_id, uint32_t o_id, uint16_t q, uint16_t p, 
											uint8_t status_, uint8_t type_, uint64_t t, UserAcc& ua_);
		void OrderBookCreate(UserAcc& ua_);
		void MatchOrders(uint32_t stock_id_);
		void UpdateStats(uint16_t encoded_price_);
		void OrderStats(double& last_price_, uint32_t& trade_count_, double& ovr_mean_, double& m2_, double& std_dev_);
		void ThreadProcess();
		
	private:
		std::mutex mtx_;
		std::unordered_map<uint32_t, StockEntry> book_;
};
