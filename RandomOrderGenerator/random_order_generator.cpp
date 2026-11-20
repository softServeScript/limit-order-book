#include "random_order_generator.hpp"
#include "../Engine/engine_util.hpp"
#include "../Engine/order_info.hpp"
#include "../Engine/order_book.hpp"
#include <cstdint>
#include <memory>
#include <random>

/*
// Orders per second
void OPS(std::atomic_bool& running) {
    while (running.load(std::memory_order_relaxed)) {
        using timer = std::chrono::steady_clock;

        timer::time_point start = timer::now(); //

        timer::time_point end = start + std::chrono::seconds(1);
    }
}
*/


// RNG template
int RandomNumberGenerator(int min, int max) {
    std::random_device rand_num;
    std::mt19937 gen(rand_num());
    std::uniform_int_distribution<int>  dist(min, max);

    return dist(gen);
}

// utilizes std::atomic_bool running(true);
// if status == true; creates orders
void RandomOrderGenerator(OrderBook& ob, std::atomic_bool& running) {

    while (running.load(std::memory_order_relaxed)) {
        uint8_t side = RandomNumberGenerator(0, 1);
        uint32_t stock_id = RandomNumberGenerator(4, 10000000);

        while (side == 0 && (!(ob.CheckForStock(stock_id)))) {
            side = RandomNumberGenerator(0, 1);
            stock_id = RandomNumberGenerator(4, 10000000);
        }

        ob.OrderBookAdd(side, stock_id, CreateFirstLast(stock_id), RandomNumberGenerator(1, 65535), RandomNumberGenerator(1, 9999), 0, RandomNumberGenerator(0, 3), 0);
    }

}

