# Reaver Compiler Architecture

This document outlines the architecture of the Reaver compiler, from lexical analysis to code generation.

## 1. Frontend

The frontend is responsible for transforming Reaver source code into an Intermediate Representation (IR).

### Token Classes

The lexer recognizes the following token classes:

- **Keywords**: `if`, `else`, `while`, `return`, `int`
- **Identifiers**: `[a-zA-Z_][a-zA-Z0-9_]*`
- **Integer Literals**: `[0-9]+`
- **Operators**: `+`, `-`, `*`, `/`, `=`, `==`, `!=`, `<`, `>`, `<=`, `>=`
- **Punctuation**: `(`, `)`, `{`, `}`, `;`, `,`
- **EOF**: End-of-file marker

### Grammar (EBNF)

The grammar will be formally defined, but a simplified EBNF is:

```ebnf
program     ::= function_declaration*

function_declaration ::= type IDENTIFIER "(" parameter_list? ")" block

block       ::= "{" statement* "}"

statement   ::= expression_statement
            |   if_statement
            |   while_statement
            |   return_statement

expression  ::= IDENTIFIER "=" expression | comparison

comparison  ::= addition ( ( "==" | "!=" ) addition )*

addition    ::= multiplication ( ( "+" | "-" ) multiplication )*

multiplication ::= primary ( ( "*" | "/" ) primary )*

primary     ::= INTEGER | IDENTIFIER | "(" expression ")"
```

### Parser Strategy

We will use a **Recursive-Descent Parser**. It's straightforward to implement by hand and maps directly to the grammar structure. This approach is efficient for our language's scale and avoids the complexity of parser generators or more advanced techniques like Pratt parsing for now.

## 2. Middle-End

The middle-end performs optimizations on the IR.

### Intermediate Representation (IR)

We will use the **Abstract Syntax Tree (AST)** directly as our primary IR. For a language of this scale, a separate, lower-level IR like Three-Address Code (TAC) is overkill. Code generation will proceed directly from the AST, simplifying the compilation pipeline.

## 3. Backend

The backend generates x86-64 assembly code from the AST.

### Target ABI

- **Architecture**: x86-64
- **Operating System**: Linux
- **ABI**: System V AMD64

### Calling Convention

We adhere to the System V ABI calling convention:
- The first six integer/pointer arguments are passed in registers `RDI`, `RSI`, `RDX`, `RCX`, `R8`, `R9`.
- Subsequent arguments are passed on the stack.
- The return value is placed in `RAX`.
- `RBP` is the base pointer, and `RSP` is the stack pointer.
- `RBX`, `R12`-`R15` are callee-saved.

### Stack Frame Layout

A typical stack frame will be organized as follows:

```
+------------------------+ <-- Higher addresses
|   Return Address       |
+------------------------+ <-- RBP points here after setup
|   Old RBP              |
+------------------------+
|   Local Variables      |
|   ...                  |
+------------------------+ <-- RSP points here
|   ...                  |
+------------------------+ <-- Lower addresses
```

### Register Usage

- `RAX`: Return values, intermediate expression results.
- `RBP`: Stack frame base pointer.
- `RSP`: Stack pointer.
- `RDI`, `RSI`, `RDX`, `RCX`, `R8`, `R9`: Function arguments.
- `R10`, `R11`: Scratch registers for temporary computations.

### String & Print Handling

Strings are not a first-class type, but string literals can be used with the print mechanism.

- Language builtin: `rp(expr);`
  - Accepts integer expressions and string literals.
  - No string variables; only literals are allowed.
- Codegen maps `rp(expr)` to calls into runtime helpers: `print_integer` (for integers) and `print_string` (for string literals), linked from a small C runtime. These helpers perform the actual I/O (via the C standard library or syscalls).

## 4. Limitations

To maintain focus, the compiler has the following limitations:

- **Platform**: Only supports 64-bit Linux.
- **Data Types**: Integer-only (`i64`). No floating-point numbers, structs, or complex types.
- **Scope**: Only global functions are supported. No nested scopes or closures.
- **Standard Library**: Minimal, limited to a `print` function for integers.
