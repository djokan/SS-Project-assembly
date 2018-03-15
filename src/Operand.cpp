#include "Operand.h"
#include "Utilities.h"
#include <iostream>
#include "Imm.h"
#include "RegDir.h"
#include "RegInd.h"
#include "RegIndPom.h"
#include "MemDir.h"
#include "PcRel.h"
using namespace std;

const int Operand::IMM =  1;
const int Operand::REGDIR =  2;
const int Operand::REGIND =  4;
const int Operand::MEMDIR =  8;
const int Operand::REGINDPOM =  16;
const int Operand::PCREL =  32;

Operand::Operand(const int& type)
{
  this->type = type;
}

Operand* Operand::stringToOperand(const string& op1){
    string op = Utilities::removeSpaces(op1);
    if (op.at(0)=='#')
    {
      return new Imm(op.substr(1));
    }
    if (op.at(0)=='$')
    {
      return new PcRel(op.substr(1));
    }
    if (op == "R0" || op == "R1" || op == "R2" || op == "R3" || op == "R4" || op == "R5" || op == "R6" || op == "R7" || op == "R8" || op == "R9" || op == "R10" || op == "R11" || op == "R12" || op == "R13" || op == "R14" || op == "R15" || op == "PC" || op == "SP" )
    {
      return new RegDir(op);
    }
    if (op == "[R0]" || op == "[R1]" || op == "[R2]" || op == "[R3]" || op == "[R4]" || op == "[R5]" || op == "[R6]" || op == "[R7]" || op == "[R8]" || op == "[R9]" || op == "[R10]" || op == "[R11]" || op == "[R12]" || op == "[R13]" || op == "[R14]" || op == "[R15]" || op == "[PC]" || op == "[SP]" )
    {
      return new RegInd(op.substr(1,op.length()-2));
    }
    if ((op.substr(0,4) == "[R0+" || op.substr(0,4) == "[R1+" || op.substr(0,4) == "[R2+" || op.substr(0,4) == "[R3+" || op.substr(0,4) == "[R4+" || op.substr(0,4) == "[R5+" || op.substr(0,4) == "[R6+" || op.substr(0,4) == "[R7+" || op.substr(0,4) == "[R8+" || op.substr(0,4) == "[R9+" || op.substr(0,5) == "[R10+" || op.substr(0,5) == "[R11+" || op.substr(0,5) == "[R12+" || op.substr(0,5) == "[R13+" || op.substr(0,5) == "[R14+" || op.substr(0,5) == "[R15+" || op.substr(0,4) == "[PC+" || op.substr(0,4) == "[SP+" ) && op.substr(op.length()-1)== "]")
    {
      return new RegIndPom(op.substr(1,op.length()-2));
    }
    return new MemDir(op);
}

int Operand::getType()
{
  return type;
}
