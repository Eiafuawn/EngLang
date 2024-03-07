#ifndef LEXER_H
#define LEXER_H

#include <cctype>
#include <cstdio>
#include <string>

enum Token {
  tok_eof = -1,

  // commands
  tok_fn = -2,
  tok_end = -3,
  tok_if = -4,
  tok_loop = -5,
  tok_do = -6,

  // primary
  tok_identifier = -7,
  tok_number = -8,
  tok_string = -9,

  tok_import = -10,
  // errors
  tok_error = -11
};

extern int getTok();
extern std::string IdentifierStr;
extern double NumVal;
extern std::string StrVal;

#endif // LEXER_H
