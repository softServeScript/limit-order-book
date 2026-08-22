#include "order_book.hpp"
#include "order.hpp"
#include "stock.hpp"
#include "enums.hpp"
#include "ticker.hpp"
#include "order_status.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <iostream>

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
			OrderBookCreation::OrderBookAdd(side, stock_id, order_id, quantity, price, status, type, timestamp);
		}
	}
}