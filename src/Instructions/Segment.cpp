#include "Instruction.h"
#include "Segment.h"
#include "ProgramContext.h"
#include "Utilities.h"
#include <iomanip>
#include <cctype>

Segment::Segment(const string& s)
{
  ins = s;
  string next;
  if (s.substr(0,5)==".text")
  {
    next = s.substr(5);
  }
  else if (s.substr(0,5) == ".data")
  {
    next = s.substr(5);
  }
  else if (s.substr(0,4) == ".bss")
  {
    next =s.substr(4);
  }
  else
  {
    next = s.substr(7);
  }
  if (next != ""){
    if (next.at(0)!= '.')
      Utilities::error("Invalid syntax: " + s);
    for (int i=1;i<next.length();i++)
    {
      if(!isdigit(next.at(i)))
      {
        Utilities::error("Invalid syntax: " + s);
      }
    }
  }
  if (SymbolTable::getSymbolTable()->currentSegment!="")
  {
    SourceCode::endSegmentFirstPass();
  }
  SymbolTable::getSymbolTable()->currentLocation = 0;
  SymbolTable::addSegmentIfNotExists(s);
  if (SymbolTable::getSymbolTable()->isOrg)
  {
    SymbolTable::getSymbolTable()->isOrg=false;
    OrgVal* o = new OrgVal;
    o->s = s;
    o->e = SymbolTable::getSymbolTable()->orgExpr;
    SymbolTable::getSymbolTable()->orgs.push_back(o);
  }
  SymbolTable::getSymbolTable()->currentSegment = s;
}

void Segment::secondPass()
{
  if (SymbolTable::getSymbolTable()->currentSegment!="")
  {
    SourceCode::endSegmentSecondPass();
  }
  SymbolTable::getSymbolTable()->currentSegment = ins;
  SymbolTable::getSymbolTable()->currentLocation = 0;
}

Segment::~Segment()
{
  
}
