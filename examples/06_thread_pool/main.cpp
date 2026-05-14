#include <chrono>
#include <iostream>
#include <promise/MessageQueue.h>

/// Example 6: Thread Pool and MessageQueue
/// Demonstrates promise::Pool for multi-threaded dispatch and MessageQueue for
/// single-queue dispatch.

Promise<void> ThreadPoolExample() {
  std::cout << "=== Thread Pool Example ===\n";

  promise::Pool<4> pool{"workers"}; // Create a pool with 4 threads

  // Dispatch synchronous work to pool
  auto task1 = pool.Dispatch([]() {
    std::cout << "Task 1 running on thread pool\n";
    return 42;
  });

  auto task2 = pool.Dispatch([]() {
    std::cout << "Task 2 running on thread pool\n";
    return std::string("result");
  });

  auto task3 = pool.Dispatch([]() {
    std::cout << "Task 3 running on thread pool\n";
    // Void return
  });

  int result1 = co_await task1;
  std::string result2 = co_await task2;
  co_await task3;

  std::cout << "Task 1 result: " << result1 << "\n";
  std::cout << "Task 2 result: " << result2 << "\n";
  std::cout << "Task 3: completed\n\n";

  co_return;
}

// Not Yet ;)
// Promise<void> PoolWithMultipleTasks() {
//     std::cout << "=== Pool with Multiple Tasks ===\n";

//     promise::Pool<3> pool{"computation"};

//     // Dispatch multiple CPU-bound tasks
//     std::vector<WPromise<int>> tasks;

//     for (int i = 0; i < 6; ++i) {
//         auto task = pool.Dispatch([i]() {
//             std::cout << "Task " << i << " starting\n";
//             [[maybe_unused]] int sum = 0;
//             for (int j = 0; j < 1000000; ++j) {
//                 sum += j;
//             }
//             std::cout << "Task " << i << " done\n";
//             return i * 10;
//         });
//         tasks.push_back(task);
//     }

//     // Collect all results
//     auto all = promise::All(tasks.begin(), tasks.end());
//     co_await all;

//     std::cout << "All pool tasks completed\n\n";

//     co_return;
// }

Promise<void> MessageQueueExample() {
  std::cout << "=== MessageQueue Example ===\n";

  promise::MessageQueue queue{"ui_thread"};

  // Tasks are executed sequentially on the queue's thread
  auto task1 = queue.Dispatch([]() {
    std::cout << "Task 1 on queue thread\n";
    return 10;
  });

  auto task2 = queue.Dispatch([]() {
    std::cout << "Task 2 on queue thread (after Task 1)\n";
    return 20;
  });

  auto task3 = queue.Dispatch([]() {
    std::cout << "Task 3 on queue thread (after Task 2)\n";
    return 30;
  });

  int r1 = co_await task1;
  int r2 = co_await task2;
  int r3 = co_await task3;

  std::cout << "Results: " << r1 << ", " << r2 << ", " << r3 << "\n";
  std::cout << "Queue thread ID: " << queue.ThreadId() << "\n\n";

  co_return;
}

Promise<void> PoolWithDelayedDispatch() {
  std::cout << "=== Pool with Delayed Dispatch ===\n";

  promise::Pool<2> pool{"delayed"};

  auto delayed_task = pool.Dispatch(
      []() {
        std::cout << "This executes after 100ms\n";
        return "delayed_result";
      },
      std::chrono::milliseconds(100));

  std::cout << "Dispatched delayed task\n";

  auto result = co_await delayed_task;
  std::cout << "Delayed task result: " << result << "\n\n";

  co_return;
}

Promise<void> ChainedPoolWork() {
  std::cout << "=== Chained Pool Work ===\n";

  promise::Pool<2> pool{"chained"};

  auto step1 = pool.Dispatch([]() {
    std::cout << "Step 1: Processing input\n";
    return 100;
  });

  auto step2 = co_await step1
                   .Then([&pool](int const &value) -> Promise<int> {
                     co_return co_await pool.Dispatch([value]() {
                       std::cout << "Step 2: Transforming " << value << "\n";
                       return value * 2;
                     });
                   })
                   .Then([&pool](int const &value) -> Promise<int> {
                     co_return co_await pool.Dispatch([value]() {
                       std::cout << "Step 3: Finalizing " << value << "\n";
                       return value + 23;
                     });
                   });

  std::cout << "Final result: " << step2 << "\n\n";

  co_return;
}

int main() {
  WPromise<void> demo{[]() -> Promise<void> {
    co_await ThreadPoolExample();
    //  co_await PoolWithMultipleTasks();
    co_await MessageQueueExample();
    co_await PoolWithDelayedDispatch();
    co_await ChainedPoolWork();
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
