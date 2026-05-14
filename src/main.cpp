#include <chrono>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <thread>

#include <promise/promise.h>

WPromise<int> BuildChain(bool fail_fast) {
    if (fail_fast) {
        return Promise<int>::Reject<std::runtime_error>("forced failure");
    }

    return WPromise<int>{[]() -> Promise<int> {
               co_return 21;
           }}
        .Then([](int const& value) { return value * 2; })
        .Then([](int const& value) -> Promise<int> {
            co_return value + 1;
        })
        .Catch([](std::exception_ptr) -> Promise<int> {
            co_return -1;
        })
        .Finally([]() {
            std::cout << "Primary chain finished\n";
        });
}

Promise<void> RunDemo() {
    int const chained_value = co_await BuildChain(false);
    std::cout << "Then/Catch/Finally result: " << chained_value << "\n";

    auto [created, resolve, reject] = promise::Create<int>();
    (void)reject;
    (*resolve)(7);

    int const resolver_value = co_await created.Then([](int const& value) {
        return value + 35;
    });

    std::cout << "Resolver-style result: " << resolver_value << "\n";
    co_return;
}

int main() {
#ifdef PROMISE_MEMCHECK
    auto const memcheck = promise::Memcheck();
    (void)memcheck;
#endif

    WPromise<void> runner{[]() -> Promise<void> {
        co_await RunDemo();
        co_return;
    }};

    while (!runner.Done()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    if (runner.Rejected()) {
        try {
            std::rethrow_exception(runner.Exception());
        } catch (std::exception const& ex) {
            std::cerr << "Demo failed: " << ex.what() << "\n";
        }
        return 1;
    }

    return 0;
}
