# Example 3: Promise All and Race

Demonstrates combining multiple promises with `promise::All()` and `promise::Race()`.

## What You'll Learn

- `promise::All()` - wait for all promises to complete and collect results in a tuple
- `promise::Race()` - complete as soon as the first promise resolves or rejects
- Type inference with heterogeneous promise types
- Handling void returns in All/Race compositions
- Result types as tuples and variants

## Key Concepts

### All
- Collects all results in a `std::tuple`
- Void-returning promises are awaited but omitted from the tuple
- If any promise rejects, All rejects
- Order is preserved

### Race
- Returns a `std::variant` if types differ
- Returns the raw type if all promises return the same non-void type
- Returns `std::optional<T>` if any promise returns void
- Completes immediately when first promise resolves or rejects

## Run

```bash
cd /home/sleeper/workspace/tt
cmake --build build --target example_03_all_and_race
./build/examples/03_all_and_race/example_03_all_and_race
```

## Expected Output

```
=== promise::All Example ===
Result 1 (int): 10
Result 2 (double): 3.14
Result 3 (string): complete

=== promise::All with Void ===
Result 1: 10, Result 2: 3.14
Void task was awaited but not included in tuple

=== promise::Race Example ===
First to complete: 1

=== promise::Race with Different Types ===
Winner: int = 42

=== promise::Race with Error ===
Caught error from race
Race result: -1
```
