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

Expression& sqrt(Expression& inner) { return (*(new SquareRoot(&inner))); }
Expression& sqrt(Expression* inner) { return (*(new SquareRoot(inner))); }

Expression& abs(Expression& inner) { return (*(new AbsoluteValue(&inner))); }
Expression& abs(Expression* inner) { return (*(new AbsoluteValue(inner))); }

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

Expression& arcsin(Expression& inner) { return (*(new ArcSine(&inner))); }
Expression& arcsin(Expression* inner) { return (*(new ArcSine(inner))); }

Expression& arccos(Expression& inner) { return (*(new ArcCosine(&inner))); }
Expression& arccos(Expression* inner) { return (*(new ArcCosine(inner))); }

Expression& arctan(Expression& inner) { return (*(new ArcTangent(&inner))); }
Expression& arctan(Expression* inner) { return (*(new ArcTangent(inner))); }

double d(Expression* f, Expression* x) {
	map<Expression*, double> derivatives_cache;
	map<Expression*, double> evaluations_cache;
	return f->p_derive(x, derivatives_cache, evaluations_cache);
}