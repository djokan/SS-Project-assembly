#include <iostream>
#include "Utilities.h"
#include "Expr.h"
#include <sstream>
#include <cstdlib>
#include "ProgramContext.h"
#include <algorithm>
using namespace std;

string Utilities::getFirstWord(const string &line)
{
  stringstream iss(line);
  string word;
  if (!(iss >> word))
    return "";
  return word;
}

string Utilities::getSecondWord(const string &line)
{
  stringstream iss(line);
  string word;
  if (!(iss >> word))
    return "";
  if (!(iss >> word))
    return "";
  return word;
}
string Utilities::removeSpaces(const string &s)
{
  stringstream iss(s);
  string word,news;
  while ((iss >> word))
    news += word;
  return news;
}
string Utilities::removeFirstWord(const string &s)
{
  if (Utilities::getFirstWord(s) == "")
  {
    return "";
  }
  stringstream iss(s);
  string word,news="";
  iss >> word;
  while ((iss >> word))
    news += word + " ";
  return news;
  
}
void Utilities::error(string s)
{
  cout << s << endl;
  exit(1);
}
bool Utilities::containsNumber(const std::string &c)
{
    if (c.find('0') != std::string::npos ||
        c.find('1') != std::string::npos ||
        c.find('2') != std::string::npos ||
        c.find('3') != std::string::npos ||
        c.find('4') != std::string::npos ||
        c.find('5') != std::string::npos ||
        c.find('6') != std::string::npos ||
        c.find('7') != std::string::npos ||
        c.find('8') != std::string::npos ||
        c.find('9') != std::string::npos)
    {
        return true;
    }

    return false;
}
bool Utilities::containsLetter(const std::string &c)
{
    if (c.find('a') != std::string::npos ||
        c.find('b') != std::string::npos ||
        c.find('c') != std::string::npos ||
        c.find('d') != std::string::npos ||
        c.find('e') != std::string::npos ||
        c.find('f') != std::string::npos ||
        c.find('g') != std::string::npos ||
        c.find('h') != std::string::npos ||
        c.find('i') != std::string::npos ||
        c.find('j') != std::string::npos ||
        c.find('k') != std::string::npos ||
        c.find('l') != std::string::npos ||
        c.find('m') != std::string::npos ||
        c.find('n') != std::string::npos ||
        c.find('o') != std::string::npos ||
        c.find('p') != std::string::npos ||
        c.find('q') != std::string::npos ||
        c.find('r') != std::string::npos ||
        c.find('s') != std::string::npos ||
        c.find('t') != std::string::npos ||
        c.find('u') != std::string::npos ||
        c.find('v') != std::string::npos ||
        c.find('w') != std::string::npos ||
        c.find('x') != std::string::npos ||
        c.find('y') != std::string::npos ||
        c.find('z') != std::string::npos)
    {
        return true;
    }

    return false;
}


int Utilities::isValidExpr(const Expr* t)
{
  
}

void Utilities::createInstruction(const unsigned char* c, const int* a, const int& n, const int& b)
{
  if (SourceCode::getMySource()->currentLocation==-1)
    return;
  int bpos=0;
  int ipos=8;
  int cpos;
  unsigned char cur = 0x00;
  for(int i=0;i<n;i++)
  {
    cpos = a[i];
    while (bpos<b)
    {
      if (cpos>ipos)
      {
        cur |= ((int)(c[i]>>(cpos-ipos))) & (0xFF>>(8-ipos));
        cpos -= ipos;
        ipos = 8;
        bpos++;
        SourceCode::put(cur);
        cur = 0x00;
        if (bpos == b) break;
        continue;
      }
      if (cpos==ipos)
      {
        
        cur |= ((int)c[i]) & (0xFF>>8-ipos);
        ipos = 8;
        bpos++;
        SourceCode::put(cur);
        cur = 0x00;
        break;
      }
      if (cpos<ipos)
      {
        cur |= ((int)(c[i]<<(ipos-cpos))) & ((0xFF >>(8-ipos))&(0xFF<<(ipos-cpos)));
        ipos -= cpos;
        break;
      }
    }
  }
  while (bpos < b)
  {
    SourceCode::put(cur);
    bpos++;
  }
}

const char* Utilities::byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

void Utilities::createInstruction(const unsigned int& a)
{
  if (SourceCode::getMySource()->currentLocation==-1)
    return;  
  unsigned char c= (a)&0xFF;
  SourceCode::put(c);
  c= (a>>8)&0xFF;
  SourceCode::put(c);
  c= (a>>16)&0xFF;
  SourceCode::put(c);
  c= (a>>24)&0xFF;
  SourceCode::put(c);
}

unsigned int Utilities::doOp(const unsigned int& a,const unsigned int& b, const int& op)
{
  switch(op)
  {
    case 0:
      return a+b;
    case 1:
      return a-b;
    case 2:
      return a*b;
    case 3:
      return a/b;
  }
  string s = to_string(op);
  Utilities::error("Operation "+ s + " does not exist");
}

void Utilities::betweenPass()
{
  SymbolTable::checkAndNumberTable();
  int size2;
  size2 = SymbolTable::getSymbolTable()->datas.size();
  DataVal* dd;
  for (int i=0;i<size2;i++)
  {
    dd=SymbolTable::getSymbolTable()->datas.front();
    SymbolTable::getSymbolTable()->datas.pop_front();
    if (dd->number->isConstant()==false)
    {
      Utilities::error("Data expression number must be constant" + dd->number->text);
    }
    unsigned int val = dd->number->value();
    SourceCode::getMySource()->segmentCodes[dd->segment]->length += val*dd->bytes-1;
    for (auto const a: SymbolTable::getSymbolTable()->table)
    {
      if (a.second->segment==dd->segment && a.second->value>dd->adr && (a.second->type==1 || a.second->type==2))
      {
        a.second->value += dd->bytes*val-1;
      }
    }
    delete dd;
  }

  
  int size1;
  size1 = SymbolTable::getSymbolTable()->orgs.size();
  OrgVal* o;
  for (int i=0;i<size1;i++)
  {
    o=SymbolTable::getSymbolTable()->orgs.front();
    SymbolTable::getSymbolTable()->orgs.pop_front();
    if (o->e->isConstant()==false)
    {
      Utilities::error("ORG expression must be constant" + o->e->text);
    }
    Symbol* seg = SymbolTable::getSymbolTable()->table[o->s];
    unsigned int val = o->e->value();
    for (auto const a: SymbolTable::getSymbolTable()->table)
    {
      if (a.second->segment==o->s && a.second->type<3)
      {
        a.second->value += val;
        a.second->type = 3;
      }
    }
  }
  
  list<Symbol*> s;
  for (auto const a: SymbolTable::getSymbolTable()->table)
  {
    if (a.second->validValue==false && (a.second->myEx!=nullptr))
    {
       s.push_back(a.second);
    }
  }
  int size = s.size();
  int iter =0;
  int wrote = 0;
  bool l=false;
  Symbol *w;
  while (s.size()!=0)
  {
    w = s.front();
    iter++;
    if (iter==size+1)
    {
      Utilities::error("Some expressions cannot be calculated: " + w->myEx->text);
    }
    s.pop_front();
    w->value = w->myEx->value();
    w->validValue = w->myEx->validValue;
    if (w->type==3 && !w->myEx->isConstant())
    {
      Utilities::error("Expression " + w->myEx->text + " must be constant");
    }
    if (!w->validValue)
    {
      s.push_back(w);
    }
    else
    {
      iter=0;
    }
  }
  SymbolTable::getSymbolTable()->currentLocation = 0;
}

string Utilities::generateRandomString(size_t length)
{
  const char* charmap = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const size_t charmapLength = strlen(charmap);
  auto generator = [&](){ return charmap[rand()%charmapLength]; };
  string result;
  result.reserve(length);
  generate_n(back_inserter(result), length, generator);
  return result;
}

string Utilities::replaceAll(string str, const string& from, const string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}
void Utilities::parseOrg(string s)
{
  Expr* e = Expr::parseExpr(s);
  SymbolTable::getSymbolTable()->orgExpr = e;
  SymbolTable::getSymbolTable()->isOrg = true;
}

void Utilities::parseGlobal(string s)
{
  
  std::istringstream ss(removeSpaces(s));
  std::string temp;
  string sym;
  while (getline(ss, sym, ','))
  {
    SymbolTable::addGlobalIfNotExists(sym);
  }
}

string Utilities::toHex(unsigned char a)
{
  string s="";
  unsigned char c=(a>>4)&0x0F;
  if (c<10)
  {
    s += to_string((int)(c));
  }
  if (c==10)
    s += "A";
  if (c==11)
    s += "B";
  if (c==12)
    s += "C";
  if (c==13)
    s += "D";
  if (c==14)
    s += "E";
  if (c==15)
    s += "F";
  c=(a)&0x0F;
  if (c<10)
  {
    s += to_string((int)(c));
  }
  if (c==10)
    s += "A";
  if (c==11)
    s += "B";
  if (c==12)
    s += "C";
  if (c==13)
    s += "D";
  if (c==14)
    s += "E";
  if (c==15)
    s += "F";
  return s;
}

string Utilities::toHex(unsigned int a)
{
  string s="";
  unsigned char c = (((unsigned char)(a>>24))&0xFF);
  string temp = Utilities::toHex(c);
  s += temp;
  c = (((unsigned char)(a>>16))&0xFF);
  temp = Utilities::toHex(c);
  s += temp;
  c = (((unsigned char)(a>>8))&0xFF);
  temp = Utilities::toHex(c);
  s += temp;
  c = (((unsigned char)a)&0xFF);
  temp = Utilities::toHex(c);
  s += temp;
  return s;
}
