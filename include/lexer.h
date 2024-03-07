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
  tok_do = -5,

  // primary
  tok_identifier = -6,
  tok_number = -7,
  tok_string = -8,

  tok_import = -9,
  // errors
  tok_error = -10
};

Token getTok();
extern std::string IdentifierStr;
extern double NumVal;
extern std::string StrVal;

#endif // LEXER_H
