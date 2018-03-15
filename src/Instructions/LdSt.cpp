#include "LdSt.h"
#include "Utilities.h"
#include "Operand.h"
#include <sstream>
#include <iostream>
#include "ProgramContext.h"

LdSt::LdSt(const string &line,const unsigned char& opCode,const string& suf)
{
  if (SymbolTable::getSymbolTable()->currentSegment=="")
  {
    Utilities::error("Instruction must be defined in segment");
  }
  Utilities::removeSpaces(line);
  std::istringstream ss(line);
  std::string temp;
  string firstop,secondop,thirdop;
  if (opCode==0x10)    
  {
    type = 0x02;
    if ( suf== "ub")
      type= 0x03;
    if ( suf== "sb")
      type= 0x07;
    if ( suf== "uw")
      type= 0x01;
    if ( suf== "sw")
      type= 0x05;
    if ( suf== "")
      type= 0x00;
    if (type==0x02)
      Utilities::error("Unknown suffix " + suf + " instruction: " + line);
  }
  else
  {
    type = 0x02;
    if ( suf== "")
      type= 0x00;
    if ( suf== "b")
      type= 0x03;
    if ( suf== "w")
      type= 0x01;
    if (type == 0x02)
      Utilities::error("Unknown suffix " + suf+ " instruction: " + line);
  }
  
  if (!getline(ss, firstop, ','))
  {
    Utilities::error("invalid syntax: "+ line);
  }
  if (!getline(ss, secondop, ','))
  {
    Utilities::error("invalid syntax: "+ line);
  }
  if (getline(ss, thirdop, ','))
  {
    Utilities::error("invalid syntax: "+ line);
  }
  code = opCode;
  op = new Operand* [2];
  op[0] = Operand::stringToOperand(firstop);
  op[1] = Operand::stringToOperand(secondop);
  if (op[0]->type !=Operand::REGDIR || ((opCode == 0x11) && (op[1]->type==Operand::IMM)) )
  {
    Utilities::error("Invalid operand types: " + line);
  }
  SymbolTable::getSymbolTable()->currentLocation+= op[1]->size();
  if ((int)op[1]->addressingCode()==1)
  {
    cout <<"SSSSSSSSSSSSSSSSSSSSSSSSSSSSSS" <<  op[1]->type;
  }
}

void LdSt::secondPass()
{
  
  cout << "UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU" << (int)op[1]->addressingCode() << endl;
  unsigned char c[10] = {
    code,
    op[1]->addressingCode(),
    op[1]->operandCode(),
    op[0]->operandCode(),
    0x00,
    type
  };
  
  int i[10] = {
    8,
    3,
    5,
    5,
    5,
    3,
    2
  };
  Utilities::createInstruction(c,i,6,4);
  op[1]->addRelocOperand();
}

LdSt::~LdSt()
{
  delete op[1];
  delete op[0];
  delete op;
}
