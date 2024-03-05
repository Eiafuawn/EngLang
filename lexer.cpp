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
  // tok_identifier = -4,
  tok_string = -4,
  tok_number = -5,
  // tok_string = -6,
  tok_error = -7
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
  // Handle string literals
  if (LastChar == '"' || LastChar == '\'') {
    char quoteChar = LastChar;
    std::string Str;
    while (true) {
      LastChar = getchar();
      if (LastChar == EOF || LastChar == '\n' || LastChar == '\r') {
        // Unterminated string literal
        reportError("Unterminated string literal");
        return tok_error;
      }
      if (LastChar == '\\') { // Handle escape sequences
        LastChar = getchar();
        switch (LastChar) {
        case 'n':
          Str += '\n';
          break;
        case 't':
          Str += '\t';
          break;
        // Add more escape sequences as needed
        default:
          Str += LastChar;
          break;
        }
      } else if (LastChar == quoteChar) {
        LastChar = getchar();
        break; // End of string literal
      } else {
        Str += LastChar;
      }
    }
    StringVal = Str;
    return tok_string;
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

  if (LastChar == EOF)
    return tok_eof;

  int ThisChar = LastChar;
  LastChar = getchar();
  return ThisChar;
}

static void reportError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
}
