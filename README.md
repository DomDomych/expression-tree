# Expression Tree

A small C++17 expression parser and evaluator.

It builds an expression tree from infix or postfix notation and supports
evaluation, variables, multiple output forms and constant folding.

## Features

- infix and postfix expressions
- infix to postfix conversion
- expression evaluation
- variables
- prefix, infix and postfix output
- constant folding
- custom AVL-based map for variable storage

## Implementation

The expression tree is built around polymorphic node types and uses
`std::unique_ptr` for node ownership.

Variables are stored in a custom templated AVL map.

Supported operators:

`+ - * / ^`

## Build

```bash
cmake -S . -B build
cmake --build build
./build/FifthLab