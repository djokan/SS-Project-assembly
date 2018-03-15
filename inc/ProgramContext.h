#ifndef PROGRAMCONTEXT_H
#define PROGRAMCONTEXT_H
#include <iostream>
#include "Instruction.h"
#include <map>
#include <list>
#include "Expr.h"

using namespace std;

class ProgramContext{

};


class Symbol {
public:
  int number;
  string name;
  string segment;
  unsigned int value;
  int type; // 0- unknown, 1- segment, 2- label, 3- constant
  bool global;
  bool validValue;
  Expr* myEx;
  std::list<Symbol*> iAmDependingOn;
  Symbol(int,string,string,int,int);
};

struct OrgVal{
  string s;
  Expr* e;
};
struct DataVal
{
  int adr;
  Expr* number;
  string segment;
  int bytes;
};

class SymbolTable {
public:
  std::map<std::string, Symbol*> table;
  list<DataVal*> datas;
  list<OrgVal*> orgs;
  string currentSegment;
  int currentLocation;
  bool ended;
  bool isOrg;
  Expr* orgExpr;
  static void print();
  static SymbolTable* myST;
  static SymbolTable* getSymbolTable();
  static void addSegmentIfNotExists(const string& s);
  static void addLabelIfNotExists(const string& s);
  static void addConstantIfNotExists(const string& s);
  static void addGlobalIfNotExists(const string& s);
  static void addUnknownSymbolIfNotExists(const string& s);
  static void setGlobal(const string&, bool);
  static void checkAndNumberTable();
  static Symbol* getSymbol(const string&);
  SymbolTable();
  ~SymbolTable();
};
class SymbolWrite{
public:
  SymbolWrite(Symbol*,int,Expr*);
  Symbol* s;
  int address;
  Expr* expr;
  bool calcValue();
};


class RelocationInstance{
public:
  unsigned int adr;
  bool absolute;
  int number;
  RelocationInstance(int ,bool , int );
};

class RelocationTable{
public:
  static map <string, list<RelocationInstance*>*> tables;
  static void add(RelocationInstance* i);
  static void print();
};

struct SegmentCode
{
  map<int, unsigned char> code;
  int length;
};

class SourceCode{
public:
  map<string, SegmentCode* > segmentCodes;
  static std::list<SymbolWrite*> writeAfter;
  int currentLocation;
  static void writeAfterSymbols();
  static SourceCode* mySource;
  static SourceCode* getMySource();
  static void put(const unsigned char& byte);
  static void write(unsigned int);
  static void print();
  SourceCode();
  static void endSegmentFirstPass();
  static void endSegmentSecondPass();
};



#endif /* PROGRAMCONTEXT_H */
