#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <map>
#include "Instruction.h"
#include "Parser.h"
#include "ProgramContext.h"
#include "Utilities.h"
using namespace std;

Parser* Parser::myParser = nullptr;

Parser* Parser::getParser()
{
  if (myParser == nullptr)
  {
    myParser = new Parser();
  }
  return myParser;
}
void Parser::parse(ifstream& in)
{
  string line;
  int instructionsNumber = 0;
  map<int, Instruction* > m;
  bool ended = false;
  while(getline(in,line))
  {
    if (line.substr(0,4) == ".end")
    {
      if (SymbolTable::getSymbolTable()->currentSegment!="")
      {
        SourceCode::endSegmentFirstPass();
      }
      ended=true;
      break;
    }
    m[instructionsNumber] = Instruction::stringToInstruction(line);
    if (m[instructionsNumber]!=nullptr)
      instructionsNumber++;
  }
  if (!ended)
  {
    Utilities::error(".end directive doesn't exist");
  }
  Utilities::betweenPass();
  for (int i=0;i<instructionsNumber;i++)
  {
    m[i]->secondPass();
  }
  SourceCode::endSegmentSecondPass();
  SymbolTable::print();
  RelocationTable::print();
  SourceCode::print();
}
void Parser::writeToFile(ofstream& out)
{
  out << "#TabelaSimbola" << endl;
  for (auto const a: SymbolTable::getSymbolTable()->table)
  {
    if (a.second->name == a.second->segment)
    {
      out << "SEG " << a.second->number << " " << a.second->name << " " << a.second->number << " ";
      unsigned int temp = a.second->value;
      out << "0x" << Utilities::toHex(temp) << " 0x";
      temp = SourceCode::getMySource()->segmentCodes[a.second->name]->length;
      out << Utilities::toHex(temp) << " ";
      out << "L";
      if (a.second->type!=3)
        out << "R";
      out << endl;
    }
    else
    {
      if (a.second->segment!="extern")
      {
        Symbol* seg = SymbolTable::getSymbolTable()->table[a.second->segment];
        out << "SYM " << a.second->number << " " << a.second->name <<" " ;
        if (a.second->type == 3)
        {
          out << "-1 ";
        }
        else
        {
          out << seg->number << " ";
        }
        unsigned int temp = a.second->value;
        out << "0x" << Utilities::toHex(temp) << " ";
        if (a.second->global)
        {
          out << "G";
        }
        else
        {
          out << "L";
        }
        out << endl;
      }
      else
      {
        out << "SYM " << a.second->number << " " << a.second->name <<" 0 0x00 g" << endl;
      }
    }
  }
  for (auto const a: SourceCode::getMySource()->segmentCodes)
  {
    list<RelocationInstance*>* l = RelocationTable::tables[a.first];
    RelocationInstance* r;
    if (a.first.substr(0,4)==".bss")
    {
      continue;
    }
    out << "#rel" << a.first << endl;
    for (int i=0;i<l->size();i++)
    {
      r= l->front();
      l->pop_front();
      l->push_back(r);
      string t = r->absolute?"A":"R";
      out << "0x" << Utilities::toHex(r->adr) << " " << t  << " " << r->number << endl;
    }
    int iter=0;
    out << a.first << endl;
    for (int i=0;i<a.second->length;i++)
    {
      out << Utilities::toHex(a.second->code[i]);
      iter++;
      if (iter==16 || i==a.second->length-1)
      {
        iter=0;
        out << endl;
      }
      else
      {
        out << " ";
      }
    }
  }
  out << "#end" << endl;
}
