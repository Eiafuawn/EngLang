#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>

class ExprAst {
public:
  virtual ~ExprAst() = default;
};

class NumberExprAst : public ExprAst {
  double Val;

public:
  NumberExprAst(double Val) : Val(Val) {}
};

class StringExprAst : public ExprAst {
  std::string Val;

public:
  StringExprAst(const std::string &Val) : Val(Val) {}
};

class VariableExprAst : public ExprAst {
  std::string Name;

public:
  VariableExprAst(const std::string &Name) : Name(Name) {}
};

class BinaryExprAst : public ExprAst {
  char Op;
  std::unique_ptr<ExprAst> LHS, RHS;

public:
  BinaryExprAst(char Op, std::unique_ptr<ExprAst> LHS,
                std::unique_ptr<ExprAst> RHS)
      : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

class CallExprAst : public ExprAst {
  std::string Callee;
  std::vector<std::unique_ptr<ExprAst>> Args;

public:
  CallExprAst(const std::string &Callee,
              std::vector<std::unique_ptr<ExprAst>> Args)
      : Callee(Callee), Args(std::move(Args)) {}
};

class PrototypeAst {
  std::string Name;
  std::vector<std::string> Args;

public:
  PrototypeAst(const std::string &Name, std::vector<std::string> Args)
      : Name(Name), Args(std::move(Args)) {}
  const std::string &getName() const { return Name; }
};

class FunctionAst {
  std::unique_ptr<PrototypeAst> Proto;
  std::unique_ptr<ExprAst> Body;

public:
  FunctionAst(std::unique_ptr<PrototypeAst> Proto,
              std::unique_ptr<ExprAst> Body)
      : Proto(std::move(Proto)), Body(std::move(Body)) {}
};

#endif // AST_H
