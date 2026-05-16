#include <iostream>
#include <promise/StatePromise.h>
#include <thread>

/// Example 5: StatePromise for Ready/Done Workflows
/// Demonstrates StatePromise for state-machine style coordination.

WPromise<void> BasicStatePromiseExample() {
  return [] -> Promise<void> {
    std::cout << "=== Basic StatePromise Example ===\n";

    StatePromise state;

    // Worker that waits for ready, then waits for done
    WPromise<void> worker{[&] -> Promise<void> {
      std::cout << "Worker: Waiting for Ready...\n";
      co_await state.WaitReady();
      std::cout << "Worker: Ready signal received, starting work\n";

      std::cout << "Worker: Waiting for Done...\n";
      co_await state.WaitDone();
      std::cout << "Worker: Done signal received, exiting\n";

      co_return;
    }};

    // Coordinator sends Ready, then Done
    std::thread coordinator([&] {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      std::cout << "Coordinator: Sending Ready\n";
      state.Ready();

      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      std::cout << "Coordinator: Sending Done\n";
      state.Done();
    });
    coordinator.detach();

    while (!worker.Done()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "Is Done? " << state.IsDone() << "\n\n";

    co_return;
  };
}

WPromise<void> WaitAnyStateExample() {
  return [] -> Promise<void> {
    std::cout << "=== Wait Any State Example ===\n";

    StatePromise state;

    WPromise<void> waiter{[&] -> Promise<void> {
      std::cout << "Waiter: Waiting for Ready or Done\n";
      co_await state.Wait(); // Returns when either Ready or Done is signaled
      std::cout << "Waiter: Received either Ready or Done\n";

      co_await state.WaitDone(); // Now wait specifically for Done
      std::cout << "Waiter: Done was signaled\n";

      co_return;
    }};

    std::thread coordinator([&] {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      std::cout << "Coordinator: Signaling Ready\n";
      state.Ready();

      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      std::cout << "Coordinator: Signaling Done\n";
      state.Done();
    });
    coordinator.detach();

    while (!waiter.Done()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "\n";

    co_return;
  };
}

WPromise<void> MultipleWorkersExample() {
  return [] -> Promise<void> {
    std::cout << "=== Multiple Workers Example ===\n";

    StatePromise state;
    int workers_started = 0;
    int workers_finished = 0;

    // Create multiple workers
    WPromise<void> worker1{[&] -> Promise<void> {
      co_await state.WaitReady();
      workers_started++;
      std::cout << "Worker 1: Starting\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      std::cout << "Worker 1: Waiting for done\n";
      co_await state.WaitDone();
      workers_finished++;
      std::cout << "Worker 1: Finished\n";
      co_return;
    }};

    WPromise<void> worker2{[&] -> Promise<void> {
      co_await state.WaitReady();
      workers_started++;
      std::cout << "Worker 2: Starting\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      std::cout << "Worker 2: Waiting for done\n";
      co_await state.WaitDone();
      workers_finished++;
      std::cout << "Worker 2: Finished\n";
      co_return;
    }};

    WPromise<void> worker3{[&] -> Promise<void> {
      co_await state.WaitReady();
      workers_started++;
      std::cout << "Worker 3: Starting\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      std::cout << "Worker 3: Waiting for done\n";
      co_await state.WaitDone();
      workers_finished++;
      std::cout << "Worker 3: Finished\n";
      co_return;
    }};

    std::thread coordinator([&] {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      std::cout << "Coordinator: Signaling Ready to all\n";
      state.Ready();

      std::this_thread::sleep_for(std::chrono::milliseconds(80));
      std::cout << "Coordinator: Signaling Done to all\n";
      state.Done();
    });
    coordinator.detach();

    while (!worker1.Done() || !worker2.Done() || !worker3.Done()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "Workers started: " << workers_started
              << ", Finished: " << workers_finished << "\n\n";

    co_return;
  };
}

WPromise<void> ResetExample() {
  return [] -> Promise<void> {
    std::cout << "=== Reset for Multiple Cycles ===\n";

    StatePromise state;

    WPromise<void> worker{[&] -> Promise<void> {
      for (int i = 0; i < 2; ++i) {
        std::cout << "Cycle " << i << ": Waiting for Ready\n";
        co_await state.WaitReady();
        std::cout << "Cycle " << i << ": Got Ready, working...\n";

        std::cout << "Cycle " << i << ": Waiting for Done\n";
        co_await state.WaitDone();
        std::cout << "Cycle " << i << ": Got Done\n";

        if (i < 1) {
          std::cout << "Cycle " << i << ": Resetting for next cycle\n";
          state.Reset();
        }
      }
      co_return;
    }};

    std::thread coordinator([&] {
      // Cycle 0
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      state.Ready();
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      state.Done();

      // Cycle 1
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      state.Ready();
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      state.Done();
    });
    coordinator.detach();

    while (!worker.Done()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "\n";

    co_return;
  };
}

int main() {
  WPromise<void> demo{[]() -> Promise<void> {
    co_await BasicStatePromiseExample();
    co_await WaitAnyStateExample();
    co_await MultipleWorkersExample();
    co_await ResetExample();
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
