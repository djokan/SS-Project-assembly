#ifndef EXPR_H
#define EXPR_H
#include <iostream>

class Symbol;

using namespace std;

class Expr{
private:
  Expr(const string& s, const int& i=0);
public:
  static Symbol* depends;
  static Expr* currentExpr;
  Expr** e;
  string text;
  string s;
  int op; // 0= '+' 1= '-' 2= '*' 3= '/'
  unsigned int a;
  int type; // 0= ex ex; 1= int; 2= string
  bool scadd;
  bool checked;
  void check();
  bool validValue;
  Expr* getTopLeftExpr();
  bool isConstant();
  static Expr* parseExpr(const string& s);
  unsigned int value();
};


#endif /* EXPR_H */
