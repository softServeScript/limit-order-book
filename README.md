# Limit Order Book: C++ Matching Engine

## Overview
A back-end limit order book, or LOB, matching engine written in C++, modeled/inspired after the financial infrastructures used by quant firms and exchanges. Built from scratch as a solo project to demonstrate systems-level C++ design, data structure selection, and low-level engineering decisions.

**Project Status (09/03/2026):** Core architecture mostly complete. `user.hpp`, `order_book.hpp`, and `order_book.cpp` still in active development. Front-end WIP.

---

## Summary

A limit order book is the heart behind every financial exchange. It maintains a record of all buy and sell orders for a given asset, matches them based on price-time priority, and executes trades when conditions are met.

This implementation handles:
- Order creation, matching, and cancellation
- Stop orders (stop-limit and stop-market)
- Running statistics per stock using Welford's online algorithm
- Persistent file storage with startup preloading
- Multithreaded order matching with mutex protection
- Privacy-conscious user identification via base-26 ticker encoding

---

## Architecture

### On Startup
```
READ file directory on startup
→ Scan for all files prefixed with "f_"
→ Stream and tokenize each file
→ Pass values through OrderInfoCreate (preserves original timestamps)
→ OrderInfoCreate passes into OrderBookAdd
```

### Runtime (Threaded)
```
MatchOrders / UpdateStats / OrderStats run continuously on a dedicated thread
Protected by std::mutex for thread-safe queue access
```

### User Flow
```
User creates account
→ Access to OrderBookCreate and CancelOrder
→ OrderBookCreate: user selects order criteria
→ Values passed through OrderInfoCreate → OrderBookAdd
→ User can cancel any unfilled order at any time
→ User can view all stocks and their stats
→ User can display their own order list
```

---

## File Structure

| File | Status | Description |
|------|--------|-------------|
| `enums.hpp` | Complete | `Side`, `Status`, `Type` enums |
| `order.hpp` | Complete | `OrderInfo` struct — core order representation |
| `order_status.hpp` | Complete | Lightweight status display wrapper |
| `stock.hpp` | Complete | `StockInfo` struct |
| `ticker.hpp` | Complete | Base-26 ticker encoding map |
| `stats.hpp` | Complete | Welford's algorithm, circular price buffer |
| `user.hpp` |  In Progress | `UserAcc` struct, order tracking |
| `order_book.hpp` |  In Progress | Central book structure, queue definitions |
| `order_book.cpp` | In Progress | File loading, matching engine implementation |
| `utils.hpp` | Planned | Reusable helpers — `Timestamp()`, `DecodeID()` |

---

## Key Design Decisions

**1. Status enum over uint8_t**
Chose `enum class Status` over raw `uint8_t` for type safety and readability. Eliminates magic numbers in order state logic.

**2. Base-26 ticker encoding**
26⁴ = 456,976 unique identifiers. Indices 0 and 1 map to `#` (buyer) and `*` (seller) for side identification. Encodes user identity without exposing names, private by design.

**3. Stock vs Order separation**
`StockInfo` and `OrderInfo` are distinct structs. Separates symbol metadata from order data, allowing independent storage and retrieval.

**4. std::priority_queue for active orders**
Chosen for O(log n) insertion and O(1) top access. Trade-off: less flexibility for cancellation and modification. Cancellation handled via lazy deletion using `std::unordered_set<uint32_t> cancelled_orders_`.

**5. Function overload for CreateOrder**
Separate overloads handle preloaded (file) orders vs new user orders. Preserves original timestamps on preloaded orders. Critical for correct price-time priority after restart.

**6. std::filesystem for persistence**
C++17 `<filesystem>` used to scan the file directory on startup. All persistent order files are prefixed `f_` for identification. Space-delimited plaintext with encoded fields. Intended to replicate live trade locally.

**7. Welford's online algorithm for statistics**
Running mean and standard deviation computed incrementally using Welford's method to avoid recomputing from scratch on every update. 10 recent transactions circular buffer tracks recent price mean.

**8. std::thread + std::mutex for matching**
`MatchOrders` runs on a detached thread. Class-level `std::mutex mtx_` protects all queue access. Chosen over async/future for direct control over thread lifecycle.

---

## File Format

Persistent order files follow this schema:

```
#AAPL 00001 15000 50 172800000000 1
*AAPL 00001 15000 50 172800000000 1
```

Fields: `stock_id_` | `order_id_` | `quantity` | `price` | `timestamp` | `status`

Leading `#` = buyer, `*` = seller. Side is stripped from the ID on load.

---

## QoL Features
- Display stock and all orders up to 10 deep, organized by volume then price
- Per-user order list display
- Stats view per stock (mean, std deviation, recent price buffer)
> More QOL in planning...

---

## Build

```bash
g++ -std=c++17 -pthread order_book.cpp -o lob
```

> Requires C++17 or later. Linux/macOS recommended.

---

## Goals and Intent
This project was built to demonstrate:
- Independent systems design without frameworks or templates
- Justified data structure selection under real constraints
- Low-level C++ patterns: RAII, custom comparators, lazy deletion, online algorithms
- Production-adjacent concerns: persistence, threading, privacy, file I/O

More importantly, this project was built out of a genuine passion for finance, economics, and systems programming. The goal was to understand, from the ground up, a small piece of the underlying mechanics that quantitative trading runs on, while designing a system with both the engineer and the end user in mind.


