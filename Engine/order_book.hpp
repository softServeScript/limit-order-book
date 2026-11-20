#pragma once
#include "order_info.hpp"
#include "stats.hpp"
#include <mutex>
#include <string>
#include <unordered_map>
#include <queue>
#include <utility>
#include <vector>
#include <memory>

struct OrderQueue {
	std::priority_queue<std::shared_ptr<OrderInfo>> buy_;
  	std::priority_queue<std::shared_ptr<OrderInfo>> sell_;
};

struct StopOrderQueue {
	std::priority_queue<std::shared_ptr<OrderInfo>> buy_;
	std::priority_queue<std::shared_ptr<OrderInfo>> sell_;
};

struct StockEntry {
	Stats s_;
	StockInfo si_;
	OrderQueue oq_;
	StopOrderQueue soq_;
};

class OrderBook {
	public: 
		void OrderBookAdd(uint8_t s, uint32_t s_id, uint32_t o_id, uint16_t q, uint16_t p, 
											uint8_t status_, uint8_t type_, uint64_t t);
		void MatchOrders(uint32_t stock_id_);
		void UpdateStats(Stats stats, std::shared_ptr<OrderInfo> sell);
		void OrderStats(double& last_price_, uint32_t& trade_count_, double& ovr_mean_, double& m2_, double& std_dev_);
		void ThreadProcess();
		inline bool CheckForStock(uint32_t stock_id) {
			return (book_.count(stock_id) == 1);
		};
		void StatusCheck(uint32_t stock_id);
		
	private:
		std::mutex mtx_;
		std::unordered_map<uint32_t, StockEntry> book_;
		
};
