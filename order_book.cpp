#include "ticker.hpp"
#include "enums.hpp"
#include "stock.hpp"
#include "order.hpp"
#include "stats.hpp"
#include "order_status.hpp"
#include "order_book.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <iostream>
#include <vector>

void OrderBookCreation::LoadFiles(const std::string& file_directory) {
	namespace fs = std::filesystem; 
	
	for (fs::directory_entry& dir_entry : fs::directory_iterator(file_directory)) { // range file, string: "file_directory"
		
		if (dir_entry.path().filename().string().substr(0, 2) != "f_") { // file name check, create entry
			continue; 
		}
		
		std::ifstream f(dir_entry.path()); // open file
		
		if (!f.is_open()) { // check for corruption
			continue;
		}
		
		while (std::string line; std::getline(f, line)) { // iterate strings in file 
			std::istringstream ss(line); // tokenize
			std::string a, b, c, d, e, f, g; // stock_id, order_id, quantity, price, status, type, timestamp
			uint8_t side = 0;

			ss >> a >> b >> c >> d >> e >> f >> g;
			
			if (Ticker::map[0] != a[0]) {
				side = 1;
			}
			
			a.erase(0, 1);
			
			// convert strings back to int/long/longlong
			uint32_t stock_id(std::stoi(a));
			uint32_t order_id(std::stoi(b));
			uint16_t quantity(std::stoi(c));
			uint16_t price(std::stoi(d));
			uint8_t status(std::stoi(e));
			uint8_t type(std::stoi(f));
			uint64_t timestamp(std::stoll(g));
						
			// class overload passing through the values
			OrderBookAdd(side, stock_id, order_id, quantity, price, status, type, timestamp);
		}
	}
}

void OrderBookCreation::OrderBookAdd(uint8_t s, uint32_t s_id, uint32_t o_id, uint16_t q, uint16_t p, uint8_t status_, uint8_t type_, uint64_t t) {
	if (!(book_.contains(s_id))) { // if stock/KEY doesnt exist
		book_[s_id].s_.stock_id_ = s_id;
		book_[s_id].si_.stock_id_ = s_id;
		book_[s_id].s_.stock_id_ = s_id; // create stats
		book_[s_id].s_.volume_ = q; // add volume stats
		book_[s_id].si_.quantity_ = q;
		//si.stock_price_ TBD LATER TIME
		book_[s_id].si_.status_ = Status::OPEN;	// OPEN
	} else {
		book_[s_id].si_.quantity_ += q;
		book_[s_id].s_.volume_ += q; // update volumt stats
	}
	
	
	// create order.hpp
	OrderInfo oi(book_[s_id].si_);
	oi.side_ = static_cast<Side>(s);
	oi.order_id_ = o_id;
	oi.quantity_ = q;
	oi.encoded_price_ = p;
	oi.status_ = static_cast<Status>(status_);
	oi.type_ = static_cast<Type>(type_);
	oi.order_time_ = std::chrono::nanoseconds(t);			

	if (type_ == 2 || type_ == 3) {
		if (s == 0) {
			book_[s_id].sov_.buy_.push_back(oi);
		} else {
			book_[s_id].sov_.sell_.push_back(oi);
		}
	} else {
		if (s == 0) {
			book_[s_id].oq_.buy_.push(oi);
		} else {
			book_[s_id].oq_.sell_.push(oi);
		}
	}
}

void OrderBookCreation::MatchOrders(uint32_t stock_id_) { // std::ref(book_[stock_id_])
	while (book_[stock_id_].oq_.sell_.top().encoded_price_ <= book_[stock_id_].oq_.buy_.top().encoded_price_) { // check for matches
		OrderInfo sell = book_[stock_id_].oq_.sell_.top();
		OrderInfo buy = book_[stock_id_].oq_.buy_.top();
		book_[stock_id_].oq_.buy_.pop();
		book_[stock_id_].oq_.sell_.pop();
		
		if (book_[stock_id_].oq_.sell_.top().quantity_ != book_[stock_id_].oq_.buy_.top().quantity_) { // if sell != buy
			if (book_[stock_id_].oq_.sell_.top().quantity_ < book_[stock_id_].oq_.buy_.top().quantity_) { // if sell < buy
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
		
		book_[stock_id_].s_.last_price_ = static_cast<double>(book_[stock_id_].oq_.sell.encoded_price_ / 100.0);
		book_[stock_id_].s_.trade_count_++;
	}
}