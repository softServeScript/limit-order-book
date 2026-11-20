#pragma once
#include "ticker.hpp"
#include <string>
#include <cstdint>
#include <chrono>
#include <algorithm>
#include <random>

// Timestamp in NANO-seconds
inline uint64_t Timestamp() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

// Converts TICKER (AAPL) to ID (02021814)
inline std::string IDToString(uint32_t id) {
	std::string word;
	
	for (int i = 0; i < 4; i ++) {
		uint8_t copy = (id  % 100);
		id = (id / 100);
		word += Ticker::map[copy];
	}
	
	std::reverse(word.begin(), word.end());
	return word;
}

/*
// Converts ID (02021814) to TICKER (AAPL)
inline uint32_t StringToID() {
	// not done
}
*/

inline uint32_t CreateOrderID(uint16_t first_last){
	std::random_device rand_id;
	std::mt19937 gen(rand_id());
	std::uniform_int_distribution<uint32_t> dist(100000, 999999); // first/last digit of APL + 6 digits
	uint32_t r_id = dist(gen);
	
	r_id = (first_last * 10000000) + r_id;
	return r_id;
}

// Converts FIRST & LAST to create unique order IDs
inline uint32_t CreateFirstLast(uint32_t stock_id) {
	uint8_t first = stock_id / 1000000;
	uint8_t last = stock_id % 100;

	return CreateOrderID(((first * 100) + last));
}

