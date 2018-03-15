#ifndef REGINDPOM_H
#define REGINDPOM_H
#include "Operand.h"
#include <iostream>

using namespace std;

class RegIndPom: public Operand{
public:
  unsigned int pom;
  unsigned char code;
  Expr* ex;
  RegIndPom(const string& s);
  unsigned char addressingCode();
  unsigned char operandCode();
  void addRelocOperand();
  int size();
};
#endif /* REGINDPOM_H */
