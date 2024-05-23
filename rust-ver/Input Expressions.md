# Input Expressions

This command line tool will accept specific commands to perform operations. These commands involve evaluations, variable storage, management of variables, etc. Here is a list of the commands that this program will support.

## `eval`

This is the default expression. This has two forms:
```
[--na] [--type] [--p] [Expression]
eval [--na] [--type] [--p] [Expression]
```
The `--na` modifyer means that it will not store the result of the expression in `ans`. 
The `--type` modifyer will output the type of the evaluated expression. For instance, if the result is `2.2`, and this modifyer is used, it will return `Scalar:2.2`.
The `--p` modifyer print the postfix of the input expression to the terminal before evaluation. 

Note that for the `eval` command, all modifiers **must** come before the [Expression], as the 

Note that if the `new` expression is used, the result will simply be copied, and will not be stored within a variable expect for `ans`, providing `--na` was not used. 


## `sto`

This command will store a value into a variable. This has the following format:

```
sto [Expression] in $[Variable Name] [--na]
```

If `[VariableName]` already exists, it will be updated with the new value, and if not, it will be created and assigned the value. This takes all kinds of expressions. 
The only restriction on the variable name is that it cannot be the environment variable `ans`. 

The `--na` modifyer means that it will not store the result of the expression in `ans`. 

## `drop`

This command will remove a variable from memory and remove its name. It has a few cases:

1. `drop ans` - Removes the value associated with `ans`, and sets it to `None`.
2. `drop *` - Removes all variables, resets `ans`, and removes all stored functions.
3. `drop $*`- Removes all variables, but not `ans` or stored functions.
4. `drop $[Name]` - Removes the variable with name `[Name]`. 
5. `drop #*` - Removes all stored functions, but not `ans` or any variables.
6. `drop #[Name]` - Removes the function with name `[Name]`.

## Expressions Format

Expressions are used in each of the commands listed before, and they come in various forms. Some expressions are not valid in some commands.

### `ans`

Using this in an expression returns the result of the previous `eval` or `sto` that did not use `--na`. If no `ans` has been set (no commands used) or the variables have been dropped, `ans` returns `None`. 

### `new`

The `new` expression constructs an exotic type. This expression is used to make vectors, matrices, and functions. The declorations goes as follows:

#### `new matrix(m, n)` - Creates a matrix of `m` rows and `n` columns

For the `new matrix` expression, after the expression is checked, the user will be asked to do `m` rows of `n` expressions, where the expressions are not `new`. Matrices do not support storing exotic types, and only store scalars. 
An example would be:
```
>sto new matrix(3, 3) in $A
new matrix(3,3), enter 3 expressions per row, separated by commas, for 3 rows:
>[Expr1], [Expr2], [Expr3] 
>[Expr4], [Expr5], [Expr6]
>[Expr7], [Expr8], [Expr9]
```
Assuming that all expressions are valid, the matrix will be made.

#### `new vector(d)` - Creates a vector of dimension `d`

The `new vector` expression comes in three forms:
1. `new vector(d)` - Asks the user for the values of the vector after command processing.
2. `new vector(d){[Expr1], [Expr2], ..., [Exprd]}` - Ensures that the number of expressions in the `{}` equal `d`, and fills the vector with those values without prompting for values after processing.
3. `new vector{[Expr1], [Expr2], ..., [Exprd]}` - Makes a vector of dimension `d` and fills it with the values.

Vectors do not support storing exotic types, and only store scalars.

#### `new function` - Creates a stored function

### Expressions Format

In this program, an expression is defined to be a collection of operations done on numbers, exotic types (declared with `new`), 
