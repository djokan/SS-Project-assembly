#ifndef MEMDIR_H
#define MEMDIR_H
#include "Operand.h"
#include "Expr.h"

class MemDir: public Operand {
public:
  Expr* ex;
  MemDir(const string& s);
  unsigned char operandCode();
  unsigned char addressingCode();
  void addRelocOperand();
  int size();
};


#endif /* MEMDIR_H */
