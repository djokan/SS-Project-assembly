#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include "Parser.h"

using namespace std;


int main(int argc, char *argv[])
{
  if (argc!=3)
  {
    cout << "Invalid number of arguments" << endl;
    exit(1);
  }
  ifstream in;
  in.open(argv[1]);
  ofstream out;
  out.open(argv[2]);

  if (in==NULL)
  {
    cout << "Input file not found or locked" << endl;
    exit(1);
  }

  if (out==NULL)
  {
    cout << "Output file not found or locked" << endl;
    exit(1);
  }

  Parser::getParser()->parse(in);
  Parser::getParser()->writeToFile(out);
  in.close();
  out.close();
  cout << "OK!" << endl;
  return 0;
}
