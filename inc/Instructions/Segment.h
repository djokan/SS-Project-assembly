#ifndef SEGMENT_H
#define SEGMENT_H




#include "Instruction.h"

class Segment: public Instruction{
public:
  string ins;
  Segment(const string& s);
  void secondPass();
  ~Segment();
};

#endif /* SEGMENT_H */
