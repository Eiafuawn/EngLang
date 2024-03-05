#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <string>

/* Keywords:
 *  - fn: function
 *  - if, do, else, elseif: conditional
 *  - let: variable
 *  - end: to end blocks (loop, if, etc followed by keyword)
 */
enum Token {
  tok_eof = -1,

  // commands
  tok_fn = -2,
  tok_end = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5,

  // error
  tok_error = -6
};

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;

static int gettok() {
  static int LastChar = ' ';

  // Skip any whitespace.
  while (isspace(LastChar))
    LastChar = getchar();

  if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    IdentifierStr = LastChar;
    while (isalnum((LastChar = getchar())))
      IdentifierStr += LastChar;

    if (IdentifierStr == "fn")
      return tok_fn;
  }

  // Get int and floats as numbers
  if (isdigit(LastChar) || LastChar == '.') { // Number: [0-9.]+
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');

    NumVal = strtod(NumStr.c_str(), nullptr);
    return tok_number;
  }

  // Comment until end of line.
  if (LastChar == '#') {
    do
      LastChar = getchar();
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF)
      return gettok();
  }

  // Check for end of file. Don't eat the EOF.
  if (LastChar == EOF)
    return tok_eof;
  
  // else just return the current char
  int ThisChar = LastChar;
  LastChar = getchar();
  return ThisChar;
}

static void reportError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
}
