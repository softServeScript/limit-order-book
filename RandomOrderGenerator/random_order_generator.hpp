#pragma once
#include "../Engine/order_book.hpp"
#include <atomic>

// Orders Per Second
void OPS(std::atomic_bool& running);

// RNG template
int RandomNumberGenerator(int min, int max);

// utilizes std::atomic_bool running(true);
// if status == true; creates orders
void RandomOrderGenerator(OrderBook& ob, std::atomic_bool& running);