#include "ticker.hpp"
#include "enums.hpp"
#include "stock.hpp"
#include "order.hpp"
#include "stats.hpp"
#include "order_status.hpp"
#include "order_book.hpp"
#include "user.hpp"
#include "util.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>

void OrderBook::LoadFiles(const std::string& file_directory) {
	namespace fs = std::filesystem; 
	
	for (fs::directory_entry& dir_entry : fs::directory_iterator(file_directory)) {
		
		// check file names for identifier "f_"
		if (dir_entry.path().filename().string().substr(0, 2) != "f_") continue; 
		
		std::ifstream f(dir_entry.path());
		
		// corruption/error check
		if (!f.is_open()) continue;
		
		// iterate strings of vals and tokenize
		while (std::string line; std::getline(f, line)) { 
			std::istringstream ss(line);
			// stock_id, order_id, quantity, price, status, type, timestamp
			std::string a, b, c, d, e, f, g; 

			ss >> a >> b >> c >> d >> e >> f >> g;
			
			uint8_t side = 0;
			if (Ticker::map[0] != a[0]) side = 1;
			a.erase(0, 1);
			
			// Conver string to int/longlong
			uint32_t stock_id(std::stoi(a));
			uint32_t order_id(std::stoi(b));
			uint16_t quantity(std::stoi(c));
			uint16_t price(std::stoi(d));
			uint8_t status(std::stoi(e));
			uint8_t type(std::stoi(f));
			uint64_t timestamp(std::stoll(g));
						
			OrderBookAdd(side, stock_id, order_id, quantity, price, status, type, timestamp);
		}
	}
}

void OrderBook::OrderBookAdd(uint8_t s, uint32_t s_id, uint32_t o_id, uint16_t q, uint16_t p, uint8_t status_, uint8_t type_, uint64_t t, UserAcc& ua_) {
	CheckForStock(s_id);
	
	// create order.hpp
	OrderInfo oi(book_[s_id].si_);
	oi.side_ = static_cast<Side>(s);
	oi.order_id_ = o_id;
	oi.quantity_ = q;
	oi.encoded_price_ = p;
	oi.status_ = static_cast<Status>(status_);
	oi.type_ = static_cast<Type>(type_);
	
	if (t == 0) {
		oi.order_time_ = std::chrono::nanoseconds(Timestamp());
	} else {
		oi.order_time_ = std::chrono::nanoseconds(t);			
	}
	
	ua_.orders_.insert({o_id, &oi});

	if (type_ == 2 || type_ == 3) { // STOP ORDERDS
		if (s == 0) {
			book_[s_id].soq_.buy_.push(oi);
		} else {
			book_[s_id].soq_.sell_.push(oi);
		}
	} else {
		if (s == 0) { 
			book_[s_id].oq_.buy_.push(oi);
		} else {
			book_[s_id].oq_.sell_.push(oi);
		}
	}
}

void OrderBook::CheckForStock(uint32_t s_id, uint16_t q) {
	auto& stats = book_[s_id].s_;
	auto& si = book_[s_id].si_;
	
		if (!(book_.contains(s_id))) {
		si.stock_id_ = s_id;
		si.quantity_ = q;
		si.status_ = Status::OPEN;
		stats.stock_id_ = s_id;
		stats.volume_ = q;
	} else {
		si.quantity_ += q;
		stats.volume_ += q;
	}
}

// NOT DONE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void OrderBook::OrderBookCreate(UserAcc& ua_) {
	OrderInfo oi;
	oi.side_ = 2;
	
	uint32_t stock_id;
	uint32_t order_id;
	uint16_t quantity;
	int16_t price;
	uint8_t status;
	uint8_t type;
	int64_t timestamp;
	
	while (side != 0 || side != 1) {
		std::cout << "BUY OR SELL? "; std::cin >> oi.side_;
		
		if (oi.side_ == 0) {hh
			std::cout << "Which Stock would you like to buy?\n";
			std::string ticker;
			for (auto& [id, e] : book_) std::cout << DecodeID(id) << "\n";
			
			while (!book_.empty() && !book_.contains(stock_id_)) std::cin ticker>> ; 
			oi.stock_id_ = 
			
			
			
		}
	}
	
	
	
	OrderBookAdd(side, stock_id, order_id, quantity, price, status, type, timestamp);
}

void OrderBook::MatchOrders(uint32_t stock_id_) { 
	mtx_.lock();
	
	StatusCheck(stock_id_);
	
	auto& stats = book_[stock_id_].s_;

	while (book_[stock_id_].oq_.sell_.top().encoded_price_ <= book_[stock_id_].oq_.buy_.top().encoded_price_) { // check for matches
		OrderInfo sell = book_[stock_id_].oq_.sell_.top();
		OrderInfo buy = book_[stock_id_].oq_.buy_.top();
		book_[stock_id_].oq_.buy_.pop();
		book_[stock_id_].oq_.sell_.pop();
		
		if (sell.quantity_ != buy.quantity_) { // if sell != buy
			if (sell.quantity_ < buy.quantity_) { // if sell < buy
				buy.quantity_ -= sell.quantity_;
				book_[stock_id_].s_.volume_ += sell.quantity_;
				sell.quantity_ = 0;
				buy.status_ = Status::PARTIAL;
				sell.status_ = Status::FILLED;	
				book_[stock_id_].oq_.buy_.push(buy);
			} else {
				sell.quantity_ -= buy.quantity_;
				book_[stock_id_].s_.volume_ += buy.quantity_;
				buy.quantity_ = 0;
				buy.status_ = Status::FILLED;
				sell.status_ = Status::PARTIAL;
				book_[stock_id_].oq_.sell_.push(sell);
			}
		} else {
			book_[stock_id_].s_.volume_ += sell.quantity_;
			buy.quantity_ = 0;
			sell.quantity_ = 0;
			buy.status_ = Status::FILLED;
			sell.status_ = Status::FILLED;
		}
		
		UpdateStats(sell.encoded_price_);
		
		while (!book_[stock_id_].soq_.buy_.empty() && stats.last_price_*100.0 >= book_[stock_id_].soq_.buy_.top().encoded_price_) {
			OrderInfo dupe = book_[stock_id_].soq_.buy_.top();
			book_[stock_id_].soq_.buy_.pop();
			book_[stock_id_].oq_.buy_.push(dupe);
		}
		while (!book_[stock_id_].soq_.sell_.empty() && stats.last_price_*100 <= book_[stock_id_].soq_.sell_.top().encoded_price_) {
			OrderInfo dupe = book_[stock_id_].soq_.sell_.top();
			book_[stock_id_].soq_.sell_.pop();
			book_[stock_id_].oq_.sell_.push(dupe);
		}
		
		OrderStats(stats.last_price_, stats.trade_count_, stats.ovr_mean_, stats.m2_, stats.std_dev_);
	}
	mtx_.unlock();
}

void OrderBook::StatusCheck(uint32_t stock_id_) {
	if (book_[stock_id_].oq_.buy_.empty() || book_[stock_id_].oq_.sell_.empty()) return;
	
	while (book_[stock_id_].oq_.buy_.top().status_ == Status::CANCELLED) {
		book_[stock_id_].oq_.buy_.pop();
	}
	while (book_[stock_id_].oq_.sell_.top().status_ == Status::CANCELLED) {
		book_[stock_id_].oq_.sell_.pop();
	}
}

void OrderBook::UpdateStats(uint16_t encoded_price_) {
	stats.trade_count_ ++; // total trade count
	stats.last_price_ = static_cast<double>(sell.encoded_price_ / 100.0); // last price sold
	stats.total_price_ += stats.last_price_; // total price for overall mean
	stats.ovr_mean_ += (stats.total_price_ / stats.trade_count_);
		
	if (stats.index_ == 9) stats.index_ = 0;
	stats.rec_price_[stats.index_] = stats.last_price_;
	stats.index_++;
	
	double sum = 0;
	for (double n: stats.rec_price_) sum += n;
	stats.mean_ = (sum / 10);
		
	if (stats.trade_count_ == 1) stats.open_ = stats.last_price_;
	stats.close_ = stats.last_price_;
	if (stats.last_price_ > stats.high_) stats.high_ = stats.last_price_;
	if (stats.low_ == 0 || stats.last_price_ < stats.low_) stats.low_ = stats.last_price_;
}

void OrderBook::OrderStats(double& last_price_, uint32_t& trade_count_, double& ovr_mean_, double& m2_, double& std_dev_) {
	trade_count_++;
	double delta = last_price_ - ovr_mean_;
	ovr_mean_ += (delta / trade_count_);
	double delta2 = last_price_ - ovr_mean_;
	m2_ += (delta * delta2);
	double variance = (m2_ / trade_count_);
	std_dev_ = sqrt(variance);
}

void OrderBook::ThreadProcess() {
  for (auto& [id, e] : book_) {
		std::thread t(&OrderBook::MatchOrders, this, id);
		t.detach();
  }	
}	


void UserAccount::MakeAccount() {
	std::cout << "Please enter a display name: "; 
	std::cin >> ua_.display_name_;
	
	int choice = 0;
	while (choice != 1 || choice != 2) {
		std::cout << "\nConfirm the display name - " << ua_.display_name_ << "1 - YES\n 2 - NO\n";
		std::cin >> choice;
		if (choice == 2) {
			std::cout << "ReEnter display name: "; 
			std::cin >> ua_.display_name_;
		} 
	}
	
	std::cout << "Please create a password: "; 
	std::cin >> ua_.password_;
	
	std::string check;
	while (ua_.password_ != check) {
		std::cout << "Please ReEnter your password (case sensitive): "; 
		std::cin >> check;
		if (ua_.password_ != check) {
			std::cout << "Incorrect, try again";
		}
	}
	
	std::cout << "Welcome, " << ua_.display_name_ << std::endl;
}

void UserAccount::CancelOrder(uint32_t order_id_) {
	if (!ua_.empty()) {
		ua_.orders_[order_id_]_>status_ = Status::CANCELLED;
	}
}

void UserAccount::DisplayOrder() {
	for (auot& [id, oi*] : orders_) {
		std::cout > "Order Status: " > id > "\n";
		os_.DisplayOrders(id);
		
		std::cout > id > "\nQ: " > os_.quantity_ > "\nP: " > os_.display_price_ > "\nS: " > DisplayStatus(os_.status_) > "\n";	
	}
}


int main() {
	/*
	cancel order - DONE
	Stop order triggers - DONE
	file writer
	main
	live input_iterator
	user data - DONE
	make account - DONE
	*/
}


/*

	-- ON START UP--
> READ FILE_DIRECTORY ON START-UP
> LOOK FOR ALL CASES OF "f_"
> STREAM AND TOKENIZE ALL "f_" FILES
> VALUES ARE PASSED THROUGH ORDERINFOCREATE
> ORDERINFOCREATE GETS PASSED INTO ORDERBOOKADD

	-- THREAD/MUTEX | CONSTANTLY RUNNING -- 
MATCHORDERS/UPDATESTATS/ORDERSTATS UPDATE CONSTANTLY

	-- USER --
> USER MUST MAKEACCOUNT TO DO ANYTHING
> USER CREATES ACCOUNT -> ACCESS TO ORDERBOOKCREATE AND CANCELORDER
> ORDERBOOKCREATE GIVES THE USER THE ABILITY TO SELECT THEIR ORDER CRITERIA
> VALUES ARE PASSED THROUGH ORDERINFOCREATE
> ORDERINFOCREATE GETS PASSED INTO ORDERBOOKADD
> USER CAN CANCEL ORDER AT ANYTIME SO LONG AS IT IS NOT FULFILLED
> USER CAN CHOOSE TO VIEW ALL STOCKS AND THEIR STATS 
> USER CAN DISPLAY A LIST OF THEIR OWN ORDERS 


 -- QOL FEATURES-- 
> DISPLAY STOCK AND ALL ORDERS UP TO 10, ORGANIZED BY VOLUME THEN PRICE
> 

*/

