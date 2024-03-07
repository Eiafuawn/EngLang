#include "lexer.cpp"
#include "ast.cpp"
Lexer lexer;

extern std::unique_ptr<ExprAst> ParseDefinition();

static void HandleDefinition() {
    if (ParseDefinition()) {
      fprintf(stderr, "Error: failed to parse definition\n");
  } else {
    lexer.getTok();
  }
}

extern std::unique_ptr<ExprAst> ParseImport();

static void HandleImport() {
  if (ParseImport()) {
    fprintf(stderr, "Error: failed to parse import\n");
  } else {
    lexer.getTok();
  }
}

extern std::unique_ptr<ExprAst> ParseTopLevelExpr();
static void HandleTopLevelExpr() {
  if (ParseTopLevelExpr()) {
    fprintf(stderr, "Error: failed to parse top level expression\n");
  } else {
    lexer.getTok();
  }
}

static void MainLoop() {
  while (true) {
    fprintf(stderr, "ready> ");
    switch (lexer.getTok()) {
    case tok_eof:
      return;
    case ';':
      lexer.getTok();
      break;
    case tok_fn:
      HandleDefinition();
      break;
    case tok_import:
      HandleImport();
      break;
    default:
      HandleTopLevelExpr();
      break;
    }
  }
}
