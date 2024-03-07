#include "../include/handlers.h"
//// Simulate llvm IR generation

void HandleDefinition() {
  if (ParseFn()) {
    fprintf(stderr, "Parsed a definition\n");
  } else {
    getTok();
  }
}

void HandleImport() {
  if (ParseImport()) {
    fprintf(stderr, "Parsed an import\n");
  } else {
    getTok();
  }
}

void HandleTopLevelExpr() {
  if (ParseTopLevelExpr()) {
    fprintf(stderr, "Parsed top level expression\n");
  } else {
    getTok();
  }
}

void MainLoop() {
  while (true) {
    fprintf(stderr, "ready> ");
    switch (getTok()) {
    case tok_eof:
      return;
    case ';':
      getTok();
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
