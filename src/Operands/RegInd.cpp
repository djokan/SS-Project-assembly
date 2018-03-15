#include "Utilities.h"
#include "RegInd.h"
#include "Operand.h"
#include "RegDir.h"

RegInd::RegInd(const string &s):Operand(Operand::REGIND)
{
  reg = s;
}


unsigned char RegInd::operandCode()
{
  return RegDir::regToOpCode(reg);
}
unsigned char RegInd::addressingCode()
{
  return 2;
}

void RegInd::addRelocOperand()
{
  
}
int RegInd::size()
{
  return 4;
}
