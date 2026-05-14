# Example 1: Basic Promise Chaining

Demonstrates the fundamental promise patterns: **Then**, **Catch**, and **Finally** - similar to JavaScript promises.

## What You'll Learn

- Creating promises with lambda coroutines
- Chaining operations with `.Then()`
- Error handling with `.Catch()`
- Cleanup with `.Finally()`
- Mixing synchronous and asynchronous operations in a chain

## Key Concepts

- `Promise<T>` is the coroutine return type
- `WPromise<T>` is the owning handle you create with a lambda
- Handlers receive const references to values (e.g., `const int&`)
- `.Then()` handlers can return raw values or `Promise<T>`
- `.Catch()` receives `std::exception_ptr` and can recover the chain

## Run

```bash
cd /home/sleeper/workspace/tt
cmake --build build --target example_01_basic_chain
./build/examples/01_basic_chain/example_01_basic_chain
```

## Expected Output

```
=== Basic Chain Example ===
Step 1: Got value 10
Step 2: Doubled to 20
Step 3: Added 5, result = 25
Finally: Cleanup complete
Final result: 25

=== Chain with Functions ===
User age: 25

=== Error Handling Example ===
Caught exception in Then/Catch chain
Result after error: -1
```
