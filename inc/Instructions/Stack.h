#include "Instruction.h"

#include "Operand.h"

class Stack: public Instruction{
public:
  string ins;
  unsigned char code;
  Operand* op;
  Stack(const string&,const unsigned char&);
  void secondPass();
  virtual ~Stack();
};
