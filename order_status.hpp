#pragma once
#include "enums.hpp"
#include "order.hpp"
#include <cstdint>

// light weight display header
struct OrderStatus {
	uint32_t order_id_;
	uint16_t quantity_;
	double display_price_;
	Status status_;
	
	void DisplayOrder(const OrderInfo& order) {
		order_id_ = order.order_id_;
		quantity_ = order.quantity_;
		display_price_ = static_cast<double>(order.encoded_price_)/100.0;
		status_ = order.status_;
	}
};