#include "Instruction.h"
#include "Operand.h"

class LdSt: public Instruction{
public:
  unsigned char type;
  unsigned char code;
  unsigned char suffix;
  Operand** op;
  LdSt(const string &s,const unsigned char& opCode,const string&);
  void secondPass();
  ~LdSt();
};
