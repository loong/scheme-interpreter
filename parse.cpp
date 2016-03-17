/**
 * \file parse.cpp
 *
 * Implementation of a parser that analyzes a string containing an
 * s-expression, and determines its tree structure.
 */

#include "parse.hpp"
bool inparsecar;
// check whether chr is white space
bool iswhitespace(char ch)
{
  if ((' ' == ch)||('\n' == ch)||('\t' == ch)||('\r' == ch)) {
    return true;
  } else {
    return false;
  }
}


/**
 * \brief Check whether numericstr is an legal numericstr string
 * \param str The string to be checked
 * \return ture if numericstr is an legal numericstr string, false otherwise
 */
bool is_legalnumeric(string str) 
{
  int dotnum = 0;
  int length = str.length();
  int i;
  if ('.' == str[0]) {
    dotnum ++;
  } else if ( !((str[0] >= '0') && (str[0] <= '9')) && ('+'!=str[0]) && ('-'!=str[0])) {
    return false;
  }
  for (i = 1; i < length; i ++) {
    if ('.' == str[i]) {
      dotnum ++;
    } else if ((str[i] < '0') || (str[i] > '9')) {
      return false;
    }
  }
  if (dotnum>1) {
    return false;
  }
  return true;
}

/**
 * \brief Check whether str is a legal operator
 * 
 */
bool is_legaloperator(string str)
{
  return true;
}

void readsinglesymbol(string& substring, string& sexpr)
{
  char currentchar;
  int i = 0;
  int length = substring.length();

  // get the first character
  currentchar = substring[i];

  if (currentchar == '\"') {
    // read a string literal
    sexpr += currentchar;
    do {
      ++i;
      currentchar = substring[i];
      sexpr += currentchar;
    } while (currentchar != '\"' && i < length-1);
    if ('\"' != currentchar) {
      cout << "error: illegal string" << endl;
      exit(1);
    }
  } else {
    // read a numeric literal or operator
    do {
      sexpr += currentchar;
      ++i;
      if (i >= static_cast<int>(substring.size())) {
	break;
      }
      currentchar = substring[i];
    } while ((!iswhitespace(currentchar)) && (currentchar != '(') && currentchar != '\"');
    --i;
  }
  substring = substring.substr(i+1, length - i - 1);
}


/**
 * \brief Clear the whitespace at the begining and end of string sexpr.
 * \param sexpr The string.
 */
void clearwhitespace(string& sexpr)
{
  if (sexpr.size() <= 0) {
    return;
  }
  int leftvalidpos;
  int rightvalidpos;
  int length = sexpr.size();
  int i;
  // most left non-whitespace position
  for (i = 0; i < length; ++i) {
    if (iswhitespace(sexpr[i])) {
      continue;
    } else {
      leftvalidpos = i;
      break;
    }
  }

  // most right non-whitespace position
  for (i = length - 1; i >= 0; i --) {
    if (iswhitespace(sexpr[i])) {
      continue;
    } else {
      rightvalidpos = i;
      break;
    }
  }

  // delete the white space at the beginning and end
  if ( i == -1 ) sexpr = "";
  else sexpr  =  sexpr.substr(leftvalidpos, rightvalidpos - leftvalidpos + 1);
}

/**
 * \brief Check whether the s-expression legal
 */
bool is_legalexpr(string sexpr)
{
  clearwhitespace(sexpr);
  if (sexpr.length()==0) {
    cout << "blank string " << endl;
    return false;
  }
  if (')' == sexpr[0]) {
    cout << "error: illegal s-expression" << endl;
    return false;
  }
  if ('(' == sexpr[0]) {
    // it is expression
    int length = sexpr.length();
    int inumleftparenthesis = 1;
    int i;
    int quotationmark = 0;
    for (i = 1; i < length; i ++ ) {
      if ('\"' == sexpr[i]) {
        quotationmark ++;
        quotationmark = quotationmark%2;
      } else if ('(' == sexpr[i] && 0 == quotationmark) {
        inumleftparenthesis ++;
      } else if (')' == sexpr[i] && 0 == quotationmark) {
        inumleftparenthesis --;
      }
      if (0 == inumleftparenthesis) {
        break;
      }
    }
    if ((i < length - 1) || (i == length) || (inumleftparenthesis > 0) || 0 != quotationmark) {
      cout << "error: illegal s-expression " << endl;
      return false;
    }
  } else if ('\"' != sexpr[0]) {
    // single element
    if (string::npos != sexpr.find('(') || string::npos != sexpr.find(')') || string::npos != sexpr.find(' ') || string::npos != sexpr.find('\"'))  {
      cout << "error: illegal s-expression " << endl;
      return false;
    }
    // check whether str is illegal numeric literal or illegal operator
    if ((false == is_legalnumeric(sexpr)) && (false ==is_legaloperator(sexpr))) {
      cout << "error: illegal numeric literal or illegal operator" << endl;
      return false;
    }
  } else {
    int length = sexpr.length();
    int inumleft = 1;
    int i;
    for (i = 1; i < length; i ++) {
      if ('\"' == sexpr[i]) {
        inumleft ++;
      }
      if (2 == inumleft) {
        break;
      }
    }
    if ((i < length-1) || (inumleft != 2)) {
      cout << "error: illegal s-expression " << endl;
      return false;
    }
  }
  return true;
}

/**
 * \brief Make the cell.
 * \param str The string to represent the symbol, int or double.
 */
Cell* makecell(string str)
{
  Cell* root;
  if (((str[0] >= '0') && (str[0] <= '9')) || (str[0] == '.') 
      || ((('+'==str[0]) || ('-'==str[0]))&&(str.length()>1))) {
    if (false == is_legalnumeric(str)) {
      cout << "error: illegal numeric literal" << endl;
      exit(1);
    }
    // this is a numeric literal
    if (string::npos == str.find('.')) {
      // int number
      char* fchar = const_cast<char*>(str.c_str());
      int value = atoi(fchar);
      root = make_int(value);
    } else {
      // this is a double
      char* fchar = const_cast<char*>(str.c_str());
      double value = atof(fchar);   
      root = make_double(value);
    }
  } 
  
  // we don't deal with literal strings right now, so they are commented out
  // else if (str[0] == '\"') {
//     // this is a string literal
//     string strval = str.substr(1, str.size() - 2);
//     root = make_string(const_cast<char*>(strval.data()));
//   } 
  else {
    // this is a symbol
    if (false == is_legaloperator(str)) {
      cout << "error: illegal operator" << endl;
      exit(1);
    }
    root = make_symbol(const_cast<char*>(str.data()));
  }
  return root;
}

Cell* separate_parse(string& sexpr); 

Cell* parse(string sexpr)
{
  // is_legal(sexpr);
  // delete the whitesapce at the begining and end
  // such that the first and last character are not white space
  clearwhitespace(sexpr);
//   if (sexpr.length() == 0) {
//     return nil;
//   }
  if (sexpr.length() == 0) {
    return nil;
  }
  if ( !is_legalexpr(sexpr)) {
    return nil;
  }
  // check whether is single symbol
  // i.e. leaf cell
  // if (string::npos == sexpr.find('(')) {
  if ('(' != sexpr[0]) {
    // this is leaf cell
    // bulid this leaf cell
    Cell* root = makecell( sexpr );
    return root;
  }

  // the first and last character are '(' and ')', respectively
  // delete the two characters
  int length = sexpr.size();
  sexpr = sexpr.substr(1, length-2);
  clearwhitespace(sexpr);
  length = sexpr.size();
//   if ( inparsecar ) {
//     if (sexpr == "") {
//       Cell* ec = new Cell("()");
//       return ec;
//     }
//   }
  // separate the s-expression into two left and right subsexps
  Cell* root = separate_parse(sexpr);

  return root;
}

/**
 * \brief Separately parse the sexpr and build the tree.
 * \param instr The string which consists of s-expressions.
 * \return A pointer to the conspair cell at the root of the parse tree.
 */
Cell* separate_parse(string& instr)
{
  string sexp;
  bool isstartsexp = false;
  int inumleftparenthesis = 0;

  // check whether to read the end
  clearwhitespace(instr);
  int length = instr.size();
    // check whether it is a "()" sexpr

  while (instr.size() > 0) {
    // read char by char
    char currentchar = instr[0];
    // skip some white space before new s-expression occurs
    if ((true == iswhitespace(currentchar))&&(false == isstartsexp)) {
      continue;
    }
    // run accross a new s-expression
    if ((false == isstartsexp)&&(false == iswhitespace(currentchar))) {
      // check whether single symbol
      if ('(' != currentchar) {
	// read single a single symbol
	readsinglesymbol(instr, sexp);
	clearwhitespace(instr);  
	inparsecar = true;
	Cell* car = parse(sexp);
	inparsecar = false;
	Cell* cdr = parse("(" + instr + ")");
	Cell* root = cons(car, cdr);
	sexp.clear();
	return root;
      } else {
	// start new expression
	isstartsexp = true;
	// read left parenthesiss
	sexp += currentchar;
	instr = instr.substr(1, instr.size() -1);
	inumleftparenthesis = 1;
      }
    } else {
      // in the process of reading the current s-expression
      if (true == isstartsexp) {
	if (true == iswhitespace(currentchar)) {
	  // append a blankspace
	  sexp += ' ';
	  instr = instr.substr(1, instr.size() -1);
	} else {
	  // append current character
	  sexp += currentchar;
	  instr = instr.substr(1, instr.size() -1);
	  // count left parenthesiss
	  if ('(' == currentchar) {
	    inumleftparenthesis ++;
	  }
	  if (')' == currentchar) {
	    inumleftparenthesis --;

	    // check whether current s-expression ends
	    if (0 == inumleftparenthesis) {
	      // current s-expression ends
	      isstartsexp = false;
	      clearwhitespace(instr);
	      inparsecar = true;
	      Cell* car = parse(sexp);        
	      inparsecar = false;
	      int length = instr.length();
	      Cell* cdr;
	      Cell* root;
	      if (length <= 0) {
		cdr = nil;
	      } else {
		cdr = parse("(" + instr + ")");       
	      }
	      root = cons(car, cdr);
	      sexp.clear();
	      return root;     
	    }
	  }
	}
      }
    }
  }

  return nil;
}
