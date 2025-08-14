# Reaver Language Specification

This document provides a formal specification of the Reaver programming language.

## 1. Lexical Structure

Reaver source code is ASCII text. Tokens are the fundamental building blocks.

- **Identifiers**: `[a-zA-Z_][a-zA-Z0-9_]*`. Used for function and variable names.
- **Keywords**: `if`, `else`, `while`, `return`, `int`. Reserved and cannot be used as identifiers.
- **Integer Literals**: A sequence of one or more digits, e.g., `123`. Interpreted as 64-bit signed integers.
- **Operators**: `+`, `-`, `*`, `/`, `=`, `==`, `!=`, `<`, `>`, `<=`, `>=`.
- **Punctuation**: `(`, `)`, `{`, `}`, `;`, `,`.
- **Whitespace**: Spaces, tabs, and newlines are used to separate tokens and are otherwise ignored.
- **Comments**: Not yet supported.

## 2. Grammar

The syntax is defined by the following EBNF grammar:

```ebnf
program     ::= function_declaration+

function_declaration ::= 'int' IDENTIFIER '(' parameter_list? ')' block

parameter_list ::= 'int' IDENTIFIER (',' 'int' IDENTIFIER)*

block       ::= '{' statement* '}'

statement   ::= declaration_statement
            |   expression_statement
            |   if_statement
            |   while_statement
            |   return_statement

declaration_statement ::= 'int' IDENTIFIER ('=' expression)? ';'

expression_statement ::= expression? ';'

if_statement ::= 'if' '(' expression ')' statement ('else' statement)?

while_statement ::= 'while' '(' expression ')' statement

return_statement ::= 'return' expression ';'

expression  ::= assignment

assignment  ::= logical_or ( '=' assignment )?

logical_or  ::= logical_and ( '||' logical_and )*

logical_and ::= equality ( '&&' equality )*

equality    ::= relational ( ( '==' | '!=' ) relational )*

relational  ::= additive ( ( '<' | '>' | '<=' | '>=' ) additive )*

additive    ::= multiplicative ( ( '+' | '-' ) multiplicative )*

multiplicative ::= unary ( ( '*' | '/' ) unary )*

unary       ::= ( '+' | '-' )? primary

primary     ::= INTEGER
            |   IDENTIFIER
            |   '(' expression ')'
```

## 3. Types

Reaver has one fundamental data type:

- `int`: A 64-bit signed integer.

## 4. Operator Precedence & Associativity

Operators are listed from highest to lowest precedence.

| Precedence | Operator | Description        | Associativity |
|------------|----------|--------------------|---------------|
| 1          | `()`     | Function Call      | Left-to-right |
| 2          | `+`, `-` | Unary Plus, Minus  | Right-to-left |
| 3          | `*`, `/` | Multiplication, Division | Left-to-right |
| 4          | `+`, `-` | Addition, Subtraction | Left-to-right |
| 5          | `<`, `>`, `<=`, `>=` | Relational | Left-to-right |
| 6          | `==`, `!=` | Equality | Left-to-right |
| 7          | `&&`     | Logical AND        | Left-to-right |
| 8          | `||`     | Logical OR         | Left-to-right |
| 9          | `=`      | Assignment         | Right-to-left |

## 5. Semantics

- **Execution**: Program execution begins at the `main` function.
- **Scoping**: Lexical scoping. Variables are visible within the block they are declared in.
- **Parameters**: Arguments are passed by value.
- **Control Flow**: `if`/`else` and `while` provide conditional and iterative execution. `return` exits a function.

## 6. Built-ins

- **`rp(expr);`**: Prints the result of `expr` to standard output.
  - Allowed operands: integer expressions and string literals.
  - Strings are not first-class values; only literals are permitted.
  - Behavior: writes the textual representation to stdout (no allocation, implementation provided by runtime helpers).
