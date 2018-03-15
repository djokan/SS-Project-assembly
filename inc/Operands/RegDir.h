#ifndef REGDIR_H
#define REGDIR_H

#include "Operand.h"
#include <iostream>

using namespace std;

class RegDir: public Operand{
public:
  string reg;
  RegDir(const string& s);
  unsigned char addressingCode();
  unsigned char operandCode();
  static unsigned char regToOpCode(const string &);
  void addRelocOperand();
  int size();
};
#endif /* REGDIR_H */
