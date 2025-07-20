# Reaver Language Documentation

Reaver is a simple, Turing-complete programming language designed for educational purposes. It features a minimal set of commands, focusing on core programming constructs.

## Getting Started

To get started with the Reaver compiler, you'll need `g++` (a C++ compiler) and `nasm` (the Netwide Assembler) installed on your system.

### Compiling the Reaver Compiler

First, compile the `reaver` compiler itself from the source code:

```bash
g++ -std=c++17 main.cpp src/*.cpp -o reaver
```

This command compiles all the C++ source files and links them into a single executable named `reaver`.

## Language Features

Reaver supports the following fundamental programming constructs:

*   **Variables**: Integer variables can be declared and assigned values using the `=` operator. Variables are implicitly declared upon their first assignment. Reaver currently only supports integer values.
    ```reaver
    my_variable = 10
    another_var = my_variable + 5
    ```

*   **Arithmetic Operations**: Basic arithmetic operations are supported:
    *   Addition (`+`)
    *   Subtraction (`-`)
    *   Multiplication (`*`)
    *   Division (`/`)
    
    Parentheses `()` can be used to control the order of operations.
    ```reaver
    result = (10 + 5) * 2
    ```

*   **Conditional Statements (`if`, `else`)**: Execute blocks of code based on a condition. Only the less than (`<`) operator is supported for comparisons. Conditions must evaluate to an integer (0 for false, non-zero for true).
    ```reaver
    my_number = 10
    if my_number < 20 {
        rp "my_number is less than 20"
    } else {
        rp "my_number is not less than 20"
    }
    ```

*   **Loops (`while`)**: Repeatedly execute a block of code as long as a condition is true. Only the less than (`<`) operator is supported for comparisons. Conditions must evaluate to an integer (0 for false, non-zero for true).
    ```reaver
    i = 0
    while i < 5 {
        rp i
        i = i + 1
    }
    ```

*   **Printing (`rp`)**: Output integer values or string literals to the console.
    ```reaver
    rp 123
    rp "Hello, Reaver!"
    my_var = 42
    rp my_var
    ```

*   **Comments**: Lines starting with `#` are treated as comments and are ignored by the compiler.
    ```reaver
    # This is a single-line comment
    my_code = 1
    ```

## Compiler Usage

The Reaver compiler (`reaver`) translates `.rv` source files into native executables. The process involves generating an intermediate assembly file (`.asm`), assembling it into an object file (`.o`), and then linking it with a C runtime library to produce the final executable. These intermediate files are automatically cleaned up.

### Compilation

To compile a Reaver program, use the following syntax:

```bash
./reaver <input_file.rv> [-o <output_executable_name>]
```

*   `<input_file.rv>`: The path to your Reaver source code file.
*   `-o <output_executable_name>` (optional): Specifies the name of the generated executable. If omitted, the default output name is `output`.

**Examples:**

```bash
# Compile 'my_program.rv' to an executable named 'output'
./reaver examples/my_program.rv

# Compile 'hello.rv' to an executable named 'hello_world'
./reaver examples/hello.rv -o hello_world
```

### Running Compiled Programs

After successful compilation, you can run the generated executable directly:

```bash
./<output_executable_name>
```

**Example:**

```bash
./hello_world
```

## Error Handling

The Reaver compiler performs basic semantic analysis. If it detects issues like the use of undeclared variables, it will report a semantic error and halt compilation.

**Example (semantic_error.rv):**

```reaver
rp x  # 'x' is used before being assigned a value
```

Compiling this file would result in an error message:

```
Semantic Error: Use of undeclared variable 'x'
Compilation failed due to semantic errors.
```

## Pros and Cons of Reaver

### Pros

*   **Simplicity**: The language design is minimal, making it easy to learn and understand the core concepts of programming and compilation.
*   **Turing Complete**: Despite its simplicity, Reaver can theoretically perform any computation that a more complex language can.
*   **Educational Value**: Ideal for learning about compiler design, lexing, parsing, semantic analysis, and code generation to assembly.
*   **Native Executables**: Compiles directly to machine code, resulting in fast execution.

### Cons

*   **Limited Features**: Lacks many features common in modern languages (e.g., floating-point numbers, arrays, functions, user input, complex data structures, more comparison operators).
*   **No Standard Library**: Relies on a very minimal custom runtime for basic operations like printing.
*   **Error Reporting**: While basic semantic errors are caught, error messages could be more detailed and user-friendly.
*   **Development Speed**: Writing complex programs in Reaver would be very slow due to its limited expressiveness.

## Development

The Reaver compiler is structured into several distinct phases:

*   **Lexer**: Converts the source code into a stream of tokens.
*   **Parser**: Builds an Abstract Syntax Tree (AST) from the token stream.
*   **Semantic Analyzer**: Traverses the AST to check for semantic errors (e.g., use of undeclared variables).
*   **Code Generator**: Translates the AST into x86-64 assembly code.

The compiler extensively uses the **Visitor pattern** for AST traversal, allowing different analysis and generation phases to operate on the same AST structure.

## Future Improvements

Potential enhancements for the Reaver language and compiler include:

*   **Expanded Comparison Operators**: Add support for `==`, `!=`, `>`, `<=`, and `>=`.
*   **User Input**: Implement a mechanism for programs to read input from the user.
*   **Functions**: Introduce support for user-defined functions.
*   **More Data Types**: Add support for floating-point numbers, booleans, or arrays.
*   **Improved Error Messages**: Provide more context and suggestions for compiler errors.
*   **Standard Library**: Develop a more comprehensive set of built-in functions.

## Use Cases

For a detailed discussion on the intended use cases and limitations of the Reaver language, refer to the [Use Cases document](use_cases.md).

## Examples

You can find example Reaver programs in the `examples/` directory.

*   `examples/hello.rv`: Basic string and integer printing.
*   `examples/loop.rv`: Demonstrates `while` loops.
*   `examples/math.rv`: Shows arithmetic operations and operator precedence.
*   `examples/parens.rv`: Tests expressions with parentheses.
*   `examples/semantic_error.rv`: An example of code that triggers a semantic error.
*   `examples/variables.rv`: Demonstrates variable declaration, assignment, and usage.
*   `examples/calculator.rv`: A simple calculator-like example demonstrating arithmetic and conditional logic.
*   `examples/countdown.rv`: A program that counts down from a given number.
*   `examples/max_of_two.rv`: Finds the maximum of two numbers using conditionals.
*   `examples/sum_n.rv`: Calculates the sum of numbers from 1 to N using a loop.
*   `examples/logic_gates.rv`: Demonstrates how to simulate basic logic gates.
*   `examples/factorial.rv`: Calculates the factorial of a number.
*   `examples/even_odd.rv`: Determines if a number is even or odd.
