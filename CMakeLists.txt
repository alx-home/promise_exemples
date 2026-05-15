cmake_minimum_required(VERSION 3.20)

project(promise_examples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/resolve_alx_promise.cmake)
resolve_alx_promise_dependency()

# Main demo executable
add_executable(promise_demo
    src/main.cpp
)

target_link_libraries(promise_demo
    PRIVATE
        alx-home::promise
)

# Example 1: Basic Chain
add_executable(example_01_basic_chain
    examples/01_basic_chain/main.cpp
)

target_link_libraries(example_01_basic_chain
    PRIVATE
        alx-home::promise
)

# Example 2: Resolver Style
add_executable(example_02_resolver_style
    examples/02_resolver_style/main.cpp
)

target_link_libraries(example_02_resolver_style
    PRIVATE
        alx-home::promise
)

# Example 3: All and Race
add_executable(example_03_all_and_race
    examples/03_all_and_race/main.cpp
)

target_link_libraries(example_03_all_and_race
    PRIVATE
        alx-home::promise
)

# Example 4: CVPromise
add_executable(example_04_cv_promise
    examples/04_cv_promise/main.cpp
)

target_link_libraries(example_04_cv_promise
    PRIVATE
        alx-home::promise
)

# Example 5: StatePromise
add_executable(example_05_state_promise
    examples/05_state_promise/main.cpp
)

target_link_libraries(example_05_state_promise
    PRIVATE
        alx-home::promise
)

# Example 6: Thread Pool
add_executable(example_06_thread_pool
    examples/06_thread_pool/main.cpp
)

target_link_libraries(example_06_thread_pool
    PRIVATE
        alx-home::promise
)
