# Example 2: Resolver-Style Promises

Demonstrates manual resolve/reject patterns using `Promise<T, true>` and `promise::Create<T>()`.

## What You'll Learn

- `Promise<T, true>` - resolver-enabled coroutine promises
- `Resolve<T>` and `Reject` handles
- `promise::Create<T>()` - create promise with separate resolver/rejecter handles
- Conditional resolution based on application logic
- Integrating promises with thread-based async work

## Key Concepts

- Resolver-style is useful when you cannot express async work as a simple coroutine
- `Resolve<T>()` and `reject()` can be called later, from callbacks or other threads
- `promise::Create<T>()` returns a tuple: `[promise_handle, resolve_ptr, reject_ptr]`
- Both `resolve` and `reject` are idempotent - calling twice ignores the second call
- Resolvers keep the promise state alive as long as they hold shared_ptr references

## Run

```bash
cd /home/sleeper/workspace/tt
cmake --build build --target example_02_resolver_style
./build/examples/02_resolver_style/example_02_resolver_style
```

## Expected Output

```
=== Resolver Style Example ===
Direct resolve result: 7

=== promise::Create<T>() Example ===
Result: Hello from background!

=== Conditional Resolve/Reject Example ===
Success: 99
Caught failure in chain
Failure result: -1
```
