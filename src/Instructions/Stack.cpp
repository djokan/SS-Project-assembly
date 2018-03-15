#include "Stack.h"
#include "Utilities.h"
#include "ProgramContext.h"
Stack::Stack(const string& ins,const unsigned char& c)
{
  if (SymbolTable::getSymbolTable()->currentSegment=="")
  {
    Utilities::error("Instruction must be defined in segment");
  }
  this->ins = ins;
  code = c;
  op = Operand::stringToOperand(ins);
  if (op->type != Operand::REGDIR)
  {
    Utilities::error("Type must be RegDir");
  }
  SymbolTable::getSymbolTable()->currentLocation+= op->size();
}

void Stack::secondPass()
{
  char b2;
  unsigned char c[5] = {
    code, 
    op->addressingCode(),
    op->operandCode()
  };
  
  int i[5] = {
    8,
    3,
    5
  };

  Utilities::createInstruction(c,i,3,4);
}

Stack::~Stack()
{
  delete op;
}
