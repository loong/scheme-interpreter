# Scheme Interpreter

## General remarks
  * Every operation is seen as a function. eval() uses Cell::apply() which call the corresponding (overwritten) method of either 
  * ArithmeticCell or FunctionCell
  ** ArithmeticCell is used for undefined number of operands/ argument and uses a recursive approach
  ** FunctionCell is used for functions (where if-else branching is considered as a function as well) with a defined number of arguments
  * Internally the Cell Abstract Base Class is called CellABC (to make it
    really clear). In order to comply to the cons.hpp interface a typedef 
    alias to Cell is used.
  * Function called by FunctionCells are separated in functions.hpp (easier
    to maintain). Cell.hpp only specified which functions are available.

## Acknoledgements

Special thanks to Dekai Wu for to give me theoretical background to build this interpreter and to Karteek Addanki to give tips on best practices and comments on my code.