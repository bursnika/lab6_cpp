#ifndef RANDOM_GEN_H
#define RANDOM_GEN_H

#include <coroutine>
#include <queue>

struct SuspendIfOdd {
    int value;
    bool await_ready() const noexcept;
    void await_suspend(std::coroutine_handle<>) const noexcept;
    void await_resume() const noexcept;
};

class RandomGenerator {
public:
    struct promise_type {
        RandomGenerator get_return_object();
        std::suspend_always initial_suspend();
        std::suspend_always final_suspend() noexcept;
        void unhandled_exception();
        void return_void();
    };

    explicit RandomGenerator(std::coroutine_handle<promise_type> h);
    ~RandomGenerator();

    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;
    RandomGenerator(RandomGenerator&& other) noexcept;

    void resume();
    bool done() const;

private:
    std::coroutine_handle<promise_type> handle;
};

RandomGenerator createRandomSequence(std::queue<int>& q, int count);

#endif