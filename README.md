# Reaver: A Simple, Educational Programming Language

## Performance & Size Micro-benchmarks

This table shows preliminary benchmarks for the Reaver compiler (`reaverc`) compared to `clang`.

| Benchmark         | Reaver Compile Time (ms) | Reaver Binary Size (bytes) | Clang Compile Time (ms) | Clang Binary Size (bytes) |
|-------------------|--------------------------|----------------------------|-------------------------|---------------------------|
| **Hello World**   | 354                      | 16024                      | 68                      | 15416                     |
| **Simple Loop**   | 107                      | 16072                      | 84                      | 15464                     |
| **Arithmetic**    | 182                      | 16048                      | 139                     | 15464                     |

*Benchmarks are illustrative and will be updated as the compiler matures.*

Reaver is a minimalist, Turing-complete programming language designed to make learning about compiler design accessible and engaging. It provides a hands-on experience with fundamental programming constructs and the process of translating high-level code into native executables.

## Key Features

*   **Variables**: Integer variable declaration and assignment.
*   **Arithmetic Operations**: Basic addition, subtraction, multiplication, and division.
*   **Conditional Logic**: `if-else` statements for decision-making.
*   **Looping**: `while` loops for repetitive tasks.
*   **Output**: Print integer values and string literals to the console.
*   **Comments**: Single-line comments for code annotation.
*   **Native Compilation**: Compiles directly to x86-64 assembly and links into standalone executables.

## Quick Start

To get started with Reaver, follow these simple steps:

1.  **Compile the Reaver Compiler** (requires `g++` and `nasm`):
    ```bash
    g++ -std=c++17 main.cpp src/*.cpp -o reaver
    ```

2.  **Compile a Reaver Program** (e.g., `examples/hello.rv`):
    ```bash
    ./reaver examples/hello.rv -o hello_program
    ```

3.  **Run the Compiled Program**:
    ```bash
    ./hello_program
    ```

## Why Reaver?

Reaver serves as an excellent educational tool for:

*   Understanding the core concepts of programming languages.
*   Exploring the stages of a compiler (lexing, parsing, semantic analysis, code generation).
*   Gaining insight into how high-level code translates to low-level machine instructions.

## Documentation

For a comprehensive guide on Reaver's language features, compiler usage, error handling, and development insights, please refer to the [full documentation](docs/README.md).

## Examples

Explore various Reaver programs demonstrating its capabilities in the `examples/` directory.