#include "../include/handlers.h"

void HandleDefinition() {
    if (ParseFn()) {
        fprintf(stderr, "Error: failed to parse definition\n");
    }
    else {
        getTok();
    }
}


void HandleImport() {
    if (ParseImport()) {
        fprintf(stderr, "Error: failed to parse import\n");
    }
    else {
        getTok();
    }
}

void HandleTopLevelExpr() {
    if (ParseTopLevelExpr()) {
        fprintf(stderr, "Error: failed to parse top level expression\n");
    }
    else {
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