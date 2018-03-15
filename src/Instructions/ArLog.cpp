#include "ArLog.h"
#include "Operand.h"
#include "Utilities.h"
#include "Instruction.h"
#include <sstream>
#include "ProgramContext.h"
#include <iostream>
#include "Utilities.h"
using namespace std;

ArLog::ArLog(string& line, int ops, const unsigned char& opCode)
{
  if (SymbolTable::getSymbolTable()->currentSegment=="")
  {
    Utilities::error("Instruction must be defined in segment");
  }
  Utilities::removeSpaces(line);
  std::istringstream ss(line);
  std::string temp;
  this->ops = ops;
  string firstop,secondop,thirdop;
  if (!getline(ss, firstop, ','))
  {
    Utilities::error("invalid syntax: "+ line);
  }
  if (!getline(ss, secondop, ','))
  {
    Utilities::error("invalid syntax: "+ line);
  }
  if ((!getline(ss, thirdop, ',')) == (ops==3))
  {
    Utilities::error("invalid syntax: "+ line);
  }
  if ((ops==3) && getline(ss, temp, ','))
  {
    Utilities::error("invalid syntax: "+ line);
  }
  code = opCode;
  op = new Operand* [3];
  op[0] = Operand::stringToOperand(firstop);
  op[1] = Operand::stringToOperand(secondop);
  if (ops ==3) op[2] = Operand::stringToOperand(thirdop);
  if (op[0]->type != Operand::REGDIR || op[1]->type != Operand::REGDIR || op[2-(ops==3?0:1)]->type != Operand::REGDIR)
  {
    Utilities::error("All three operands of Mov instruction must be RegDir address modes!");
  }
  SymbolTable::getSymbolTable()->currentLocation+= op[0]->size();
}


void ArLog::secondPass()
{
  unsigned char t= ops==3?op[2]->operandCode():0x00;
  unsigned char c[5] = {
    code,
    op[0]->addressingCode(),
    op[0]->operandCode(),
    op[1]->operandCode(),
    t
  };
  
  int i[5] = {
    8,
    3,
    5,
    5,
    5
  };

  Utilities::createInstruction(c,i,5,4);
}

ArLog::~ArLog()
{
  delete op[0];
  delete op[1];  
  if (ops==3)
  {
    delete op[2];
  }
  delete op;
}
