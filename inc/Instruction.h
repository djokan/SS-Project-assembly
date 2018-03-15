#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "Operand.h"
#include "Expr.h"
#include "ProgramContext.h"
#include<iostream>

using namespace std;

class Instruction{
protected:
  string i;
 public:
  static Instruction* stringToInstruction(string line);
  virtual void secondPass() =0;
  Instruction();
  virtual ~Instruction();
};

#endif /* INSTRUCTION_H */
