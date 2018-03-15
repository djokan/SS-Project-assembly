#include "ProgramContext.h"
#include "Utilities.h"
#include "PcRel.h"
#include <ctype.h>
#include <iomanip>
#include <cctype>
#include "Operand.h"

PcRel::PcRel(const string& s): Operand(Operand::PCREL)
{
  ex = Expr::parseExpr(s);
}

unsigned char PcRel::addressingCode()
{
  return 0x07;
}
unsigned char PcRel::operandCode()
{
  return 0x11;
}

void PcRel::addRelocOperand()
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
      Symbol* seg = SymbolTable::getSymbolTable()->table[SymbolTable::getSymbolTable()->currentSegment];
      Symbol* seg1 = SymbolTable::getSymbolTable()->table[s->segment];
      if (seg->type==3)
      {
        Utilities::createInstruction(val-SymbolTable::getSymbolTable()->currentLocation-seg->value-4);
      }
      else
      {
        if (!s->global)
        {
          RelocationTable::add(new RelocationInstance(SymbolTable::getSymbolTable()->currentLocation,false,seg1->number));
          Utilities::createInstruction(val-4);
        }
        else
        {
          RelocationTable::add(new RelocationInstance(SymbolTable::getSymbolTable()->currentLocation,false,seg1->number));
          Utilities::createInstruction(val-s->value-4);
        }
      }
    }
    else
    {
      if (s->type==2 && s->segment == SymbolTable::getSymbolTable()->currentSegment)
      {
        Utilities::createInstruction(val-SymbolTable::getSymbolTable()->currentLocation-4);
      }
      else
      {
        if (s->global)
        {
          RelocationTable::add(new RelocationInstance(SymbolTable::getSymbolTable()->currentLocation,false,s->number));
          Utilities::createInstruction(val-s->value-4);
        }
        else
        {
          Symbol* seg = SymbolTable::getSymbolTable()->table[s->segment];
          RelocationTable::add(new RelocationInstance(SymbolTable::getSymbolTable()->currentLocation,false,seg->number));
          Utilities::createInstruction(val-4);
        }
      }
    }
  }
}

int PcRel::size()
{
  return 8;
}
