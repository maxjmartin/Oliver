# Test-Interpreter
An (experimental) immutable interpreted programming language. Written in C++, and inspired by Python, Lisp, and Forth. It is designed to be thread safe and suitable for real time systems.  This was written to understand how programming languages can be implemented.  

### Code Comments
Comments may be added to code using the operator '#'.  A single '#' will comment out the remainder of a line of text.  A double '##' will comment out all code until another '##' has been encountered.  Using '##' operators requires at least one whitespace character between each operator.  

### Expressions
Oliver accepts and evaluates expressions.  And returns the result the expression evaluation.  In general expression may be bound by the pair of characters ( and ) or also : and ;.  
```
(a b c d)
:a b c d;
```

### Typed Expressions
Specific 'primitive' types of expressions may be defined using specific characters to identify the start and finish of a primitive data type.
```
'42'    # A number.
"text"  # A text string.
```

### Anonymous Functions
Lambda expressions are used to define anonymous functions.  Using the format identifier of 'lambda' followed by two expressions.  The first being the arguments the function accepts.  The second being the body of the function to be evaluated.  
```
lambda(x): x ;
lambda(x y): x ;
lambda(x y): y ;
lambda(f):
    lambda(x):
        f (x x)
    ;
    lambda(x):
        f (x x)
    ;
;
```

### Constant and Variable Assignment
The keyword 'let' initiates an assignment of data to a constant or variable.  What must follow is three expressions.  The argument(s) to assign values to, the operation of assignment to perform, and the values to be assigned.  
```
let x = '2'                 # Variable assignemnt.
let (x y) = ('42' '12')     # Multiple variable assignment.
let x := '2'                # Constant assignemnt.
let (x y) := ('42' '12')    # Multiple constant assignment.
let z = lambda(x y): y ;    # Assign an anonymous function as the value of z.
```

### Application Assignment
If an expression needs to be evaluated before it can be assigned as the value of a constant or variable, the application assignment operator '=:' can be used for variables, and ':=:' can be used for constants.
```
let x =: (f '42')   # Assign the result of the application of f('42') to the variable x.
let y :=: (g '42')  # Assign the result of the application of g('42') to the variable y.
```

### Named Functions
Short hand assignment of a name to an anonymous function can be done during the definition of a function using the keyword 'def'.  Named function defined in this manner are always assigned as variables.  
```
def f(x):
    x * '2'
;
```

### Whitespace
Whitespace is used to separate individual expression of code which are not defined using a specific character or characters to indicate the start and the stop of an expression.  