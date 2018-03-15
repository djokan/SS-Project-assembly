#ifndef PCREL_H
#define PCREL_H
#include "Operand.h"
#include "Expr.h"

class PcRel: public Operand {
protected:
  Expr* ex;
public:
  PcRel(const string& s);
  unsigned char addressingCode();
  unsigned char operandCode();
  void addRelocOperand();
  int size();
};


#endif /* PCREL_H */
