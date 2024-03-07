#include "lexer.h"

// Function to get the next token
Token getTok(char &LastChar) {
    // Skip whitespace
    while (isspace(LastChar))
        LastChar = getchar();

    // Identifier: [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(LastChar)) {
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

        return tok_number;
    }

    // String: ".*" or '.*'
    if (LastChar == '"' || LastChar == '\'') {
        std::string StrVal;
        char quoteChar = LastChar;
        LastChar = getchar(); // Consume the opening quote
        while (LastChar != EOF && LastChar != quoteChar) {
            StrVal += LastChar;
            LastChar = getchar();
        }
        if (LastChar == quoteChar)
            LastChar = getchar(); // Consume the closing quote
        return tok_string;
    }

    // Comment: #.*
    if (LastChar == '#') {
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r')
            LastChar = getchar();
        return getTok(LastChar); // Get the next token after the comment
    }

    // Return single character token
    if (LastChar != EOF) {
        int ThisChar = LastChar;
        LastChar = getchar();
        return static_cast<Token>(ThisChar);
    }

    return tok_eof;
}

static void reportError(const char *Str) {
    fprintf(stderr, "Error: %s", Str);
}
