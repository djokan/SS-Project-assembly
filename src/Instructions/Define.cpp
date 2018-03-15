#include "Instruction.h"
#include "Expr.h"
#include "Operand.h"
#include "Define.h"
#include "Utilities.h"

Define::Define(string e)
{
  if (SymbolTable::getSymbolTable()->currentSegment=="")
  {
    Utilities::error("Symbol definition must be in segment");
  }
  if (e.find("$") != string::npos)
  {
    string rand;
    while (1)
    {
      rand = Utilities::generateRandomString(10);
      rand = "##" + rand;
      if (SymbolTable::getSymbolTable()->table.find(rand) == SymbolTable::getSymbolTable()->table.end())
      {
        SymbolTable::addLabelIfNotExists(rand);
        e= Utilities::replaceAll(e,"$",rand);
        break;
      }
    }
  }
  
  string sym = Utilities::getFirstWord(e);
  if (sym=="DD") size=4;
  if (sym=="DW") size=2;
  if (sym=="DB") size=1;
  
  e = Utilities::removeFirstWord(e);
  int dup;
  if ((dup=e.find("DUP"))!=string::npos)
  {
    number = Expr::parseExpr(e.substr(0,dup));
    string after = e.substr(dup+3);
    if (Utilities::getFirstWord(after)== "?")
    {
      data=Expr::parseExpr("0");
    }
    else
    {
      data= Expr::parseExpr(after);
    }
  }
  else
  {
    number = Expr::parseExpr("1");
    if (Utilities::getFirstWord(e)== "?")
    {
      data = Expr::parseExpr("0");
    }
    else
    {
      data = Expr::parseExpr(e);
    }
  }
  DataVal* d = new DataVal;
  d->number = number;
  d->adr = SymbolTable::getSymbolTable()->currentLocation;
  d->segment = SymbolTable::getSymbolTable()->currentSegment;
  d->bytes = size;
  SymbolTable::getSymbolTable()->datas.push_back(d);
  SymbolTable::getSymbolTable()->currentLocation++;
}

void Define::secondPass()
{
  if (!data->isConstant() || !number->isConstant())
  {
    Utilities::error("Value in definition must be constant!");
  }
  unsigned int val = data->value();
  unsigned int num = number->value();
  if (size==1)
  {
    unsigned char b = val & 0xFF;
    for(int i=0;i<num;i++)
      SourceCode::put(b);
  }
  if (size==2)
  {
    unsigned char b = val & 0xFF;
    unsigned char b1 = (val>>8)&0xFF;
    for(int i=0;i<num;i++)
    {
      SourceCode::put(b);
      SourceCode::put(b1);
    }
  }
  if (size==4)
  {
    for(int i=0;i<num;i++)
      Utilities::createInstruction(val);
  }
}

Define::~Define()
{
  
}
