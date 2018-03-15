#ifndef OPERAND_H
#define OPERAND_H

class Expr;

#include <iostream>

using namespace std;

class Operand
{
public:
  int type;
  int getType();
  static const int IMM;
  static const int REGDIR;
  static const int REGIND;
  static const int MEMDIR;
  static const int REGINDPOM;
  static const int PCREL;
  Operand(const int& );
  static Operand* stringToOperand(const string& op);
  virtual unsigned char operandCode() =0;
  virtual unsigned char addressingCode() =0;
  virtual void addRelocOperand()=0;
  virtual int size() =0;
};



#endif /* OPERAND_H */
