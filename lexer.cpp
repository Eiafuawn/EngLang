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

  // errors
  tok_error = -9
};

class Lexer {
public:
  Lexer() : LastChar(' ') {}

  Token getNextToken() {
    while (isspace(LastChar))
      LastChar = getchar();

    if (isalpha(LastChar))
      return tokenizeIdentifier();

    if (isdigit(LastChar))
      return tokenizeNumber();

    if (LastChar == '"' || LastChar == '\'')
      return tokenizeString();

    if (LastChar == '#') {
      skipComment();
      return getNextToken();
    }

    if (LastChar == EOF)
      return tok_eof;

    // Return single character token
    int ThisChar = LastChar;
    LastChar = getchar();
    return static_cast<Token>(ThisChar);
  }

private:
  char LastChar;

  Token tokenizeIdentifier() {
    std::string IdentifierStr;
    do {
      IdentifierStr += LastChar;
      LastChar = getchar();
    } while (isalnum(LastChar));

    if (IdentifierStr == "fn")
      return tok_fn;
    if (IdentifierStr == "end")
      return tok_end;
    if (IdentifierStr == "if")
      return tok_if;
    if (IdentifierStr == "do")
      return tok_do;

    return tok_identifier;
  }

  Token tokenizeString() {
    std::string Str;
    LastChar = getchar();
    do {
      Str += LastChar;
      LastChar = getchar();
    } while (LastChar != '"' && LastChar != '\'');
    LastChar = getchar();
    return tok_string;
  }

  Token tokenizeNumber() {
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');

    return tok_number;
  }

  void skipComment() {
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r')
      LastChar = getchar();
  }
};

static void reportError(const char *Str) { fprintf(stderr, "Error: %s", Str); }
