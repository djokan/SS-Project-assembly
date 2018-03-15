#include "Utilities.h"
#include "MemDir.h"
#include <ctype.h>
#include <iomanip>
#include <cctype>
#include "ProgramContext.h"

MemDir::MemDir(const string& s): Operand(Operand::MEMDIR)
{
  ex = Expr::parseExpr(s);
}


unsigned char MemDir::operandCode()
{
  return 0x00;
}
unsigned char MemDir::addressingCode()
{
  return 6;
}

void MemDir::addRelocOperand()
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
int MemDir::size()
{
  return 8;
}
