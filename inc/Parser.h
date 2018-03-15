#ifndef PARSER_H
#define PARSER_H





#include <fstream>

using namespace std;

class Parser
{
private:
  static Parser *myParser;
  Parser()
  {
    
  }
public:
  void parse(ifstream &in);
  void writeToFile(ofstream &out);
  static Parser* getParser();  
};

#endif /* PARSER_H */
