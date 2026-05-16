#include <iostream>
#include <promise/CVPromise.h>
#include <thread>

/// Example 4: CVPromise for Async Notification
/// Demonstrates condition-variable-style signaling between coroutines.

WPromise<void>
CVPromiseNotifyExample() {
   return [] -> Promise<void> {
      std::cout << "=== CVPromise Notify Example ===\n";

      CVPromise ready;
      bool      worker_started    = false;
      bool      notifier_signaled = false;

      // Worker coroutine waiting for signal
      WPromise<void> worker{[&] -> Promise<void> {
         worker_started = true;
         std::cout << "Worker: Waiting for signal...\n";
         try {
            co_await *ready;
            std::cout << "Worker: Received signal!\n";
         } catch (const CVPromise::End&) {
            std::cout << "Worker: Notifier was destroyed\n";
         }
         co_return;
      }};

      // Simulate background task that signals
      std::jthread signaler([&] {
         std::this_thread::sleep_for(std::chrono::milliseconds(50));
         std::cout << "Signaler: Sending notification\n";
         ready.Notify();
         notifier_signaled = true;
      });

      // Wait for completion
      while (!worker.Done()) {
         std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }

      std::cout << "Worker started: " << worker_started
                << ", Notifier signaled: " << notifier_signaled << "\n\n";
      co_return;
   };
}

WPromise<void>
CVPromiseResetExample() {
   return [] -> Promise<void> {
      std::cout << "=== CVPromise Reset Example ===\n";

      CVPromise signal;
      int       notification_count = 0;

      // Multiple cycles of wait/reset
      WPromise<void> consumer{[&] -> Promise<void> {
         for (int i = 0; i < 3; ++i) {
            std::cout << "Cycle " << i << ": Waiting...\n";
            try {
               co_await *signal;
               notification_count++;
               std::cout << "Cycle " << i << ": Notified!\n";
            } catch (const CVPromise::End&) {
               break;
            }
         }
         co_return;
      }};

      std::jthread notifier([&]() {
         for (int i = 0; i < 3; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            std::cout << "Notifying cycle " << i << "\n";
            signal.Notify();
         }
      });

      while (!consumer.Done()) {
         std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }

      std::cout << "Total notifications received: " << notification_count << "\n\n";
      co_return;
   };
}

WPromise<void>
MultipleWaitersExample() {
   return [] -> Promise<void> {
      std::cout << "=== Multiple Waiters Example ===\n";

      CVPromise broadcast;
      int       waiter_count = 0;

      // Create multiple waiters
      WPromise<void> waiter1{[&] -> Promise<void> {
         std::cout << "Waiter 1: Waiting\n";
         co_await *broadcast;
         waiter_count++;
         std::cout << "Waiter 1: Received signal\n";
         co_return;
      }};

      WPromise<void> waiter2{[&] -> Promise<void> {
         std::cout << "Waiter 2: Waiting\n";
         co_await *broadcast;
         waiter_count++;
         std::cout << "Waiter 2: Received signal\n";
         co_return;
      }};

      WPromise<void> waiter3{[&] -> Promise<void> {
         std::cout << "Waiter 3: Waiting\n";
         co_await *broadcast;
         waiter_count++;
         std::cout << "Waiter 3: Received signal\n";
         co_return;
      }};

      // Signal all at once
      std::jthread signaler([&] {
         std::this_thread::sleep_for(std::chrono::milliseconds(50));
         std::cout << "Broadcasting signal to all waiters\n";
         broadcast.Notify();
      });

      while (!waiter1.Done() || !waiter2.Done() || !waiter3.Done()) {
         std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }

      std::cout << "Waiters that received signal: " << waiter_count << "\n\n";
      co_return;
   };
}

int
main() {
   WPromise<void> demo{[]() -> Promise<void> {
      co_await CVPromiseNotifyExample();
      co_await CVPromiseResetExample();
      co_await MultipleWaitersExample();
      co_return;
   }};

   while (!demo.Done()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
   }

   if (demo.Rejected()) {
      try {
         std::rethrow_exception(demo.Exception());
      } catch (std::exception const& ex) {
         std::cerr << "Fatal error: " << ex.what() << "\n";
         return 1;
      }
   }

   return 0;
}
