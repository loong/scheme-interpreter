MicroScheme - A Scheme Interpreter
==================================

Implementation of a basic *turing-complete* Scheme interpretation, complying to [these specs](https://mitpress.mit.edu/sicp/full-text/book/book-Z-H-4.html).

To reach turing-completness, C++ is used as bootstrap medium. After I finished creating my 'own scheme', I continued to extend the language with it's own syntax, which can be found in `library.scm`

This project was created during the Honors course for OOP and Data Structures (COMP 2012H) at the Hong Kong University of Science and Technology, instructed by [Prof. Dekai Wu](http://www.cse.ust.hk/~dekai/) and Karteek Addanki.

The old (and unclean) project from the course with full commit history [can be found on bitbucket](https://bitbucket.org/mindworker/microscheme/overview).

## How to build
### Pre-requisite
Make sure you have all dependencies and build tools installed to compile and link a C++. If not you can install in Debian / Ubuntu it with:
```
sudo apt-get install build-essential
```
### Build and run
Simply compile with `make` which results in an executable `main` file
```
make
./run
```

## Bonus 'Game'
A Labyrinth generator is implemented with this scheme implementation. The code can be found in `library.scm` and runs once on startup. You can run it manually by executing this in the scheme shell:
```
(example-labyrinth)
```
This was extremely hard, because there is no random access in my scheme interpretation. I made a workaround by converting the linked-list syntax into a string and do string operations on it in order to come near to random access. You can see the *performance comparison* by executing the following in the scheme shell:
```
(example-performance)
```

## General design remarks
  * Every operation is seen as a function. eval() uses Cell::apply() which call the corresponding (overwritten) method of either 
  * Difference between ArithmeticCell and FunctionCell
  ** `ArithmeticCell` is used for undefined number of operands/ argument and uses a recursive approach
  ** `FunctionCell` is used for functions (where if-else branching is considered as a function as well) with a defined number of arguments
  * Internally the Cell Abstract Base Class is called CellABC (to make it
    really clear). In order to comply to the cons.hpp interface a typedef 
    alias to Cell is used.
  * Function called by FunctionCells are separated in functions.hpp (easier
    to maintain). Cell.hpp only specified which functions are available.

## Further improvements
`FunctionCell` and `ArithmeticCell` could be merged into a single unit neatly.

## Acknoledgements

Special thanks to Dekai Wu for to give me theoretical background to build this interpreter and to Karteek Addanki to give tips on best practices and comments on my code.
