# RPNlang
programming language interpreter based on Reverse Polish Notation just for fun ;)


## A beautiful syntax
In RPNlang, there is no need to use horribles and ugly curly brackets or parentheses. Instead, you use the most beautiful syntax you can imagine: the **reverse polish notation** (RPN for the insiders).

Exemples are better than words so, there are some just here waiting for your beautiful eyes:

Comments:
```
# comment a line
```

Display "5 + 6 = 11":
```
5 6 5 6 + f"{} + {} = {}" :print
```

Calculate first four pi decimal and display them:
```
3.0 8.0 60.0 / + 29.0 60.0 2 ^ / + 44.0 60.0 3 ^ / + :print
```

Conditions:
```
a b == if: 
	"hello world" :print 
else:
	"nope" :print
```