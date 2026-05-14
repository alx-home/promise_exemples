#include <iostream>
#include <promise/promise.h>
#include <variant>

/// Example 3: promise::All and promise::Race
/// Demonstrates combining multiple promises.

Promise<int> Task1() { co_return 10; }

Promise<double> Task2() { co_return 3.14; }

Promise<std::string> Task3() { co_return "complete"; }

Promise<void> PromiseAllExample() {
  std::cout << "=== promise::All Example ===\n";

  auto all = promise::All(Task1, Task2, Task3);

  auto [i, d, s] = co_await all;

  std::cout << "Result 1 (int): " << i << "\n";
  std::cout << "Result 2 (double): " << d << "\n";
  std::cout << "Result 3 (string): " << s << "\n\n";

  co_return;
}

Promise<void> PromiseAllWithVoid() {
  std::cout << "=== promise::All with Void ===\n";

  auto all = promise::All(Task1, []() -> Promise<void> { co_return; }, Task2);

  auto [i, d] = co_await all; // void result is omitted from tuple

  std::cout << "Result 1: " << i << ", Result 2: " << d << "\n";
  std::cout << "Void task was awaited but not included in tuple\n\n";

  co_return;
}

Promise<void> PromiseRaceExample() {
  std::cout << "=== promise::Race Example ===\n";

  auto race = promise::Race([]() -> Promise<int> { co_return 1; },
                            []() -> Promise<int> { co_return 2; },
                            []() -> Promise<int> { co_return 3; });

  // All same type, result is just int
  int winner = co_await race;
  std::cout << "First to complete: " << winner << "\n\n";

  co_return;
}

Promise<void> PromiseRaceVariant() {
  std::cout << "=== promise::Race with Different Types ===\n";

  auto race = promise::Race([]() -> Promise<int> { co_return 42; },
                            []() -> Promise<double> { co_return 2.71; });

  auto result = co_await race;

  if (std::holds_alternative<int>(result)) {
    std::cout << "Winner: int = " << std::get<int>(result) << "\n";
  } else {
    std::cout << "Winner: double = " << std::get<double>(result) << "\n";
  }
  std::cout << "\n";

  co_return;
}

Promise<void> PromiseRaceWithError() {
  std::cout << "=== promise::Race with Error ===\n";

  auto race = promise::Race(
      []() -> Promise<int> {
        throw std::runtime_error("Task failed");
        co_return 0;
      },
      []() -> Promise<int> { co_return 99; });

  auto result = co_await race.Catch([](std::exception_ptr) {
    std::cout << "Caught error from race\n";
    return -1;
  });

  std::cout << "Race result: " << result << "\n\n";

  co_return;
}

int main() {
  WPromise<void> demo{[]() -> Promise<void> {
    co_await PromiseAllExample();
    co_await PromiseAllWithVoid();
    co_await PromiseRaceExample();
    co_await PromiseRaceVariant();
    co_await PromiseRaceWithError();
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
