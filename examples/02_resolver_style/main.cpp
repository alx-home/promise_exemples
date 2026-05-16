#include <iostream>
#include <promise/promise.h>
#include <thread>

/// Example 2: Resolver-Style Promises
/// Demonstrates Promise<T, true> with explicit Resolve<T> and Reject handles.

WPromise<int> AsyncOperation() {
  return [](Resolve<int> const &resolve, Reject const &) -> Promise<int, true> {
    // Simulate async work - in practice this could be a callback or background
    // task
    std::thread worker([resolve]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      resolve(42);
    });
    worker.detach();

    co_return;
  };
}

WPromise<int> ConditionalResolve() {
  return [](Resolve<int> const &resolve,
            Reject const &reject) -> Promise<int, true> {
    bool success = true; // Change to false to test rejection
    if (success) {
      resolve(100);
    } else {
      reject(std::runtime_error("Operation failed"));
    }
    co_return;
  };
}

WPromise<void> ResolverStyleExample() {
  return [] -> Promise<void> {
    std::cout << "=== Resolver Style Example ===\n";

    int value = co_await
        [](Resolve<int> const &resolve, Reject const &) -> Promise<int, true> {
      resolve(7);
      co_return;
    };
    std::cout << "Direct resolve result: " << value << "\n\n";

    co_return;
  };
}

WPromise<void> CreateHelperExample() {
  return [] -> Promise<void> {
    std::cout << "=== promise::Create<T>() Example ===\n";

    auto [prom, resolve, reject] = promise::Create<std::string>();
    (void)reject;

    // Simulate background task
    std::thread task([resolve]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      (*resolve)("Hello from background!");
    });
    task.detach();

    std::string result = co_await prom;
    std::cout << "Result: " << result << "\n\n";

    co_return;
  };
}

WPromise<void> ConditionalExample() {
  return [] -> Promise<void> {
    std::cout << "=== Conditional Resolve/Reject Example ===\n";

    // Success case
    [[maybe_unused]] auto success_result = co_await WPromise<int>{
        [](Resolve<int> const &resolve, Reject const &) -> Promise<int, true> {
          resolve(99);
          co_return;
        }}.Then([](int const &value) {
      std::cout << "Success: " << value << "\n";
      return value;
    });

    // Failure case
    auto failure_result = co_await WPromise<int>{
        [](Resolve<int> const &, Reject const &reject) -> Promise<int, true> {
          reject(std::runtime_error("Intentional failure"));
          co_return;
        }}.Catch([](std::exception_ptr) {
      std::cout << "Caught failure in chain\n";
      return -1;
    });

    std::cout << "Failure result: " << failure_result << "\n\n";

    co_return;
  };
}

int main() {
  WPromise<void> demo{[] -> Promise<void> {
    co_await ResolverStyleExample();
    co_await CreateHelperExample();
    co_await ConditionalExample();
    co_return;
  }};

  while (!demo.Done()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  if (demo.Rejected()) {
    try {
      std::rethrow_exception(demo.Exception());
    } catch (std::exception const &ex) {
      std::cerr << "Fatal error: " << ex.what() << "\n";
      return 1;
    }
  }

  return 0;
}
