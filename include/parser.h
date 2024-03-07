#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"
#include <map>

extern std::map<char, int> BinopPrecedence;

// Forward Declarations
extern std::unique_ptr<ExprAst> ParseExpression();

// Utility Functions
extern int getNextToken();
extern std::unique_ptr<ExprAst> LogError(const char *Str);
extern std::unique_ptr<PrototypeAst> LogErrorP(const char *Str);

// Parsing Functions
extern std::unique_ptr<ExprAst> ParsePrimary();
extern std::unique_ptr<ExprAst> ParseBinOpRHS(int ExprPrec,
                                              std::unique_ptr<ExprAst> LHS);
extern std::unique_ptr<ExprAst> ParseParenExpr();
extern std::unique_ptr<ExprAst> ParseNumberExpr();
extern std::unique_ptr<ExprAst> ParseStringExpr();
extern std::unique_ptr<ExprAst> ParseIdentifierExpr();
extern std::unique_ptr<FunctionAst> ParseFn();
extern std::unique_ptr<PrototypeAst> ParseImport();
extern std::unique_ptr<FunctionAst> ParseTopLevelExpr();
extern int GetTokPrecedence();

extern int CurTok;
int getNextToken();

#endif // PARSER_H
