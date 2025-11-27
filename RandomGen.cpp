#include "RandomGen.h"
#include <iostream>
#include <random>
#include <ctime>

bool SuspendIfOdd::await_ready() const noexcept {
    return (value % 2 == 0);
}

void SuspendIfOdd::await_suspend(std::coroutine_handle<>) const noexcept {}

void SuspendIfOdd::await_resume() const noexcept {}

RandomGenerator RandomGenerator::promise_type::get_return_object() {
    return RandomGenerator{std::coroutine_handle<promise_type>::from_promise(*this)};
}

std::suspend_always RandomGenerator::promise_type::initial_suspend() { return {}; }
std::suspend_always RandomGenerator::promise_type::final_suspend() noexcept { return {}; }
void RandomGenerator::promise_type::unhandled_exception() { std::terminate(); }
void RandomGenerator::promise_type::return_void() {}

RandomGenerator::RandomGenerator(std::coroutine_handle<promise_type> h) : handle(h) {}

RandomGenerator::~RandomGenerator() {
    if (handle) handle.destroy();
}

RandomGenerator::RandomGenerator(RandomGenerator&& other) noexcept : handle(other.handle) {
    other.handle = nullptr;
}

void RandomGenerator::resume() {
    if (handle && !handle.done()) {
        handle.resume();
    }
}

bool RandomGenerator::done() const {
    return !handle || handle.done();
}

RandomGenerator createRandomSequence(std::queue<int>& q, int count) {
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(1, 256);

    for (int i = 0; i < count; ++i) {
        int num = dist(rng);
        q.push(num);
        std::cout << "[Co] Generated: " << num << std::endl;
        co_await SuspendIfOdd{num};
    }
}