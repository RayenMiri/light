Certainly! Let's add a section on `while` loops to the documentation:

---

# Documentation for Light

## Introduction

Light is a simple, beginner-friendly programming language designed for beginners. This document provides an overview of the language’s syntax, features, usage, key concepts, and examples.

## Syntax and Grammar

Light uses a syntax that is simple and intuitive, making it easy for beginners to learn. The basic syntax rules include:

- **Identifiers**: Names for variables, functions, etc., which must start with a letter and can contain letters, digits, and underscores.
- **Keywords**: Reserved words that have special meaning in Light. Examples include `var`, `function`, and `if`.

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

### Local vs Global Variables

Variables in Light can be classified into **local** and **global** based on their scope and accessibility.

### Global Variables

- **Definition**: Global variables are declared outside of any function or block and are accessible throughout the entire program.
- **Scope**: Available everywhere in the program, including inside functions or blocks.

### Example of Global Variable

```light
var globalVar = 100;

function showGlobal() {
    print(globalVar);
}

showGlobal(); // Output: 100.000000
```

### Local Variables

- **Definition**: Local variables are declared inside a function or block and are only accessible within that function or block.
- **Scope**: Limited to the function or block where they are declared.

### Example of Local Variable

```light
function localExample() {
    var localVar = 50;
    print(localVar);
}

localExample(); // Output: 50.000000
// print(localVar); // Error: Undefined variable ‘localVar’ 
```

### Important Points

- **Global variables**: Can be used by any function or block within the program.
- **Local variables**: Can only be used within the function or block where they are declared and are not accessible outside of it.

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
- **Conditional (If)**: Executes code based on a condition.

### Example of Assignment

```light
var x = 12;
```

## Conditional Statements (If)

Light supports `if` statements to allow conditional execution of code. The syntax for an `if` statement is as follows:

```light
if (condition) {
    // code to be executed if condition is true
}
```

### Example of If Statement

```light
var score = 72;

if (score >= 90) {
 print("Grade: A");
} elsif (score >= 80) {
 print("Grade: B");
} elsif (score >= 70) {
 print("Grade: C");
} elsif (score >= 60) {
 print("Grade: D");
} else {
 print("Grade: F");
}
```

### Explanation:

- The `if` statement checks the condition inside the parentheses.
- If the condition evaluates to `true`, the code inside the `{}` block following the `if` is executed.
- If there is an `else` block, it will execute if the condition evaluates to `false`.

## Loops

Loops are used to repeatedly execute a block of code as long as a specified condition is true. Light supports `while` loops.

### While Loop

The `while` loop repeatedly executes a block of code as long as the condition specified in the `while` statement is `true`.

### Syntax

```light
while (condition) {
    // code to be executed repeatedly
}
```

### Example of While Loop

```light
var count = 0;

while (count < 5) {
    print(count);
    count = count + 1;
}
```

### Explanation:

- The loop starts by checking the condition `(count < 5)`.
- If the condition is `true`, the code inside the `{}` block is executed.
- After each iteration, the condition is checked again.
- The loop continues until the condition becomes `false`.

### Output:

```
0.000000
1.000000
2.000000
3.000000
4.000000
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
    print(localVar);
}

example(); // Output: 10.000000, 5.000000
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
    print("Hello, " , name);
}

greet("Alice"); // Output: Hello, Alice
```

### Example 3: Conditional Statement

```light
var temperature = 30;

if (temperature > 25) {
    print("It's hot outside!");
} else {
    print("It's cool outside!");
}
```

### Example 4: Local vs Global Variables

```light
var globalVar = 10;

function testScope() {
    var localVar = 5;
    print(globalVar); // Output: 10.000000
    print(localVar);  // Output: 5.000000
}

testScope();

print(globalVar); // Output: 10.000000
// print(localVar); // Error: Undefined variable 'localVar'
```

### Example 5: While Loop

```light
var i = 1;

while (i <= 3) {
    print("Loop iteration: " , i);
    i = i + 1;
}
```

### Explanation:

- The loop will run as long as `i <= 3` is `true`.
- The value of `i` is incremented by 1 in each iteration, and the loop will stop when `i` becomes greater than 3.

### Output:

```
Loop iteration: 1.000000
Loop iteration: 2.000000
Loop iteration: 3.000000
```

This documentation provides a comprehensive overview of Light, covering its syntax, features, and usage. For more detailed information, send me an email at rayenmiri000@gmail.com.
