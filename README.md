# RPNlang

programming language interpreter based on Reverse Polish Notation just for fun ;)

- [RPNlang](#rpnlang)
	- [1.1. A beautiful syntax](#11-a-beautiful-syntax)
	- [1.2. How to install?](#12-how-to-install)
	- [1.3. Exemples](#13-exemples)
	- [1.4. How it works?](#14-how-it-works)
	- [1.5. Language keywords](#15-language-keywords)
	- [1.6. Types](#16-types)

## 1.1. A beautiful syntax

In RPNlang, there is no need to use horribles and ugly curly brackets or parentheses. Instead, you use the most beautiful syntax you can imagine: the **reverse polish notation** (RPN for the insiders).

## 1.2. How to install?

You must download this repository:

```sh
git clone http https://github.com/Robotechnic/RPNlang.git
```

Then you can make the project:

```sh
cd RPNlang
mkdir build
cd build
mkdir RPNmodules
cmake ..
make -j$(nproc)
```

## 1.3. Exemples

Exemples are better than words so, there are some just here waiting for your beautiful eyes:

**Comments**:

```RPNlang
# comment a line
```

**Display "5 + 6 = 11"**:

```RPNlang
5 6 5 6 + f"{} + {} = {}" :print
```

**Calculate first four pi decimal and display them**:

```RPNlang
3.0 8.0 60.0 / + 29.0 60.0 2 ^ / + 44.0 60.0 3 ^ / + :print
```

**Boolean operators**:

```RPNlang
a b :and false :not c :and :or :print
```

will always output "true"

**Conditions**:

```RPNlang
a b == if
	"hello world" :print 
else
	"nope" :print
fi
```

**Loops**:

```RPNlang
a 10 =
b 0 =
a b > while
	"hello world" :print
	b b 1 + =
elihw
```

```RPNlang
i 0 10 1 for
	"hello world" :print
rof
```

**Functions**:

```RPNlang
hello a string -> none fun
	a f"hello {a}" :print
nuf
```

```RPNlang
pow a float b int -> float fun
	b 0 == if
		1 return
	fi
	0 b 1 for
		a a * =
	rof
	a return
nuf
```

```RPNlang
divide nb float -> float fun 
	nb .5 / return 
nuf
```

**Functions call**:

```RPNlang
"yeye" :hello
```

**Module import**:
By default, RPNlang modules are namespaced with the module name (or file name in case of custom module)

```RPNlang
"math" import
math.pi :print
5 :math.sqrt :print
math.pi 2 / math.cos :print
```

But you can also import a module with a custom namespace:

```RPNlang
"math" "mymath" importAs
mymath.pi :print
5 :mymath.sqrt :print
mymath.pi 2 / mymath.cos :print
```

**Lists**:

```RPNlang
a 1 2 3 4 5 6 7 8 9 10 list[int] =
a 0 get f"{}\n" :print
a 9 get f"{}\n" :print
a 5 get 7 =
```

**Structs**:

```RPNlang
"math" :import
Vector struct
	x -> float
	y -> float
tcurts

normalise Vector v -> none fun
	len v->x 2 ^ v->y 2 ^ + :math.sqrt =
	v->x v->x len / =
	v->y v->y len / =
nuf

v 10 10 Vector =
v->x f"{}\n" :print
v->y f"{}\n" :print
```

More exemples [here](https://github.com/Robotechnic/RPNlang/tree/master/examples)

## 1.4. How it works?

This language use a stack to store the things that you type. Each time that you use a function or a math operator, the required arguments are poped from the stack, processed and the result is pushed back to the stack. After each line of instructions, the stack must contain no more than one element (it can be empty), else the program will crash.
RPNlang is also a strongly typed language, each variable has a type (if you define a variable it will be implicitly typed). All the types can be implicitly casted to another if they are compatible (for instance, an integer can be converted to a float).

## 1.5. Language keywords

In this languages there are just a few language specific keywords, everithing else is a literal.
| Keyword   | Description |
|:---------:|:-----------:|
|  `if`     | If the condition is true, execute the block of instruction after it. Else skip it |
| `else`    | If the previous condition is false, execute the block of instruction after it.|
|  `fi`     | End of an if/else block |
|  `for`    | Execute the block of instruction for each increment in a given range |
|  `rof`    | End of a for loop |
|  `while`  | Execute the block of instruction while the condition is true |
|  `elihw`  | End of a while loop |
|  `fun`    | Define a function |
|  `nuf`    | End of a function block|
| `try`     | start of a try catch block |
| `catch`   | start of catch block |
| `finally` | start of a finally block |
| `yrt`     | end of try catch block |
| `import`  | import the given module |
| `importAs`| import the given module and rename it |

## 1.6. Types

There are a few types in this language:
| Type | Description |
|:----:|:-----------:|
| `int` | Integer number |
| `float` | Floating point number |
| `string` | String |
| `bool` | Boolean value |
| `function` | Function |
| `none` | Empty value |
| `list` | list of values |
| `struct` | Custom datatype that allow to agregate multiple values |
