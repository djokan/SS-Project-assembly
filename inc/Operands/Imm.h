#ifndef IMM_H
#define IMM_H
#include "Operand.h"
#include "Expr.h"

class Imm: public Operand {
protected:
  Expr* ex;
public:
  Imm(const string& s);
  unsigned char addressingCode();
  unsigned char operandCode();
  void addRelocOperand();
  int size();
};


#endif /* IMM_H */
