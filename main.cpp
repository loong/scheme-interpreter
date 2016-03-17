/**
 * \file main.cpp
 *
 * Driver code implementing the main read-parse-eval-print loop.
 * Supports both (1) an interactive mode, and (2) a batch mode where
 * input expressions are read from the file specified by the first
 * command-line argument.
 */

#include <stdexcept>
#include "parse.hpp"
#include "eval.hpp"
#include <sstream>

using namespace std;

/**
 * \brief Parse and evaluate the s-expression, and print the result.
 * \param sexpr The string vaule holding the s-expression.
 */
void parse_eval_print(string sexpr)
{
  try {
    Cell* root = parse(sexpr);
    //    cout << endl;
    //    cout << *root << endl;
    Cell* result = eval(root);
    if ( result == nil ) {
      cout << "()" << endl;
    } else {
      cout << *result << endl;
    }
    // delete root;
    // delete result;
  } catch (runtime_error &e) {
    cerr << "ERROR: " << e.what() << endl;
  } catch (logic_error &e) {
    cerr << "LOGIC ERROR: " << e.what() << endl;
    exit(1);
  }
}

/**
 * \brief Read single single symbol into the end of a string buffer.
 * \param fin The input file stream.
 * \param str The string buffer.
 */
void readsinglesymbol(ifstream& fin, string& str)
{
  char currentchar;
  fin.get(currentchar);
  if (fin.eof()) {
    return;
  }
  if (currentchar == '\"') {
    // read a string literal
    do {
      str += currentchar;
      fin.get(currentchar);
    } while (currentchar != '\"');
    str += currentchar;
  } else {
    do {
      str += currentchar;
      fin.get(currentchar);
    } while ((false == iswhitespace(currentchar)) 
	     && ('(' != currentchar) 
	     && (false == fin.eof()));
    fin.putback(currentchar);  
  }
}

/**
 * \brief Read, parse, evaluate, and print the expression one by one from
 * the input stream.
 *
 * \param fin The input file stream.
 */
void readfile(ifstream& fin)
{
  string sexp;
  bool isstartsexp = false;
  int inumleftparenthesis = 0;

  // check whether to read the end
  while (!fin.eof()) {
    // read char by char
    char currentchar;
    fin.get(currentchar);
    if (fin.eof()) {
      break;
    }

    // skip some white space before new s-expression occurs
    if ((true == iswhitespace(currentchar))&&(false == isstartsexp)) {
      continue;
    }
    // run across a new s-expression
    if ((false == isstartsexp)&&(false == iswhitespace(currentchar))) {
      // check whether single symbol
      if ('(' != currentchar)	{
	// read a single symbol
	fin.putback(currentchar);
	readsinglesymbol(fin, sexp);
	// call function
	parse_eval_print(sexp);
	sexp.clear();
      }	else {
	// start new expression
	isstartsexp = true;
	// read left parenthesis
	sexp += currentchar;
	inumleftparenthesis = 1;
      }
    } else {
      // in the process of reading the current s-expression
      if (true == isstartsexp) {
	if (true == iswhitespace(currentchar)) {
	  // append a blankspace
	  //sexp += ' ';
	  sexp += currentchar;
	} else {
	  // append current character
	  sexp += currentchar;
	  // count left parenthesis
	  if ('(' == currentchar) {
	    inumleftparenthesis ++;
	  }
	  if (')' == currentchar) {
	    inumleftparenthesis --;
	    // check whether current s-expression ends
	    if (0 == inumleftparenthesis) {
	      // current s-expression ends
	      isstartsexp  =  false;
	      // call functions
	      parse_eval_print(sexp);
	      sexp.clear();
	    }
	  }
	}
      }
    }
  }
}

/**
 * \brief Read the expressions from the file.
 * \param fn The file name.
 */
void readfile(char* fn)
{
  ifstream fin(fn);
  readfile(fin);
  fin.close();
}

/**
 * \brief Read, parse, evaluate, and print the expression one by one from
 * the standard input, interactively.
 */
void readconsole()
{
  string sexpr;
  // read the input
  do {
    cout << "> ";
    getline(cin, sexpr);
    if (cin.eof()) {
      break;
    }
    if ("(exit)" == sexpr) {
      return;
    }
    parse_eval_print(sexpr);
  } while (true);
}

/**
 * \brief Call either the batch or interactive main drivers.
 */
int main(int argc, char* argv[])
{
  // read from the standard input
  readfile("library.scm");

  switch(argc) {
  case 1:
    readconsole();
    exit(0);
    break;
  case 2:
    // read from a file
    readfile(argv[1]);
    break;
  default:
    cout << "too many arguments!" << endl;
    exit(0);
  }
  return 0;
}
