#ifndef NATIVE_OPERATORS_H
#define NATIVE_OPERATORS_H
#include "expressions.h"

Expression& operator + (Expression& this_exp, Expression& other);
Expression& operator + (Expression& this_exp, Expression* other);
Expression& operator + (Expression* this_exp, Expression& other);

Expression& operator - (Expression& this_exp, Expression& other);
Expression& operator - (Expression& this_exp, Expression* other);
Expression& operator - (Expression* this_exp, Expression& other);

Expression& operator * (Expression& this_exp, Expression& other);
Expression& operator * (Expression& this_exp, Expression* other);
Expression& operator * (Expression* this_exp, Expression& other);

Expression& operator / (Expression& this_exp, Expression& other);
Expression& operator / (Expression& this_Exp, Expression* other);
Expression& operator / (Expression* this_exp, Expression& other);

Expression& operator ^ (Expression& this_exp, unsigned int power);


Expression& operator ^ (double base, Expression& power);
Expression& exp(Expression& inner, double base = Math().E);
Expression& exp(Expression* inner, double base = Math().E);


Expression& ln(Expression& inner);
Expression& ln(Expression* inner);

Expression& log(Expression& inner, double base = Math().E);
Expression& log(Expression* inner, double base = Math().E);

Expression& sin(Expression& inner);
Expression& sin(Expression* inner);

Expression& cos(Expression& inner);
Expression& cos(Expression* inner);

Expression& tan(Expression& inner);
Expression& tan(Expression* inner);

double d(Expression* f, Expression* x);

#endif