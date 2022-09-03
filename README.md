# RPNlang
programming language interpreter based on Reverse Polish Notation just for fun ;)


## A beautiful syntax
In RPNlang, there is no need to use horribles and ugly curly brackets or parentheses. Instead, you use the most beautiful syntax you can imagine: the **reverse polish notation** (RPN for the insiders).

## Exemples
Exemples are better than words so, there are some just here waiting for your beautiful eyes:

**Comments**:
```
# comment a line
```

**Display "5 + 6 = 11"**:
```
5 6 5 6 + f"{} + {} = {}" :print
```

**Calculate first four pi decimal and display them**:
```
3.0 8.0 60.0 / + 29.0 60.0 2 ^ / + 44.0 60.0 3 ^ / + :print
```

**Boolean operators**:
```
a b :and false :not c :and :or :print
```
will always output "true"

**Conditions**:
```
a b == if
	"hello world" :print 
else
	"nope" :print
fi
```

**Loops**:
```
a 10 =
b 0 =
a b > while
	"hello world" :print
	b b 1 + =
elihw
```

```
i 0 10 1 for
	"hello world" :print
rof
```


**Functions**:
```
fun hello a string -> none:
	a f"hello {a}" :print
nuf
```

```
fun pow a float b int -> float:
	b 0 == if
		1 return
	fi
	0 b 1 for
		a a * =
	rof
	a return
nuf
```

```
divide fun nb float -> float: nb .5 / return nuf =
```

**Functions call**:
```
"yeye" :hello
```

**Module import**:
By default, RPNlang modules are namespaced with the module name (or file name in case of custom module)
```
"math" :import
math.pi :print
5 :math.sqrt :print
math.pi 2 / math.cos :print
```

But you can also import a module with a custom namespace:
```
"math" "mymath" :importAs
mymath.pi :print
5 :mymath.sqrt :print
mymath.pi 2 / mymath.cos :print
```

More exemples [here](https://github.com/Robotechnic/RPNlang/tree/master/examples)


## How it works?
This language use a stack to store the things that you type. Each time that you use a function or a math operator, the required arguments are poped from the stack, processed and the result is pushed back to the stack. After each line of instructions, the stack must contain no more than one element (it can be empty), else the program will crash.
RPNlang is also a strongly typed language, each variable has a type (if you define a variable it will be implicitly typed). All the types can be implicitly converted to each other if they are compatible (for example, a integer can be converted to a float).

## Language keywords
In this languages there are just a few language specific keywords, everithing else is a function.
| Keyword | Description |
|:-------:|:-----------:|
|  `if`   | If the condition is true, execute the block of instruction after it. Else skip it |
| `else`  | If the previous condition is false, execute the block of instruction after it.|
|  `fi`   | End of an if/else block |
|  `for`  | Execute the block of instruction for each increment in a given range |
|  `rof`  | End of a for loop |
|  `while`| Execute the block of instruction while the condition is true |
|  `elihw`| End of a while loop |
|  `fun`  | Define a function |
|  `nuf`  | End of a function block|
|  `->`   | Define function return type|

## Types
There are a few types in this language:
| Type | Description |
|:----:|:-----------:|
| `int` | Integer number |
| `float` | Floating point number |
| `string` | String |
| `bool` | Boolean value |
| `function` | Function |
| `none` | Empty value |
