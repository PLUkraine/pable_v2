# Pable

![screenshot](https://i.ibb.co/kXD3vV6/Screen-Shot-2019-06-01-at-9-06-56-PM.png)

## What is this?
This is a spreadsheet aplication. It can store integer values and evaluate expressions.  

Cells can depend on other cells. The dependency can be expressed with the following notation: $ColumnRow. For example, $A2 refers to 0 column and 2 row.  

## Expressions
Currently expressions should contain space between each individual token. For example, "( $A1 - 3 ) + 4".

Application supports +, - operators, brackets, cell references and integer values.

## Dependencies
Application can handle all kinds of dependencies. It detects cycles and propagates errors.

## Build
Qt >= 5.9 and qmake are required. You will also need a compliler that supports c++17 standard.
