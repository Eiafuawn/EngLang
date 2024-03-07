#include "../include/parser.h"

auto LHS = std::make_unique<VariableExprAst>("x");
auto RHS = std::make_unique<VariableExprAst>("y");

auto Result =
    std::make_unique<BinaryExprAst>('+', std::move(LHS), std::move(RHS));

std::map<char, int> BinopPrecedence;
int CurTok;
int getNextToken() { return CurTok = getTok(); }

std::unique_ptr<ExprAst> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

std::unique_ptr<PrototypeAst> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

std::unique_ptr<ExprAst> ParseNumberExpr() {
  auto Result = std::make_unique<NumberExprAst>(NumVal);
  getNextToken();
  return std::move(Result);
}

std::unique_ptr<ExprAst> ParseStringExpr() {
  auto Result = std::make_unique<StringExprAst>(StrVal);
  getNextToken();
  return std::move(Result);
}

std::unique_ptr<ExprAst> ParseParenExpr() {
  getNextToken(); // eat (.
  auto V = ParseExpression();
  if (!V)
    return nullptr;

  if (CurTok != ')')
    return LogError("expected ')'");
  getNextToken(); // eat ).
  return V;
}

std::unique_ptr<ExprAst> ParseIdentifierExpr() {
  std::string IdName = IdentifierStr;

  getNextToken(); // eat identifier.

  if (CurTok != '(') // Simple variable ref.
    return std::make_unique<VariableExprAst>(IdName);

  // Call.
  getNextToken(); // eat (
  std::vector<std::unique_ptr<ExprAst>> Args;
  if (CurTok != ')') {
    while (1) {
      if (auto Arg = ParseExpression())
        Args.push_back(std::move(Arg));
      else
        return nullptr;

      if (CurTok == ')')
        break;

      if (CurTok != ',')
        return LogError("Expected ')' or ',' in argument list");
      getNextToken();
    }
  }

  getNextToken(); // eat ).
  return std::make_unique<CallExprAst>(IdName, std::move(Args));
}

std::unique_ptr<ExprAst> ParsePrimary() {
  switch (CurTok) {
  default:
    return LogError("unknown token when expecting an expression");
  case tok_identifier:
    return ParseIdentifierExpr();
  case tok_number:
    return ParseNumberExpr();
  case '(':
    return ParseParenExpr();
  }
}

int GetTokPrecedence() {
  if (!isascii(CurTok))
    return -1;

  int TokPrec = BinopPrecedence[CurTok];
  if (TokPrec <= 0)
    return -1;
  return TokPrec;
}

std::unique_ptr<ExprAst> ParseBinOpRHS(int ExprPrec,
                                       std::unique_ptr<ExprAst> LHS) {
  while (1) {
    int TokPrec = GetTokPrecedence();

    if (TokPrec < ExprPrec)
      return LHS;

    int BinOp = CurTok;
    getNextToken(); // eat binop

    auto RHS = ParsePrimary();
    if (!RHS)
      return nullptr;

    int NextPrec = GetTokPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
      if (!RHS)
        return nullptr;
    }

    LHS =
        std::make_unique<BinaryExprAst>(BinOp, std::move(LHS), std::move(RHS));
  }
}

std::unique_ptr<ExprAst> ParseExpression() {
  auto LHS = ParsePrimary();
  if (!LHS)
    return nullptr;

  return ParseBinOpRHS(0, std::move(LHS));
}

std::unique_ptr<PrototypeAst> ParsePrototype() {
  if (CurTok != tok_identifier)
    return LogErrorP("Expected function name in prototype");

  std::string FnName = IdentifierStr;
  getNextToken();

  if (CurTok != '(')
    return LogErrorP("Expected '(' in prototype");

  std::vector<std::string> ArgNames;
  while (getNextToken() == tok_identifier)
    ArgNames.push_back(IdentifierStr);
  if (CurTok != ')')
    return LogErrorP("Expected ')' in prototype");

  getNextToken(); // eat ')'.

  return std::make_unique<PrototypeAst>(FnName, std::move(ArgNames));
}

std::unique_ptr<FunctionAst> ParseFn() {
  getNextToken(); // eat fn.
  auto Proto = ParsePrototype();
  if (!Proto)
    return nullptr;

  if (auto E = ParseExpression())
    return std::make_unique<FunctionAst>(std::move(Proto), std::move(E));
  return nullptr;
}

std::unique_ptr<PrototypeAst> ParseImport() {
  getNextToken(); // eat import.
  return ParsePrototype();
}

std::unique_ptr<FunctionAst> ParseTopLevelExpr() {
  if (auto E = ParseExpression()) {
    auto Proto = std::make_unique<PrototypeAst>("", std::vector<std::string>());
    return std::make_unique<FunctionAst>(std::move(Proto), std::move(E));
  }
  return nullptr;
}
