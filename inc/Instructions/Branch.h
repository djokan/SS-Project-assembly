#ifndef BRANCH_H
#define BRANCH_H

#include "Instruction.h"
#include <iostream>

class Operand;

using namespace std;
class Branch: public Instruction{
public:
  Branch(const string &s,unsigned char );
  Operand** op;
  unsigned char code;
  void secondPass();
};


#endif /* BRANCH_H */
