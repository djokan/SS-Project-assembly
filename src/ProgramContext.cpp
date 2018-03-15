#include "ProgramContext.h"
#include "Utilities.h"

map <string, list<RelocationInstance*>*> RelocationTable::tables ;
SymbolTable* SymbolTable::myST = nullptr;
SourceCode* SourceCode::mySource = nullptr;
std::list<SymbolWrite*> SourceCode::writeAfter;


void SymbolTable::addSegmentIfNotExists(const string& s)
{
  if (getSymbolTable()->table.find(s) == getSymbolTable()->table.end())
  {
    getSymbolTable()->table[s] = new Symbol(0,s,s,0,1);
    getSymbolTable()->table[s]->validValue=true;
    setGlobal(s,true);
  }
  else
  {
    if (getSymbolTable()->table[s]->type!=0 && getSymbolTable()->table[s]->segment!="global")
    {
      Utilities::error("Multiple declaration of segment " + s);
    }
    else
    {
      getSymbolTable()->table[s]->value = 0;
      getSymbolTable()->table[s]->type = 1;
      getSymbolTable()->table[s]->segment = s;
      getSymbolTable()->table[s]->validValue = true;
    }
  }
}

void SymbolTable::addLabelIfNotExists(const string& s)
{
  if (getSymbolTable()->table.find(s) == getSymbolTable()->table.end())
  {
    getSymbolTable()->table[s] = new Symbol(0,s,getSymbolTable()->currentSegment,getSymbolTable()->currentLocation,2);
    getSymbolTable()->table[s]->validValue = true;
  }
  else
  {
    if (getSymbolTable()->table[s]->type!=0 && getSymbolTable()->table[s]->segment!="global")
    {
      Utilities::error("Multiple declaration of symbol " + s);
    }
    else
    {
      getSymbolTable()->table[s]->type = 2;
      getSymbolTable()->table[s]->value = getSymbolTable()->currentLocation;
      getSymbolTable()->table[s]->segment = getSymbolTable()->currentSegment;
      getSymbolTable()->table[s]->validValue = true;
    }
  }
}

void SymbolTable::addGlobalIfNotExists(const string& s)
{
  
  if (getSymbolTable()->table.find(s) == getSymbolTable()->table.end())
  {
    getSymbolTable()->table[s] = new Symbol(0,s,"global",0,2);
    getSymbolTable()->table[s]->validValue = true;
    getSymbolTable()->table[s]->global = true;
  }
  else
  {
    if (getSymbolTable()->table[s]->type ==0)
      getSymbolTable()->table[s]->type = 2;
    getSymbolTable()->table[s]->global = true;
  }
}



void SymbolTable::setGlobal(const string& a,bool b)
{
  if (!(getSymbolTable()->table.find(a) == getSymbolTable()->table.end()))
  {
    getSymbolTable()->table[a]->global = b;
  }
}

void SymbolTable::addConstantIfNotExists(const string& s)
{
  if (getSymbolTable()->table.find(s) == getSymbolTable()->table.end())
  {
    getSymbolTable()->table[s] = new Symbol(0,s,getSymbolTable()->currentSegment,0,3);
  }
  else
  {
    if (getSymbolTable()->table[s]->type!=0 && getSymbolTable()->table[s]->segment!="global")
    {
      Utilities::error("Multiple declaration of symbol: "+ s);
    }
    else
    {
      getSymbolTable()->table[s]->type = 3;
      getSymbolTable()->table[s]->segment = getSymbolTable()->currentSegment;
    }
  }
}

void SymbolTable::addUnknownSymbolIfNotExists(const string& s)
{
  if (getSymbolTable()->table.find(s) == getSymbolTable()->table.end())
  {
    getSymbolTable()->table[s] = new Symbol(0,s,getSymbolTable()->currentSegment,0,0);
  }
}

SymbolTable::SymbolTable()
{
  ended = false;
  isOrg = false;
  currentLocation = 0;
  currentSegment = "";
}

SymbolTable* SymbolTable::getSymbolTable()
{
  if (myST==nullptr)
    myST = new SymbolTable();
  return myST;
}

Symbol::Symbol(int a, string b, string c, int d,int e)
{
  number = a;
  name = b;
  segment = c;
  value = d;
  type = e;
  global = false;
  validValue = false;
  myEx= nullptr;
}

void RelocationTable::add(RelocationInstance* i)
{
  tables[SymbolTable::getSymbolTable()->currentSegment]->push_back(i);
}

void SymbolTable::print()
{
  cout << "Table of Symbols:" << endl;
  for(auto const s1 : getSymbolTable()->table)
  {
    Symbol* s = s1.second;
    string temp= s->global?"yes":"no";
    cout << "Name: " << s->name << ", Segment: " << s->segment << ", Value " << s->value << ", type: " << s->type << ", global: " << temp  << ",Number: " << s->number << endl;
  }
}
void SymbolTable::checkAndNumberTable()
{
  int iter=1;
  for (auto const a : getSymbolTable()->table)
  {
    if (a.second->type==0)
    {
      Utilities::error("Symbol " + a.second->name + " is undefined");
    }
    if (a.second->type==1)
    {
      a.second->number= iter++;
    }
  }
  for (auto const a : getSymbolTable()->table)
  {
    if (a.second->type!=1)
    {
      a.second->number= iter++;
    }
  }
  
}

Symbol* SymbolTable::getSymbol(const string& s)
{
  if (getSymbolTable()->table.find(s) == getSymbolTable()->table.end())
  {
    Utilities::error("Symbol " + s + " doesn't exist");
  }
  Symbol* sy = getSymbolTable()->table[s];
  return sy;
}

SourceCode::SourceCode()
{
  currentLocation = 0;
}

SourceCode* SourceCode::getMySource()
{
  if (mySource == nullptr)
    mySource = new SourceCode();
  return mySource;
}

void SourceCode::put(const unsigned char& byte)
{
  SymbolTable::getSymbolTable()->currentLocation++;
  getMySource()->segmentCodes[SymbolTable::getSymbolTable()->currentSegment]->code[(getMySource()->currentLocation)++] = byte;
}

void SourceCode::print()
{
  int i=0;
  cout << "Source codes:" << endl;
  for (auto const source: getMySource()->segmentCodes)
  {
    cout << "Segment: " <<  source.first << endl;
    cout << "BIN:" << endl;
    for (i=0;i<source.second->length;i++)
    {
      printf("%s ",(Utilities::byte_to_binary(source.second->code[i])));
    }
    cout << endl << "HEX:" << endl;
    for (i=0;i<source.second->length;i++)
    {
      if (source.second->code[i]<16)
        printf("0");
      printf("%x ",(source.second->code[i]));
    }
    cout << endl;
  }
  
}

SymbolTable::~SymbolTable()
{
  for (auto const a : getSymbolTable()->table)
  {    
    delete (getSymbolTable()->table[a.second->name]);
  }
}

SymbolWrite::SymbolWrite(Symbol *s,int adr,Expr* e)
{
  this->s = s;
  this->address = adr;
  this->expr = e;
}

void SourceCode::writeAfterSymbols()
{
  int size = writeAfter.size();
  int iter =0;
  int wrote = 0;
  bool l=false;
  int loc = SourceCode::getMySource()->currentLocation;
  for (int i=0;i<size;i++)
  {
    SymbolWrite *w = writeAfter.front();
    writeAfter.pop_front();
    SourceCode::getMySource()->currentLocation= w->address;
    if (w->calcValue()==true)
    {
      wrote++;
      i=0;
      SourceCode::write(w->s->value);
    }
  }
  SourceCode::getMySource()->currentLocation= loc;
  if (wrote!=size)
  {
    Utilities::error("Some symbols can't be calculated");
  }
}

void SourceCode::write(unsigned int a)
{
  Utilities::createInstruction(a);
}

bool SymbolWrite::calcValue()
{
  if (this->s->validValue)
    return false;
  while (this->s->iAmDependingOn.size()>0)
  {
    Symbol *s = this->s->iAmDependingOn.front();
    this->s->iAmDependingOn.pop_front();
    if (s->validValue == false)
    {
      return false;
    }
  }
  Symbol* s = this->s;
  s->value = expr->value();
  s->validValue = true;
  return true;
}

RelocationInstance::RelocationInstance(int adr, bool absolute , int number)
{
  this->adr = adr;
  this->absolute = absolute;
  this->number = number;
}

void RelocationTable::print()
{
  cout << "RelocTable: " << endl;
  RelocationInstance* r;
  for (auto const a: tables)
  {
    cout << "Segment: " << a.first << endl;
    for (int i=0;i<a.second->size();i++)
    {
      r= a.second->front();
      a.second->pop_front();
      a.second->push_back(r);
      string t = r->absolute?"yes":"no";
      cout << "adr:" <<  r->adr <<", absolute: " << t << ", number: " << r->number << endl;
    }
    cout << endl;
  }
}

void SourceCode::endSegmentFirstPass()
{
  getMySource()->segmentCodes[SymbolTable::getSymbolTable()->currentSegment] = new SegmentCode;
  getMySource()->segmentCodes[SymbolTable::getSymbolTable()->currentSegment]->length = SymbolTable::getSymbolTable()->currentLocation;
  RelocationTable::tables[SymbolTable::getSymbolTable()->currentSegment] = new list<RelocationInstance*>();
}

void SourceCode::endSegmentSecondPass()
{
  getMySource()->currentLocation=0;
}

