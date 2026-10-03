#include "native_operators.h"

Expression& operator + (Expression& this_exp, Expression& other) { return (*(new Add(this_exp, other))); }
Expression& operator + (Expression& this_exp, Expression* other) { return this_exp + (*other); }
Expression& operator + (Expression* this_exp, Expression& other) { return (*this_exp) + other; }

Expression& operator - (Expression& this_exp, Expression& other) { return (*(new Subtract(this_exp, other))); }
Expression& operator - (Expression& this_exp, Expression* other) { return this_exp - (*other); }
Expression& operator - (Expression* this_exp, Expression& other) { return (*this_exp) - other; }

Expression& operator * (Expression& this_exp, Expression& other) { return (*(new Multiply(this_exp, other))); }
Expression& operator * (Expression& this_exp, Expression* other) { return this_exp * (*other); }
Expression& operator * (Expression* this_exp, Expression& other) { return (*this_exp) * other; }

Expression& operator / (Expression& this_exp, Expression& other) { return (*(new Divide(this_exp, other))); }
Expression& operator / (Expression& this_exp, Expression* other) { return this_exp / (*other); }
Expression& operator / (Expression* this_exp, Expression& other) { return (*this_exp) / other; }


Expression& operator ^ (Expression& this_exp, unsigned int power) { return (*(new Polynomial(&this_exp, power))); }

Expression& operator ^ (double base, Expression& this_exp) { return (*(new Exp(&this_exp, base))); }
Expression& exp(Expression& inner, double base) { return (*(new Exp(&inner, base))); }
Expression& exp(Expression* inner, double base) { return (*(new Exp(inner, base))); }

Expression& ln(Expression& inner) { return (*(new Log(&inner))); }
Expression& ln(Expression* inner) { return (*(new Log(inner))); }

Expression& log(Expression& inner, double base) { return (*(new Log(&inner, base))); }
Expression& log(Expression* inner, double base) { return (*(new Log(inner, base))); }

Expression& sin(Expression& inner) { return (*(new Sine(&inner))); }
Expression& sin(Expression* inner) { return (*(new Sine(inner))); }

Expression& cos(Expression& inner) { return (*(new Cosine(&inner))); }
Expression& cos(Expression* inner) { return (*(new Cosine(inner))); }

Expression& tan(Expression& inner) { return (*(new Tangent(&inner))); }
Expression& tan(Expression* inner) { return (*(new Tangent(inner))); }

double d(Expression* f, Expression* x) {
	map<Expression*, double> derivatives_cache;
	map<Expression*, double> evaluations_cache;
	return f->p_derive(x, derivatives_cache, evaluations_cache);
}