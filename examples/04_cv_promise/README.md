# Example 4: CVPromise for Async Notification

Demonstrates `CVPromise` - a condition-variable-style primitive for signaling between coroutines.

## What You'll Learn

- Creating and using `CVPromise` for one-time or repeating signals
- `Notify()` - signal all current waiters (one-shot)
- `Reset()` - signal current waiters and arm the next wait cycle
- Multiple coroutines waiting on the same signal
- Exception handling with `CVPromise::End`
- Thread-safe cross-thread signaling

## Key Concepts

- `CVPromise` is like a coroutine-friendly condition variable
- `co_await *ready` waits for a `Notify()` or `Reset()`
- `Notify()` is one-shot - it signals all current waiters, new waiters will wait
- `Reset()` signals current waiters AND prepares the next wait cycle
- Destroying the `CVPromise` throws `CVPromise::End` to all waiters
- Useful for producer/consumer, work-ready signals, and coordination

## Run

```bash
cd /home/sleeper/workspace/tt
cmake --build build --target example_04_cv_promise
./build/examples/04_cv_promise/example_04_cv_promise
```

## Expected Output

```
=== CVPromise Notify Example ===
Worker: Waiting for signal...
Signaler: Sending notification
Worker: Received signal!
Worker started: 1, Notifier signaled: 1

=== CVPromise Reset Example ===
Cycle 0: Waiting...
Notifying cycle 0
Cycle 0: Notified!
Cycle 1: Waiting...
Notifying cycle 1
Cycle 1: Notified!
Cycle 2: Waiting...
Notifying cycle 2
Cycle 2: Notified!
Total notifications received: 3

=== Multiple Waiters Example ===
Waiter 1: Waiting
Waiter 2: Waiting
Waiter 3: Waiting
Broadcasting signal to all waiters
Waiter 1: Received signal
Waiter 2: Received signal
Waiter 3: Received signal
Waiters that received signal: 3
```
