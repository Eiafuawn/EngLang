#include "ast.cpp"
#include "lexer.cpp"
#include <map>

Lexer lexer;

auto LHS = std::make_unique<VariableExprAst>("x");
auto RHS = std::make_unique<VariableExprAst>("y");

// Forward Declarations
static std::unique_ptr<ExprAst> ParseExpression();

// Utility Functions
static int getNextToken();
std::unique_ptr<ExprAst> LogError(const char *Str);
std::unique_ptr<PrototypeAst> LogErrorP(const char *Str);

// Parsing Functions
static std::unique_ptr<ExprAst> ParsePrimary();
static std::unique_ptr<ExprAst> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAst> LHS);
static std::unique_ptr<ExprAst> ParseParenExpr();
static std::unique_ptr<ExprAst> ParseNumberExpr();
static std::unique_ptr<ExprAst> ParseIdentifierExpr();
static int GetTokPrecedence();

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

static std::unique_ptr<ExprAst> ParseIdentifierExpr() {
  std::string IdName = lexer.IdentifierStr;

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

static std::unique_ptr<ExprAst> ParsePrimary() {
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

/// BinopPrecedence
static std::map<char, int> BinopPrecedence;

static int GetTokPrecedence() {
  if (!isascii(CurTok))
    return -1;

  int TokPrec = BinopPrecedence[CurTok];
  if (TokPrec <= 0)
    return -1;
  return TokPrec;
}

// Install binary operator 0 is the lowest precedence.
// https://en.wikipedia.org/wiki/Operator-precedence_parser
int main() {
  BinopPrecedence['<'] = 10;
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence['*'] = 40;
  BinopPrecedence['/'] = 40;
}

/// binopr
static std::unique_ptr<ExprAst> ParseBinOpRHS(int ExprPrec,
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

static std::unique_ptr<ExprAst> ParseExpression() {
  auto LHS = ParsePrimary();
  if (!LHS)
    return nullptr;

  return ParseBinOpRHS(0, std::move(LHS));
}

static std::unique_ptr<PrototypeAst> ParsePrototype() {
  if (CurTok != tok_identifier)
    return LogErrorP("Expected function name in prototype");

  std::string FnName = lexer.IdentifierStr;
  getNextToken();

  if (CurTok != '(')
    return LogErrorP("Expected '(' in prototype");

  std::vector<std::string> ArgNames;
  while (getNextToken() == tok_identifier)
    ArgNames.push_back(lexer.IdentifierStr);
  if (CurTok != ')')
    return LogErrorP("Expected ')' in prototype");

  getNextToken(); // eat ')'.

  return std::make_unique<PrototypeAst>(FnName, std::move(ArgNames));
}

static std::unique_ptr<FunctionAst> ParseDefinition() {
  getNextToken(); // eat def.
  auto Proto = ParsePrototype();
  if (!Proto)
    return nullptr;

  if (auto E = ParseExpression())
    return std::make_unique<FunctionAst>(std::move(Proto), std::move(E));
  return nullptr;
}

static std::unique_ptr<PrototypeAst> ParseImport() {
  getNextToken(); // eat import.
  return ParsePrototype();
}

static std::unique_ptr<FunctionAst> ParseTopLevelExpr() {
  if (auto E = ParseExpression()) {
    auto Proto = std::make_unique<PrototypeAst>("", std::vector<std::string>());
    return std::make_unique<FunctionAst>(std::move(Proto), std::move(E));
  }
  return nullptr;
}
