#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string.h>

class Operand;
class Expr;


using namespace std;

class Utilities {
public:
  static int isValidExpr(const Expr* t);
  static string getFirstWord(const string &line);
  static string getSecondWord(const string &line);
  static string removeFirstWord(const string &line);
  static string removeSpaces(const string& s);
  static void error(string s);
  static bool containsNumber(const std::string&);
  static bool containsLetter(const std::string&);
  static void createInstruction(const unsigned char* , const int* , const int& ,const int&);
  static void createInstruction(const unsigned int&);
  static unsigned int doOp(const unsigned int&,const unsigned int&,const int&);
  static const char* byte_to_binary(int);
  static void betweenPass();
  static void parseGlobal(string);
  static string generateRandomString(size_t length);
  static string replaceAll(string str, const string& from, const string& to);
  static void parseOrg(string s);
  static string toHex(unsigned int);
  static string toHex(unsigned char);
};

#endif /* UTILITIES_H */
