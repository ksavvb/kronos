# Kronos Language Syntax Reference

Complete syntax guide for the Kronos programming language - a human-readable language with natural language keywords.

## Table of Contents

1. [Variables](#variables)
2. [Data Types](#data-types)
3. [Printing](#printing)
4. [Arithmetic Operations](#arithmetic-operations)
5. [Comparison Operators](#comparison-operators)
6. [Conditional Statements](#conditional-statements)
7. [Loops](#loops)
8. [Comments](#comments)
9. [Indentation](#indentation)

---

## Variables

Variables in Kronos are declared using the `set` keyword followed by the variable name, the word `to`, and the value.

### Syntax
```
set <variable_name> to <value>
```

### Examples
```kronos
set x to 5
set name to "Alice"
set pi to 3.14159
set isActive to true
```

### Rules
- Variable names must start with a letter or underscore
- Can contain letters, numbers, and underscores
- Case sensitive (`myVar` and `myvar` are different)
- No keywords can be used as variable names

---

## Data Types

Kronos supports several built-in data types:

### Numbers
Both integers and floating-point numbers are supported.

```kronos
set age to 25
set temperature to 98.6
set negative to -10
set decimal to 0.5
```

### Strings
Text enclosed in double quotes.

```kronos
set greeting to "Hello, World!"
set empty to ""
set multiword to "This is a sentence"
```

### Booleans
True or false values (future implementation).

```kronos
set isActive to true
set isComplete to false
```

### Nil
Represents no value (future implementation).

```kronos
set nothing to nil
```

---

## Printing

The `print` statement outputs values to the console.

### Syntax
```
print <expression>
```

### Examples

**Print literal values:**
```kronos
print "Hello, World!"
print 42
print 3.14
```

**Print variables:**
```kronos
set name to "Bob"
print name
```

**Print expressions:**
```kronos
set x to 10
set y to 20
print x plus y
```

---

## Arithmetic Operations

Kronos uses human-readable words for arithmetic operations.

### Addition
```
<value> plus <value>
```

```kronos
set sum to 5 plus 3
print sum
```

### Subtraction
```
<value> minus <value>
```

```kronos
set difference to 10 minus 4
print difference
```

### Multiplication
```
<value> times <value>
```

```kronos
set product to 6 times 7
print product
```

### Division
```
<value> divided by <value>
```

```kronos
set quotient to 20 divided by 4
print quotient
```

### Complex Expressions

Operations are evaluated left to right. Use variables to control order:

```kronos
set a to 2 plus 3 times 4
print a

set step1 to 3 times 4
set b to 2 plus step1
print b
```

### Examples
```kronos
set x to 10
set y to 3

set sum to x plus y
set diff to x minus y
set prod to x times y
set quot to x divided by y

print sum
print diff
print prod
print quot
```

---

## Comparison Operators

Comparisons return boolean values and are used in conditional statements.

### Equal
```
<value> is equal <value>
```

```kronos
set a to 5
set b to 5
if a is equal b:
    print "a equals b"
```

### Not Equal
```
<value> is not equal <value>
```

```kronos
set x to 10
set y to 20
if x is not equal y:
    print "x is not equal to y"
```

### Greater Than
```
<value> is greater than <value>
```

```kronos
set score to 85
if score is greater than 70:
    print "Passing grade"
```

### Less Than
```
<value> is less than <value>
```

```kronos
set temperature to 32
if temperature is less than 50:
    print "It's cold"
```

### Greater Than or Equal (future)
```
<value> is greater than or equal <value>
```

### Less Than or Equal (future)
```
<value> is less than or equal <value>
```

---

## Conditional Statements

Execute code based on conditions.

### If Statement

**Syntax:**
```
if <condition>:
    <indented block>
```

**Examples:**

Simple condition:
```kronos
set age to 18
if age is greater than 17:
    print "You are an adult"
```

With variable assignment:
```kronos
set score to 95
if score is greater than 90:
    set grade to "A"
    print grade
```

Multiple statements:
```kronos
set balance to 1000
if balance is greater than 500:
    print "Sufficient funds"
    set canPurchase to true
    print canPurchase
```

Nested conditions:
```kronos
set x to 10
if x is greater than 5:
    print "x is greater than 5"
    if x is less than 15:
        print "x is also less than 15"
```

**Note:** Currently, only `if` is supported. `else` and `else if` are planned for future releases.

---

## Loops

Repeat code multiple times.

### For Loop

Iterate over a range of numbers.

**Syntax:**
```
for <variable> in range <start> to <end>:
    <indented block>
```

**Examples:**

Count from 1 to 5:
```kronos
for i in range 1 to 5:
    print i
```

Count from 0 to 10:
```kronos
for counter in range 0 to 10:
    print counter
```

Use loop variable in calculations:
```kronos
for i in range 1 to 10:
    set square to i times i
    print square
```

Nested loops:
```kronos
for i in range 1 to 3:
    for j in range 1 to 3:
        set product to i times j
        print product
```

**Range behavior:**
- `range <start> to <end>` is inclusive on both ends
- `range 1 to 5` includes 1, 2, 3, 4, and 5

### While Loop

Repeat while a condition is true.

**Syntax:**
```
while <condition>:
    <indented block>
```

**Examples:**

Count up:
```kronos
set count to 1
while count is less than 6:
    print count
    set count to count plus 1
```

Count down:
```kronos
set countdown to 10
while countdown is greater than 0:
    print countdown
    set countdown to countdown minus 1
```

With complex conditions:
```kronos
set x to 0
set limit to 100
while x is less than limit:
    print x
    set x to x plus 10
```

**Warning:** Be careful to update variables in the loop to avoid infinite loops!

---

## Comments

Currently, Kronos does not support comments in the language. This is planned for a future release.

**Planned syntax:**
```kronos
# This is a comment
set x to 5  # Comments can also be inline
```

---

## Indentation

Kronos uses indentation to define code blocks, similar to Python.

### Rules

1. **Consistent Indentation:** Use spaces or tabs, but be consistent
2. **Block Indicators:** Colons (`:`) indicate the start of a block
3. **Indent Level:** Increase indentation for nested blocks

### Examples

**Single block:**
```kronos
if x is greater than 5:
    print "x is large"
    print "Still in the if block"
```

**Nested blocks:**
```kronos
for i in range 1 to 3:
    print i
    if i is equal 2:
        print "Found two!"
```

**Multiple nesting:**
```kronos
for i in range 1 to 5:
    print i
    for j in range 1 to 3:
        set product to i times j
        print product
        if product is greater than 10:
            print "Large product"
```

### Common Mistakes

**Incorrect (inconsistent indentation):**
```kronos
if x is greater than 5:
  print "Two spaces"
    print "Four spaces"  # ERROR: Inconsistent
```

**Correct:**
```kronos
if x is greater than 5:
    print "Four spaces"
    print "Four spaces"
```

---

## Complete Examples

### Example 1: Temperature Converter
```kronos
print "Fahrenheit to Celsius Converter"
set fahrenheit to 98.6
set step1 to fahrenheit minus 32
set step2 to step1 times 5
set celsius to step2 divided by 9
print celsius
```

### Example 2: Sum of Numbers
```kronos
print "Sum of numbers 1 to 10:"
set sum to 0
for i in range 1 to 10:
    set sum to sum plus i
print sum
```

### Example 3: Multiplication Table
```kronos
print "Multiplication table for 5:"
for i in range 1 to 10:
    set result to 5 times i
    print result
```

### Example 4: Countdown
```kronos
print "Countdown from 10:"
set count to 10
while count is greater than 0:
    print count
    set count to count minus 1
print "Liftoff!"
```

### Example 5: Find Maximum
```kronos
print "Finding maximum in a sequence"
set max to 0
for i in range 1 to 20:
    if i is greater than max:
        set max to i
print "Maximum is:"
print max
```

---

## Future Features

The following features are planned for future releases:

### Functions
```kronos
function greet with name:
    print "Hello"
    print name
    return name

call greet with "Alice"
```

### Else Statements
```kronos
if x is greater than 10:
    print "x is large"
else:
    print "x is small"
```

### Logical Operators
```kronos
if x is greater than 5 and x is less than 10:
    print "x is between 5 and 10"

if x is equal 0 or y is equal 0:
    print "At least one is zero"
```

### Lists
```kronos
set numbers to list 1, 2, 3, 4, 5
set first to numbers at 0
```

### Concurrency (Goroutines)
```kronos
spawn task with:
    print "Running in parallel"
```

### Exception Handling
```kronos
try:
    set result to x divided by 0
catch error:
    print "Division by zero"
```

---

## Tips and Best Practices

1. **Use descriptive variable names:**
   ```kronos
   # Good
   set totalPrice to 100
   set customerName to "John"
   
   # Avoid
   set x to 100
   set n to "John"
   ```

2. **Break complex expressions into steps:**
   ```kronos
   # Instead of nested calculations
   set x to a plus b times c minus d divided by e
   
   # Break it down
   set product to b times c
   set quotient to d divided by e
   set sum to a plus product
   set result to sum minus quotient
   ```

3. **Consistent indentation (4 spaces recommended):**
   ```kronos
   if condition:
       print "Use 4 spaces"
       for i in range 1 to 5:
           print i
   ```

4. **Initialize counters before loops:**
   ```kronos
   set sum to 0  # Initialize before use
   for i in range 1 to 10:
       set sum to sum plus i
   ```

5. **Be careful with infinite loops:**
   ```kronos
   # Good - counter increases
   set count to 0
   while count is less than 10:
       print count
       set count to count plus 1
   
   # Bad - infinite loop
   set count to 0
   while count is less than 10:
       print count
       # Forgot to increment!
   ```

---

## Error Messages

Common errors you might encounter:

- **"Undefined variable"** - Using a variable before setting it
- **"Division by zero"** - Attempting to divide by zero
- **"Stack overflow"** - Too many nested operations
- **"Unexpected token"** - Syntax error in your code

---

## Getting Help

For more examples, check the `examples/` directory:
- `hello.kr` - Basic printing and variables
- `arithmetic.kr` - Arithmetic operations
- `conditionals.kr` - If statements
- `loops.kr` - For and while loops
- `test.kr` - Mixed examples

Run the REPL for interactive testing:
```bash
./kronos
```

Execute a file:
```bash
./kronos your_program.kr
```

---

*Last updated: November 2025*
*Kronos Language Version: 0.1.0*

