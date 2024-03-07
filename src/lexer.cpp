#include "../include/lexer.h"
std::string IdentifierStr;
double NumVal;
std::string StrVal;

// Function to get the next token
int getTok() {
  static int LastChar = ' ';
  // Skip whitespace
  while (isspace(LastChar))
    LastChar = getchar();

  // Identifier: [a-zA-Z][a-zA-Z0-9]*
  if (isalpha(LastChar)) {
    IdentifierStr = LastChar;
    while (isalnum((LastChar = getchar())))
      IdentifierStr += LastChar;

    if (IdentifierStr == "fn")
      return tok_fn;
    if (IdentifierStr == "end")
      return tok_end;
    if (IdentifierStr == "if")
      return tok_if;
    if (IdentifierStr == "loop")
      return tok_loop;
    if (IdentifierStr == "do")
      return tok_do;
    if (IdentifierStr == "import")
      return tok_import;

    return tok_identifier;
  }

  // Number: [0-9.]+
  if (isdigit(LastChar) || LastChar == '.') {
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');

    NumVal = strtod(NumStr.c_str(), nullptr);
    return tok_number;
  }

  // String: ".*" or '.*'
  if (LastChar == '"' || LastChar == '\'') {
    char quoteChar = LastChar;
    LastChar = getchar(); // Consume the opening quote
    while (LastChar != EOF && LastChar != quoteChar) {
      StrVal += LastChar;
      LastChar = getchar();
    }

    LastChar = getchar(); // Consume the closing quote
    return tok_string;
  }

  // Comment: #.*
  if (LastChar == '#') {
    // Comment until end of line.
    do
      LastChar = getchar();
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF)
      return getTok();
  }

  if (LastChar == EOF)
    return tok_eof;

  int ThisChar = LastChar;
  LastChar = getchar();
  return ThisChar;
}

static void reportError(const char *Str) { fprintf(stderr, "Error: %s", Str); }
