#include "Instruction.h"
#include "ArLog.h"
#include "Utilities.h"
#include <iostream>
#include "ProgramContext.h"
#include "Stack.h"
#include "LdSt.h"
#include "Segment.h"
#include "Define.h"
#include "Branch.h"

using namespace std;

Instruction::Instruction()
{
}

Instruction* Instruction::stringToInstruction(string line)
{
  try
  {
    if (SymbolTable::getSymbolTable()->ended)
    {
      return nullptr;
    }
    if (line.find(";") != string::npos)
    {
      line = line.substr(0,line.find(";"));
    }
    string word = Utilities::getFirstWord(line);
    if (word == "")
      return nullptr;
    if (word.back() == ':')
    {
      if (SymbolTable::getSymbolTable()->isOrg==true)
      {
        Utilities::error("Label can't be defined between ORG and semgent directives");
      }
      if (SymbolTable::getSymbolTable()->currentSegment=="")
      {
        Utilities::error("Label must be defined in segment");
      }
      string label = word.substr(0,word.length()-1);
      SymbolTable::addLabelIfNotExists(label);
      line = Utilities::removeFirstWord(line);
      word = Utilities::getFirstWord(line);
    }
    string temp = Utilities::removeFirstWord(line);
    if (word == "")
      return nullptr;
    if (word.substr(0,5) == ".text" || word.substr(0,4) == ".bss" || word.substr(0,5) == ".data" || word.substr(0,7) == ".rodata")
    {
      return new Segment(word);
    }
    if (SymbolTable::getSymbolTable()->isOrg==true)
    {
      Utilities::error("Segment must be defined after org directive");
    }
    if (word == "DD" || word == "DW" || word == "DB")
    {
      return new Define(line);
    }
    if (word == "ORG")
    {
      Utilities::parseOrg(temp);
      return nullptr;
    }
    if (word == ".global")
    {
      Utilities::parseGlobal(temp);
      return nullptr;
    }
    if (word == "mod")
    { 
      return new ArLog(temp,3,0x34);
    }
    if (word == "div")
    {
      return new ArLog(temp,3,0x33);
    }
    if (word == "add")
    { 
      return new ArLog(temp,3,0x30);
    }
    if (word == "sub")
    { 
      return new ArLog(temp,3,0x31);
    }
    if (word == "mul")
    { 
      return new ArLog(temp,3,0x32);
    }
    if (word == "and")
    { 
      return new ArLog(temp,3,0x35);
    }
    if (word == "or")
    {
      return new ArLog(temp,3,0x36);
    }
    if (word == "xor")
    { 
      return new ArLog(temp,3,0x37);
    }
    if (word == "not")
    { 
      return new ArLog(temp,2,0x38);
    }
    if (word == "asl")
    { 
      return new ArLog(temp,3,0x39);
    }
    if (word == "asr")
    { 
      return new ArLog(temp,3,0x3A);
    }
    if (word == "push")
    {
      return new Stack(temp,0x20);
    }
    if (word == "pop")
    {
      return new Stack(temp,0x21);
    }
    if (word.substr(0,4) == "load")
    {
      return new LdSt(temp,0x10,word.substr(4));
    }
    if (word.substr(0,5) == "store")
    {
      return new LdSt(temp,0x11,word.substr(5));
    }
    if (word == ".end")
    {
      SymbolTable::getSymbolTable()->ended=true;
      return nullptr;
    }
    if (word == "int")
    {
      return new Branch(temp,0x00);
    }
    if (word == "jmp")
    {
      return new Branch(temp,0x02);
    }
    if (word == "call")
    {
      return new Branch(temp,0x03);
    }
    if (word == "ret")
    {
      return new Branch(temp,0x01);
    }
    if (word == "jz")
    {
      return new Branch(temp,0x04);
    }
    if (word == "jnz")
    {
      return new Branch(temp,0x05);
    }
    if (word == "jgz")
    {
      return new Branch(temp,0x06);
    }
    if (word == "jgez")
    {
      return new Branch(temp,0x07);
    }
    if (word == "jlz")
    {
      return new Branch(temp,0x08);
    }
    if (word == "jlez")
    {
      return new Branch(temp,0x09);
    }

    cout << "Instruction " + word + " does not exist!" << endl;
    exit(1);
  }
  catch (...)
  {
    Utilities::error("Unknown error on line: "+ line);
  }
}

Instruction::~Instruction()
{
  
}

