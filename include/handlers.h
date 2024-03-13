#ifndef HANDLERS_H
#define HANDLERS_H

#include "parser.h"
#include "lexer.h"

extern void HandleDefinition();
extern void HandleNumber();
extern void HandleString();
extern void HandleImport();
extern void HandleTopLevelExpr();
extern void MainLoop();

#endif // HANDLERS_H
