# Documentation for Light

## Introduction

Light is a simple, beginner-friendly programming language designed for beginners. This document provides an overview of the language’s syntax, features, usage, key concepts, and examples.

## Syntax and Grammar

Light uses a syntax that is simple and intuitive, making it easy for beginners to learn. The basic syntax rules include:

- **Identifiers**: Names for variables, functions, etc., which must start with a letter and can contain letters, digits, and underscores.
- **Keywords**: Reserved words that have special meaning in Light. Examples include `var` and `function`.

## Data Types

Light supports a few basic data types:

- **Integer**
- **Float**
- **String**

### Example

```light
var x = 10;
var y = 3.14;
var name = "Alice";
```

## Variables

Variables are used to store data. In Light, variables are declared using the following syntax:

```light
var variable_name = value;
```

### Example

```light
var age = 25;
var greeting = "Hello";
```

## Expressions and Operators

Light supports arithmetic operations, including:

- **Arithmetic Operators**: `+`, `-`, `*`, `/`

### Example

```light
var sum = 5 + 3;
print(sum); // Output: 8.000000
```

## Statements

Statements are instructions that perform actions. The primary types of statements in Light include:

- **Assignment**: Assigns a value to a variable.

### Example

```light
var x = 12;
```

## Functions

Functions are blocks of code that perform specific tasks. They are defined using the following syntax:

```light
function function_name(parameters) {
    // function body
}
```

### Example

```light
function sum(a, b) {
    print(a + b);
}

sum(1, 2); // Output: 3.000000
```

## Scope and Variable Management

Light manages variable scope using:

- **Global Scope**: Variables accessible throughout the program.
- **Local Scope**: Variables accessible only within a function or block.

### Example

```light
var globalVar = 10;

function example() {
    var localVar = 5;
    print(globalVar);
}

example(); // Output: 10.000000
```

## Examples of Programs in Light

### Example 1: Basic Operations

```light
var x = 5;
var y = 10;
var z = x + y;
var str = "Hello World";
print(z);  // Output: 15.000000
print(str); // Output: Hello World
```

### Example 2: Function Definition

```light
function greet(name) {
    print("Hello, " + name);
}

greet("Alice"); // Output: Hello, Alice
```

This documentation provides a comprehensive overview of Light, covering its syntax, features, and usage. For more detailed information, send me an email at rayenmiri000@gmail.com.
