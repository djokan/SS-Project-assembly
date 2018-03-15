#ifndef DEFINE_H
#define DEFINE_H

#include "Expr.h"
#include "Instruction.h"
#include <iostream>

using namespace std;

class Define : public Instruction{
public:
  Expr* data;
  Expr* number;
  int size;
  Define(string);
  void secondPass();
  ~Define();
};


#endif /* DEFINE_H */
