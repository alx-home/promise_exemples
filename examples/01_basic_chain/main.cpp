#include <iostream>
#include <promise/promise.h>

/// Example 1: Basic Promise Chaining
/// Demonstrates Then/Catch/Finally pattern similar to JavaScript promises.

Promise<int> FetchUserAge() { co_return 25; }

Promise<std::string> FormatAgeMessage(int age) {
  co_return "User age: " + std::to_string(age);
}

Promise<void> BasicChainExample() {
  std::cout << "=== Basic Chain Example ===\n";

  auto result =
      co_await WPromise<int>{[]() -> Promise<int> { co_return 10; }}
          .Then([](int const &value) {
            std::cout << "Step 1: Got value " << value << "\n";
            return value * 2;
          })
          .Then([](int const &value) -> Promise<int> {
            std::cout << "Step 2: Doubled to " << value << "\n";
            co_return value + 5;
          })
          .Then([](int const &value) {
            std::cout << "Step 3: Added 5, result = " << value << "\n";
            return value;
          })
          .Catch([](std::exception_ptr) {
            std::cout << "Error occurred\n";
            return -1;
          })
          .Finally([]() { std::cout << "Finally: Cleanup complete\n"; });

  std::cout << "Final result: " << result << "\n\n";
  co_return;
}

Promise<void> ChainWithFunctions() {
  std::cout << "=== Chain with Functions ===\n";

  auto age = co_await FetchUserAge();
  auto message = co_await FormatAgeMessage(age);
  std::cout << message << "\n\n";

  co_return;
}

Promise<void> ErrorHandlingExample() {
  std::cout << "=== Error Handling Example ===\n";

  auto result = co_await WPromise<int>{[]() -> Promise<int> {
                  throw std::runtime_error("Something went wrong!");
                  co_return 0;
                }}
                    .Then([](int const &value) {
                      std::cout << "This won't execute\n";
                      return value;
                    })
                    .Catch([](std::exception_ptr) {
                      std::cout << "Caught exception in Then/Catch chain\n";
                      return -1;
                    });

  std::cout << "Result after error: " << result << "\n\n";
  co_return;
}

int main() {
  WPromise<void> demo{[]() -> Promise<void> {
    co_await BasicChainExample();
    co_await ChainWithFunctions();
    co_await ErrorHandlingExample();
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
