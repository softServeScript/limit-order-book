#pragma once
#include <string>
#include <cstdint>
#include <chrono>
#include <algorithm>
#include <random>
#include "ticker.hpp"

uint64_t Timestamp() { // Timestamp by nanoseconds
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string DecodeID(uint32_t id) { // Ticker decoder for display
	std::string word;
	
	for (int i = 0; i < 4; i ++) {
		uint8_t copy = (id  % 100);
		id = (id / 100);
		word += Ticker::map[copy];
	}
	
	std::reverse(word.begin(), word.end());
	return word;
}

uint32_t CreateID(uint8_t fl){
	std::random_device rand_id;
	std::mt19937 gen(rand_id());
	std::uniform_int_distribution<uint32_t> dist(100000, 999999); // first/last digit of APL + 6 digits
	uint32_t r_id = dist(gen);
	
	r_id = (fl * 10000000) + r_id;
	return r_id;
}

std::string DisplayStatus(uint8_t x) {
	std::string word;
	
	switch (x) {
		case 0:
			word = "OPEN";
		case 1:
			word = "CANCELLED";
		case 2:
			word = "PARTIAL";
		case 3:
			word = "FILLED";
		case 4:
			word = "EXPIRED";
	}
	
	return word;
}