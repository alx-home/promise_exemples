# Example 5: StatePromise for Ready/Done Workflows

Demonstrates `StatePromise` - a state-machine style primitive for coordinating coroutines through Ready/Done states.

## What You'll Learn

- Creating and using `StatePromise` for state-machine coordination
- `WaitReady()` - wait until Ready is signaled
- `Wait()` - wait until either Ready or Done is signaled
- `WaitDone()` - wait until Done is signaled (resolve path)
- `WaitWithReject()` / `WaitDoneWithReject()` - reject with `StatePromise::End` on Done
- `Reset()` - prepare for another cycle

## Key Concepts

- `StatePromise` combines two signals: Ready and Done
- `Ready()` is non-terminal; `Done()` is terminal for the current cycle
- `WaitDone()` resolves on Done
- `WaitDoneWithReject()` throws `StatePromise::End` on Done
- `Wait()` resolves on whichever happens first: Ready or Done
- `Reset()` prepares the state for reuse across multiple cycles

## Run

```bash
cmake --build build --target example_05_state_promise
./build/example_05_state_promise
```

On Windows (PowerShell), run:

```powershell
cmake --build build --target example_05_state_promise
.\build\example_05_state_promise.exe
```

## Expected Output

```text
=== WaitReady + WaitDone (resolve path) ===
Worker: Waiting for Ready
Coordinator: Ready()
Worker: Ready received
Worker: Waiting for Done via WaitDone()
Coordinator: Done()
Worker: Done received (resolved)

=== Wait (ready OR done) ===
Waiter: Waiting with Wait()
Coordinator: Ready() first
Waiter: Wait() completed
Waiter: Waiting for terminal Done with WaitDone()
Coordinator: Done() second
Waiter: Done received

=== WaitWithReject + WaitDoneWithReject (exception path) ===
Waiter: Waiting with WaitWithReject()
Coordinator: Ready()
Waiter: Ready happened before Done
Waiter: Waiting with WaitDoneWithReject()
Coordinator: Done()
Waiter: Caught StatePromise::End on Done

=== Reset across two cycles ===
Cycle 0: WaitReady()
Coordinator: Ready() for cycle 0
Cycle 0: WaitDone()
Coordinator: Done() for cycle 0
Cycle 0: completed
Cycle 0: Reset()
Cycle 1: WaitReady()
Coordinator: Ready() for cycle 1
Cycle 1: WaitDone()
Coordinator: Done() for cycle 1
Cycle 1: completed
```
