# Example 6: Thread Pool and MessageQueue

Demonstrates `promise::Pool<SIZE>` for parallel task dispatch and `promise::MessageQueue` for sequential single-thread execution.

## What You'll Learn

- Creating and using `promise::Pool<SIZE>` for parallel work distribution
- Dispatching synchronous and coroutine work to pools
- `promise::MessageQueue` for single-threaded sequential execution
- `Dispatch()` - non-blocking task submission
- `Ensure()` - guaranteed execution on a specific thread
- Delayed dispatch with duration parameters
- Chaining pool work with promise chains

## Key Concepts

### Pool<SIZE>
- Creates SIZE worker threads
- `Dispatch(callable)` returns a `WPromise<T>` or `WPromise<void>`
- Tasks run in parallel across available threads
- Can dispatch lambdas, function pointers, or coroutines
- Optional duration parameter for delayed execution

### MessageQueue
- Single worker thread processes tasks sequentially
- `Ensure(callable)` enqueues work for that thread
- Useful for UI threads or serialized access
- `ThreadId()` returns the worker thread's ID
- Tasks execute in FIFO order

## Run

```bash
cd /home/sleeper/workspace/tt
cmake --build build --target example_06_thread_pool
./build/examples/06_thread_pool/example_06_thread_pool
```

## Expected Output

```
=== Thread Pool Example ===
Task 1 running on thread pool
Task 2 running on thread pool
Task 3 running on thread pool
Task 1 result: 42
Task 2 result: result
Task 3: completed

=== Pool with Multiple Tasks ===
Task 0 starting
Task 1 starting
Task 2 starting
...
Task 5 done
All pool tasks completed

=== MessageQueue Example ===
Task 1 on queue thread
Task 2 on queue thread (after Task 1)
Task 3 on queue thread (after Task 2)
Results: 10, 20, 30
Queue thread ID: <thread_id>

=== Pool with Delayed Dispatch ===
Dispatched delayed task
...
This executes after 100ms
Delayed task result: delayed_result

=== Chained Pool Work ===
Step 1: Processing input
Step 2: Transforming 100
Step 3: Finalizing 200
Final result: 223
```
