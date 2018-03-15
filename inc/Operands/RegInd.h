#ifndef REGIND_H
#define REGIND_H

#include "Operand.h"
#include <iostream>

using namespace std;
class RegInd: public Operand
{
public:
  string reg;
  RegInd(const string&);
  unsigned char operandCode();
  unsigned char addressingCode();
  void addRelocOperand();
  int size();
};

#endif /* REGIND_H */
