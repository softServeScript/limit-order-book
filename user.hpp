#pragma once
#include "order.hpp"
#include <unordered_map>
#include <cstdint>

struct UserAcc {
	std::string display_name_;
	std::string password_;
	double money_ = 1000000; // sum lite 
	
 
	std::unordered_map<uint32_t, OrderInfo*> orders_;
};


class UserAccount {
	public:
		void MakeAccount();
		void CancelOrder(uint32_t order_id_);
		void DisplayOrders();
		
	private:
		OrderStatus os_;
		UserAcc ua_;
};