
<p align="center">
	<img src="https://github.com/alx-home/promise/raw/master/.github/logo.svg" alt="alx-home promise logo" width="560">
</p>

# alx-home/promise Examples

Comprehensive C++23 project demonstrating all features of the [alx-home/promise](https://github.com/alx-home/promise) library through runnable examples.

## Repository Structure

```
.
├── CMakeLists.txt               # Main build configuration
├── README.md                    # This file
├── src/
│   └── main.cpp                 # Quick-start demo
└── examples/
    ├── 01_basic_chain/          # Then/Catch/Finally patterns
    ├── 02_resolver_style/       # Resolver-style promises & promise::Create<T>()
    ├── 03_all_and_race/         # Combining promises with All/Race
    ├── 04_cv_promise/           # CVPromise for async notification
    ├── 05_state_promise/        # StatePromise for state machines
    └── 06_thread_pool/          # Pool<> and MessageQueue for threading
```

## What is alx-home/promise?

A modern C++23 promise library inspired by JavaScript promises, with:
- **JS-style API**: `Then`, `Catch`, `Finally`, chaining
- **Native coroutine support**: `Promise<T>` return types, seamless `co_await`
- **Full thread safety**: Resolve/reject/await from any thread
- **Resolver model**: Manual control with `Resolve<T>` and `Reject`
- **Combinators**: `All`, `Race` for composing promises
- **Async primitives**: `CVPromise`, `StatePromise` for coordination
- **Threaded dispatch**: `Pool<SIZE>`, `MessageQueue` for worker threads

## Quick Start

### Build all examples:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Run the main demo:

```bash
./build/promise_demo
```

## Examples

### 1. Basic Chaining (`01_basic_chain`)

Learn fundamental `Then`/`Catch`/`Finally` patterns.

```bash
./build/examples/01_basic_chain/example_01_basic_chain
```

**Topics**: Promise chaining, type flow, error handling, cleanup

---

### 2. Resolver Style (`02_resolver_style`)

Manual promise resolution with `Resolve<T>` and `promise::Create<T>()`.

```bash
./build/examples/02_resolver_style/example_02_resolver_style
```

**Topics**: Resolver-enabled promises, background tasks, conditional resolution

---

### 3. All and Race (`03_all_and_race`)

Combining multiple promises into single operations.

```bash
./build/examples/03_all_and_race/example_03_all_and_race
```

**Topics**: `promise::All()`, `promise::Race()`, tuples, variants, result types

---

### 4. CVPromise (`04_cv_promise`)

Condition-variable-style signaling between coroutines.

```bash
./build/examples/04_cv_promise/example_04_cv_promise
```

**Topics**: Async notification, `Notify()`, `Reset()`, multi-waiter broadcasts

---

### 5. StatePromise (`05_state_promise`)

Ready/Done state-machine coordination.

```bash
./build/examples/05_state_promise/example_05_state_promise
```

**Topics**: State machines, Ready/Done cycles, worker coordination, reset

---

### 6. Thread Pool (`06_thread_pool`)

Parallel and sequential task dispatch.

```bash
./build/examples/06_thread_pool/example_06_thread_pool
```

**Topics**: `Pool<SIZE>`, `MessageQueue`, `Dispatch()`, `Ensure()`, delayed execution

---

## Key Features Demonstrated

| Feature | Example |
|---------|---------|
| `Then` / `Catch` / `Finally` | 01, 02, 03, 06 |
| Type flow & variants | 03 |
| `Resolve<T>` / `Reject` | 02 |
| `promise::Create<T>()` | 02, 03 |
| `promise::All()` | 03, 06 |
| `promise::Race()` | 03 |
| `CVPromise` | 04 |
| `StatePromise` | 05 |
| `Pool<SIZE>` | 06 |
| `MessageQueue` | 06 |
| Thread-safe async | All |
| Error handling | 01, 02, 03, 06 |

## Build Options

All examples compile with C++23. Customize with CMake options:

```bash
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_COMPILER=g++-14
```

## Requirements

- C++23 compiler (GCC 14+, Clang 17+)
- CMake 3.20+
- Git (for dependency fetching)

## Next Steps

1. **Run the main demo** to see a quick overview
2. **Explore examples in order** (01 → 06) for progressive complexity
3. **Read the README in each example folder** for detailed explanations
4. **Examine the source code** — comments explain key concepts
5. **Refer to the official README**: https://github.com/alx-home/promise/blob/master/README.md

## License

This example repository demonstrates [alx-home/promise](https://github.com/alx-home/promise) under its original license.
