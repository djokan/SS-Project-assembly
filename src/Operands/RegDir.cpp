#include "RegDir.h"
#include "Utilities.h"

RegDir::RegDir(const string& s):Operand(Operand::REGDIR)
{
  reg = s;
}



unsigned char RegDir::addressingCode()
{
  return 0;
}
unsigned char RegDir::operandCode()
{
  return regToOpCode(reg);
}

unsigned char RegDir::regToOpCode(const string& reg)
{
  if (reg.at(0)=='R')
  return stoi(reg.substr(1));
  if (reg =="SP")
  {
    return 0x10;
  }
  return 0x11;
}


void RegDir::addRelocOperand()
{
  
}
int RegDir::size()
{
  return 4;
}
