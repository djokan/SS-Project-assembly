#include "Utilities.h"
#include "Imm.h"
#include <ctype.h>
#include <iomanip>
#include <cctype>
#include <iostream>
#include "ProgramContext.h"

using namespace std;
Imm::Imm(const string& s): Operand(Operand::IMM)
{
  ex = Expr::parseExpr(s);
}


unsigned char Imm::addressingCode()
{
  return 4;
}
unsigned char Imm::operandCode()
{
  
}

void Imm::addRelocOperand()
{
  unsigned int val = ex->value();
  Expr* e = ex->getTopLeftExpr();
  if (e->type==1)
  {
    Utilities::createInstruction(val);
  }
  else if (e->type==2)
  {
    Symbol* s = SymbolTable::getSymbolTable()->table[e->s];
    if (s->type == 3)
    {
      Utilities::createInstruction(val);
    }
    else
    {
      if (s->global)
      {
        RelocationTable::add(new RelocationInstance(SymbolTable::getSymbolTable()->currentLocation,true,s->number));
        Utilities::createInstruction(val-s->value);
      }
      else
      {
        Symbol* seg = SymbolTable::getSymbolTable()->table[s->segment];
        RelocationTable::add(new RelocationInstance(SymbolTable::getSymbolTable()->currentLocation,true,seg->number));
        Utilities::createInstruction(val);
      }
    }
  }
  
}
int Imm::size()
{
  return 8;
}
