#include <cctype>
#include <cstdio>
#include <string>

enum Token {
  tok_eof = -1,

  // commands
  tok_fn = -2,
  tok_end = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5,

  // errors
  tok_error = -6
};

class Lexer {
public:
  Lexer() : LastChar(' ') {}

  Token getNextToken() {
    while (isspace(LastChar))
      LastChar = getchar();

    if (isalpha(LastChar))
      return tokenizeIdentifier();

    if (isdigit(LastChar) || LastChar == '.')
      return tokenizeNumber();

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

    return tok_identifier;
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

static void reportError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
}

int main() {
  Lexer lexer;
  Token tok;
  while ((tok = lexer.getNextToken()) != tok_eof) {
    if (tok == tok_error) {
      reportError("Invalid token");
      return 1;
    }
    printf("Token: %d\n", tok);
  }
  return 0;
}
