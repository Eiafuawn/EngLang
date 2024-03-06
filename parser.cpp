#include "ast.cpp"
#include "lexer.cpp"

Lexer lexer;

auto LHS = std::make_unique<VariableExprAst>("x");
auto RHS = std::make_unique<VariableExprAst>("y");
auto Result =
    std::make_unique<BinaryExprAst>('+', std::move(LHS), std::move(RHS));

static int CurTok;
static int getNextToken() { return CurTok = lexer.getTok(); }

std::unique_ptr<ExprAst> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

std::unique_ptr<PrototypeAst> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

std::unique_ptr<ExprAst> ParseNumberExpr() {
  auto Result = std::make_unique<NumberExprAst>(lexer.NumVal);
  getNextToken();
  return std::move(Result);
}

static std::unique_ptr<ExprAst> ParseParenExpr() {
  getNextToken(); // eat (.
  auto V = ParseExpression();
  if (!V)
    return nullptr;

  if (CurTok != ')')
    return LogError("expected ')'");
  getNextToken(); // eat ).
  return V;
}
