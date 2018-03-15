#include "Utilities.h"
#include "RegIndPom.h"
#include "RegDir.h"
#include "Expr.h"
#include "ProgramContext.h"

RegIndPom::RegIndPom(const string& s):Operand(Operand::REGINDPOM)
{
  int i = s.find("+");
  int p = s.find("]");
  string dig = s.substr(0,i);
  code = RegDir::regToOpCode(dig);
  ex = Expr::parseExpr(s.substr(i+1));
}


unsigned char RegIndPom::addressingCode()
{
  return 7;
}

unsigned char RegIndPom::operandCode()
{
  return code;
}


void RegIndPom::addRelocOperand()
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
int RegIndPom::size()
{
  return 8;
}
