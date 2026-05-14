# Example 5: StatePromise for Ready/Done Workflows

Demonstrates `StatePromise` - a state-machine style primitive for coordinating coroutines through Ready/Done states.

## What You'll Learn

- Creating and using `StatePromise` for state-machine coordination
- `WaitReady()` - wait until Ready is signaled
- `WaitDone()` - wait until Done is signaled
- `Wait()` - wait for either Ready or Done
- `IsDone()` - check if Done state has been reached
- `Reset()` - prepare for another cycle
- Coordinating multiple workers with single state transitions

## Key Concepts

- `StatePromise` combines two `CVPromise` signals: Ready and Done
- Perfect for producer/consumer and worker coordination patterns
- `Ready()` is non-terminal; `Done()` is terminal and final
- `Reset()` prepares the state for another Ready/Done cycle
- Destroying `StatePromise` implicitly calls `Done()`
- Useful in thread pools, work distribution, and lifecycle management

## Run

```bash
cd /home/sleeper/workspace/tt
cmake --build build --target example_05_state_promise
./build/examples/05_state_promise/example_05_state_promise
```

## Expected Output

```
=== Basic StatePromise Example ===
Worker: Waiting for Ready...
Coordinator: Sending Ready
Worker: Ready signal received, starting work
Worker: Waiting for Done...
Coordinator: Sending Done
Worker: Done signal received, exiting
Is Done? 1

=== Wait Any State Example ===
Waiter: Waiting for Ready or Done
Coordinator: Signaling Ready
Waiter: Received either Ready or Done
Waiter: Now wait specifically for Done
Coordinator: Signaling Done
Waiter: Done was signaled

=== Multiple Workers Example ===
...
Coordinator: Signaling Ready to all
Worker 1: Starting
Worker 2: Starting
Worker 3: Starting
...
Coordinator: Signaling Done to all
Worker 1: Finished
Worker 2: Finished
Worker 3: Finished
Workers started: 3, Finished: 3
```
