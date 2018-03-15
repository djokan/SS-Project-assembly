#include "Expr.h"
#include "Utilities.h"
#include <execinfo.h>
#include <sstream>
#include <exception>
#include <iomanip>
#include <cctype>
#include "ProgramContext.h"
#include "LdSt.h"

Symbol* Expr::depends= nullptr;
Expr* Expr::currentExpr = nullptr;

Expr::Expr(const string &s1,const int& i)
{
  checked = false;
  if (i==1)
  {
    currentExpr = this;
  }
  string s = s1;
  text = s;
  std::istringstream ss(s);
  std::string temp;
  string firstop,secondop;
  int pos = s.find('+');
  int cnt;
  scadd=false;
  while (pos!=string::npos)
  {
    cnt=0;
    for (int i=0;i<pos;i++)
    {
      if (s.at(i) == '(') cnt++;
      if (s.at(i) == ')') cnt--;
    }
    if (cnt!=0)
    {
      pos = s.find('+',pos+1);
      continue;
    }
    string t1;
    t1 = s.substr(0,pos);
    string t2;
    t2 = s.substr(pos+1,s.length()-pos-1);
    type = 0;
    op = 0;
    e = new Expr*[2];
    e[0] = new Expr(t1);
    e[1] = new Expr(t2);
    return;
  }
  pos = s.find('-');
  while (pos!=string::npos)
  {
    cnt = 0;
    for (int i=0;i<pos;i++)
    {
      if (s.at(i) == '(') cnt++;
      if (s.at(i) == ')') cnt--;
    }
    if (cnt!=0)
    {
      pos = s.find('-',pos+1);
      continue;
    }
    string t1;
    t1 = s.substr(0,pos);
    string t2;
    t2 = s.substr(pos+1,s.length()-pos-1);
    type = 0;
    op = 1;
    e = new Expr*[2];
    e[0] = new Expr(t1);
    e[1] = new Expr(t2);
    return;
  }
  pos = s.find('/');
  while (pos!=string::npos)
  {
    cnt = 0;
    for (int i=0;i<pos;i++)
    {
      if (s.at(i) == '(') cnt++;
      if (s.at(i) == ')') cnt--;
    }
    if (cnt!=0)
    {
      pos = s.find('/',pos+1);
      continue;
    }
    string t1;
    t1 = s.substr(0,pos);
    string t2;
    t2 = s.substr(pos+1,s.length()-pos-1);
    op = 3;
    type = 0;
    e = new Expr*[2];
    e[0] = new Expr(t1);
    e[1] = new Expr(t2);
    return;
  }
  pos = s.find('*');
  while (pos!=string::npos)
  {
    cnt = 0;
    for (int i=0;i<pos;i++)
    {
      if (s.at(i) == '(') cnt++;
      if (s.at(i) == ')') cnt--;
    }
    if (cnt!=0)
    {
      pos = s.find('*',pos+1);
      continue;
    }
    string t1;
    t1 = s.substr(0,pos);
    string t2;
    t2 = s.substr(pos+1,s.length()-pos-1);
    type = 0;
    op = 2;
    e = new Expr*[2];
    e[0] = new Expr(t1);
    e[1] = new Expr(t2);
    return;
  }
  if (s.at(0)=='(' && s.at(s.length()-1)==')')
  {
    Expr*e  = new Expr(s.substr(1,s.length()-2));
    this->e = e->e;
    this->type = e->type;
    this->a = e->a;
    this->s = e->s;
    this->op = e->op;
    return;
  }
  if (isdigit(s.at(0)))
  {
    type = 1;
    if (s.substr(0,2)=="0b")
    {
      int val = 0;
      for (int i=2;i<s.length();i++)
      {
        if (s.at(i)!='1' && s.at(i)!='0')
          Utilities::error("Invalid expression: " + s);
        val*=2;
        if (s.at(i)=='1')
          val+=1;
      }
      a=val;
      return;
    }
    if (s.at(0)=='0' && s.length()>1 && s.at(1)!='x')
    {
      for (int i=1;i<s.length();i++)
      {
        if (!isdigit(s.at(i)) || s.at(i)=='8' || s.at(i)=='9')
          Utilities::error("Invalid expression: " + s);
      }
      ss >> oct >> a;
      return;
    }
    if (s.substr(0,2)=="0x")
    {
      for (int i=2;i<s.length();i++)
        if (!isdigit(s.at(i)) && s.at(i)!='A' && s.at(i)!='B' && s.at(i)!='C' && s.at(i)!='D' && s.at(i)!='E' && s.at(i)!='F')
          Utilities::error("Invalid expression: " + s);
      ss >> hex >> a;
      return;
    }
    for (int i=0;i<s.length();i++)
      if (!isdigit(s.at(i)))
        Utilities::error("Invalid expression: " + s);
    ss >> a;
    type = 1;
    return;
  }
  this->s = s;
  SymbolTable::addUnknownSymbolIfNotExists(s);
  type=2;
  if (depends!=nullptr)
  {
    Symbol* sd= SymbolTable::getSymbolTable()->table[s];
    if (sd==depends)
    {
      Utilities::error("Symbol cannot depend on itself " + s1);
    }
    depends->iAmDependingOn.push_back(sd);
  }
}

Expr* Expr::parseExpr(const string & s)
{ 
  return new Expr(Utilities::removeSpaces(s), 1);
}


void Expr::check()
{

  if (checked) return;
  if (text.at(0)=='(' && isConstant()==false)
  {
    if (text.find("+")!=string::npos || text.find("-")!=string::npos || text.find("*")!=string::npos || text.find("/")!=string::npos)
      Utilities::error("Expression between braces mustn't be relocatible: " + text);
  }
  checked=true;
  scadd=false;
  if (type==0)
  {
    e[0]->check();
    e[1]->check();

    //cout <<"(" <<  text << ")" << e[0]->type << "," << e[1]->type << endl;
    if (e[0]->type==2 && e[1]->type==2)
    {
      Symbol* s1=SymbolTable::getSymbol(e[0]->s);
      Symbol* s2=SymbolTable::getSymbol(e[1]->s);
      //cout << s1->type << "," << s2->type << endl;
      if (((s1->segment != s2->segment) && s1->type==2 && s2->type==2))
      {
        Utilities::error("Can not operate 2 symbols from different segments: " + text);
      }
      if (((s1->segment != s2->segment && this->op!=1) && s1->type==2 && s2->type==2))
      {
        Utilities::error("Only substracion is allowed for operating 2 symbols: " + text);
      }
      if ((s1->type==3 && s2->type==2) )
      {
        Utilities::error("Can't add symbol and constant except when symbol is first: " + text);
      }
      if ((s1->type==2 && s2->type==3) || e[1]->scadd || e[0]->scadd)
      {
        //cout << "scadd" << endl;
        scadd=true;
      }
    }
    if (e[0]->type==1 && e[1]->type==2)
    {
      Symbol* s2=SymbolTable::getSymbol(e[1]->s);
      if (s2->type!=3)
        Utilities::error("Can't add symbol and constant except when symbol is first: " + text);
    }
    if (e[0]->type == 2 && op>1)
    {
      Symbol* s1=SymbolTable::getSymbol(e[0]->s);
      if (s1->type!=3)
        Utilities::error("symbol cannot be multiplied or divided " + text);
    }
    if (e[1]->type == 2 && op>1)
    {
      Symbol* s2=SymbolTable::getSymbol(e[1]->s);
      if (s2->type!=3)
        Utilities::error("symbol cannot be multiplied or divided " + text);
    }
    if (e[0]->type==2 && e[1]->type==1)
    {
      Symbol* s1=SymbolTable::getSymbol(e[0]->s);
      if (s1->type!=3)
      {
        scadd=true;
      }
    }
    if (e[1]->scadd)
    {
      Utilities::error("Can't add symbol and constant except when symbol is first: " + text);
    }
    if (e[1]->scadd || e[0]->scadd)
    {
      //cout << "scadd" << endl;
      scadd=true;
    }
  }
}


Expr* Expr::getTopLeftExpr()
{
  if (this->type==0)
  {
    return this->e[0]->getTopLeftExpr();
  }
  return this;
}

unsigned int Expr::value()
{
  check();
  this->validValue = true;
  if (this->type==0 && (e[0]->type==2 && e[1]->type==2))
  {
    Symbol* s1=SymbolTable::getSymbol(e[0]->s);
    Symbol* s2=SymbolTable::getSymbol(e[1]->s);
    
    if (s1->validValue==false || s2->validValue==false)
    {
      this->validValue = false;
      return 0;
    }
    string s =  to_string(Utilities::doOp(s1->value, s2->value,op));
    //cout << "Expr(" + this->text + ")=" + s << endl;
    return Utilities::doOp(s1->value, s2->value,op);
  }
  if (this->type == 0)
  {
    int value = Utilities::doOp(e[0]->value(),e[1]->value(),op);
    this->validValue = e[0]->validValue && e[1]->validValue;
    if (this->validValue)
    {
      //cout << "Expr(" + this->text + ")=" + to_string(value) << endl;
    }
    return value;
  }
  if (this->type == 1)
  {
    //cout << "Expr(" + this->text + ")=" + to_string(this->a) << endl;
    this->validValue = true;
    return this->a;
  }
  if (this->type == 2)
  {
    Symbol* s1=SymbolTable::getSymbol(this->s);
    this->validValue = s1->validValue;
    if (this->validValue)
    {
      //      cout << "Expr(" + this->text + ")=" + to_string(s1->value) << endl;
    }
    return s1->value;
  }
}

bool Expr::isConstant()
{
  if (type==0 && e[0]->type==0)
    return e[0]->isConstant();
  if (type==0)
  {
    if (e[0]->type==1)
      return true;
    Symbol* s =  SymbolTable::getSymbolTable()->table[e[0]->s];
    if (s->type==3)
      return true;
    if (e[1]->type!=2)
      return false;
    Symbol* s1 =  SymbolTable::getSymbolTable()->table[e[1]->s];
    if (s1->type==3)
      return false;
    if (s1->segment==s->segment && this->op==1)
      return true;
    return false;
  }
  if (type==1)
    return true;
  Symbol* s2 =  SymbolTable::getSymbolTable()->table[this->s];
  if (s2->type==3)
    return true;
  return false;
}
