#include <chrono>
#include <iostream>
#include <promise/StatePromise.h>
#include <thread>

/// Example 5: StatePromise Ready/Done Synchronization
/// Shows how to use WaitReady, Wait, WaitDone and reject-style terminal waits.

namespace {
void
WaitUntilDone(WPromise<void> const& promise) {
   while (!promise.Done()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
   }
}
}  // namespace

WPromise<void>
WaitReadyAndWaitDoneExample() {
   return [] -> Promise<void> {
      std::cout << "=== WaitReady + WaitDone (resolve path) ===\n";

      StatePromise state;

      WPromise<void> worker{[&] -> Promise<void> {
         std::cout << "Worker: Waiting for Ready\n";
         co_await state.WaitReady();
         std::cout << "Worker: Ready received\n";

         std::cout << "Worker: Waiting for Done via WaitDone()\n";
         co_await state.WaitDone();
         std::cout << "Worker: Done received (resolved)\n";
         co_return;
      }};

      std::jthread coordinator([&] {
         std::this_thread::sleep_for(std::chrono::milliseconds(50));
         std::cout << "Coordinator: Ready()\n";
         state.Ready();

         std::this_thread::sleep_for(std::chrono::milliseconds(60));
         std::cout << "Coordinator: Done()\n";
         state.Done();
      });

      WaitUntilDone(worker);
      std::cout << "\n";
      co_return;
   };
}

WPromise<void>
WaitEitherExample() {
   return [] -> Promise<void> {
      std::cout << "=== Wait (ready OR done) ===\n";

      StatePromise state;

      WPromise<void> waiter{[&] -> Promise<void> {
         std::cout << "Waiter: Waiting with Wait()\n";
         co_await state.Wait();
         std::cout << "Waiter: Wait() completed\n";

         std::cout << "Waiter: Waiting for terminal Done with WaitDone()\n";
         co_await state.WaitDone();
         std::cout << "Waiter: Done received\n";
         co_return;
      }};

      std::jthread coordinator([&] {
         std::this_thread::sleep_for(std::chrono::milliseconds(50));
         std::cout << "Coordinator: Ready() first\n";
         state.Ready();

         std::this_thread::sleep_for(std::chrono::milliseconds(50));
         std::cout << "Coordinator: Done() second\n";
         state.Done();
      });

      WaitUntilDone(waiter);
      std::cout << "\n";
      co_return;
   };
}

WPromise<void>
RejectTerminalExample() {
   return [] -> Promise<void> {
      std::cout << "=== WaitWithReject + WaitDoneWithReject (exception path) ===\n";

      StatePromise state;

      WPromise<void> waiter{[&] -> Promise<void> {
         try {
            std::cout << "Waiter: Waiting with WaitWithReject()\n";
            co_await state.WaitWithReject();
            std::cout << "Waiter: Ready happened before Done\n";

            std::cout << "Waiter: Waiting with WaitDoneWithReject()\n";
            co_await state.WaitDoneWithReject();
         } catch (StatePromise::End const&) {
            std::cout << "Waiter: Caught StatePromise::End on Done\n";
         }

         co_return;
      }};

      std::jthread coordinator([&] {
         std::this_thread::sleep_for(std::chrono::milliseconds(40));
         std::cout << "Coordinator: Ready()\n";
         state.Ready();

         std::this_thread::sleep_for(std::chrono::milliseconds(40));
         std::cout << "Coordinator: Done()\n";
         state.Done();
      });

      WaitUntilDone(waiter);
      std::cout << "\n";
      co_return;
   };
}

WPromise<void>
ResetForSecondCycleExample() {
   return [] -> Promise<void> {
      std::cout << "=== Reset across two cycles ===\n";

      StatePromise state;

      WPromise<void> worker{[&] -> Promise<void> {
         for (int cycle = 0; cycle < 2; ++cycle) {
            std::cout << "Cycle " << cycle << ": WaitReady()\n";
            co_await state.WaitReady();

            std::cout << "Cycle " << cycle << ": WaitDone()\n";
            co_await state.WaitDone();
            std::cout << "Cycle " << cycle << ": completed\n";

            if (cycle == 0) {
               std::cout << "Cycle 0: Reset()\n";
               state.Reset();
            }
         }

         co_return;
      }};

      std::jthread coordinator([&] {
         for (int cycle = 0; cycle < 2; ++cycle) {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            std::cout << "Coordinator: Ready() for cycle " << cycle << "\n";
            state.Ready();

            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            std::cout << "Coordinator: Done() for cycle " << cycle << "\n";
            state.Done();
         }
      });

      WaitUntilDone(worker);
      std::cout << "\n";
      co_return;
   };
}

int
main() {
   WPromise<void> demo{[]() -> Promise<void> {
      co_await WaitReadyAndWaitDoneExample();
      co_await WaitEitherExample();
      co_await RejectTerminalExample();
      co_await ResetForSecondCycleExample();
      co_return;
   }};

   WaitUntilDone(demo);

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
