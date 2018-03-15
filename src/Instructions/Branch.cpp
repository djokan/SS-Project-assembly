#include <sstream>
#include "Operand.h"
#include "Branch.h"
#include "Utilities.h"
#include "ProgramContext.h"

Branch::Branch(const string& s, unsigned char opcode)
{
  if (SymbolTable::getSymbolTable()->currentSegment=="")
  {
    Utilities::error("Instruction must be defined in segment");
  }
  code = opcode;
  if (opcode>3)
  {
    Utilities::removeSpaces(s);
    std::istringstream ss(s);
    std::string temp;
    string firstop,secondop,thirdop;
    if (!getline(ss, firstop, ','))
    {
      Utilities::error("invalid syntax: "+ s);
    }
    if (!getline(ss, secondop, ','))
    {
      Utilities::error("invalid syntax: "+ s);
    }
    if (getline(ss, thirdop, ','))
    {
      Utilities::error("invalid syntax: "+ s);
    }
    op = new Operand* [2];
    op[0] = Operand::stringToOperand(firstop);
    op[1] = Operand::stringToOperand(secondop);
    if (op[0]->type!=Operand::REGDIR || (op[1]->type!=Operand::MEMDIR && op[1]->type!=Operand::REGIND && op[1]->type!=Operand::REGINDPOM && op[1]->type !=Operand::PCREL))
    {
      Utilities::error("Invalid operand types: " + s);
    }
    SymbolTable::getSymbolTable()->currentLocation += op[1]->size();
  }
  else if (opcode==1)
  {
    if (s!="")
    {
      Utilities::error("Invalid syntax on return statement");
    }
    SymbolTable::getSymbolTable()->currentLocation += 4;
  }
  else if (opcode==0)
  {
    cout << "ok" << endl;
    if (s.find(",")!=string::npos)
      Utilities::error("Invalid syntax: "+ s);
    op = new Operand*[1];
    op[0] = Operand::stringToOperand(s);
    if (op[0]->type !=Operand::REGDIR)
    {
      Utilities::error("Invalid operand type " + s);
    }
    SymbolTable::getSymbolTable()->currentLocation += op[0]->size();
  }
  else if (opcode==2 || opcode==3)
  {
    if (s.find(",")!=string::npos)
      Utilities::error("Invalid syntax: "+ s);
    op = new Operand*[1];
    op[0] = Operand::stringToOperand(s);
    if (op[0]->type !=Operand::MEMDIR && op[0]->type !=Operand::REGINDPOM && op[0]->type !=Operand::REGIND && op[0]->type !=Operand::PCREL)
    {
      Utilities::error("Invalid operand type " + s);
    }
    SymbolTable::getSymbolTable()->currentLocation += op[0]->size();
  }
}

void Branch::secondPass()
{
  if (code>3)
  {
    unsigned char c[4] = {
      code,
      op[1]->addressingCode(),
      op[1]->operandCode(),
      op[0]->operandCode()
    };
  
    int i[4] = {
      8,
      3,
      5,
      5
    };
    Utilities::createInstruction(c,i,4,4);
    op[1]->addRelocOperand();
  }
  if (code ==1)
  {
    unsigned char c[4] = {
      code
    };
  
    int i[4] = {
      8
    };
    Utilities::createInstruction(c,i,1,4);
    
  }
  if (code ==2 || code==3 || code == 0)
  {
    unsigned char c[4] = {
      code,
      op[0]->addressingCode(),
      op[0]->operandCode(),
      op[0]->operandCode()
    };
  
    int i[4] = {
      8,
      3,
      5,
      5
    };
    Utilities::createInstruction(c,i,4,4);
    op[0]->addRelocOperand();
  }  
  
}
