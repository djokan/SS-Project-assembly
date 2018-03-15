#ifndef ARLOG_H
#define ARLOG_H
#include <iostream>
#include "Instruction.h"
#include "Operand.h"

using namespace std;

class ArLog: public Instruction{
private:
  Operand **op;
  string line;
  char code;
  int ops;
public:
  ArLog(string&, int ,const unsigned char&);
  void secondPass();
  ~ArLog();
};

#endif /* ARLOG_H */
