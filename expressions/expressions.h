#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H


#include <iostream>
#include <string>
#include <set>

using namespace std;

class Expression
{
private:
	string type;
	set<Expression*> Ch;
public:
	Expression(string type) { this->type = type; }
	virtual double derive() = 0;
	virtual double evaluate() = 0;
	void add_child(Expression* child) { Ch.insert(child); }
	string get_type() { return type; }
	virtual ~Expression() {}
};

class Number : public Expression
{
private:
	double value;
public:
	Number(double value) : Expression("number") { this->value = value; }
	double derive() { return 0; }
	double evaluate() { return value; }
	virtual ~Number() {}
	operator int() { return (int)value; }
	operator int() { return value; }
};

class Variable : public Expression
{
private:
	double value;
public:
	Variable(double value) : Expression("variable") { this->value = value; }
	double derive() { return 1; }
	double evaluate() { return value; }
	virtual ~Variable() {}

};

class Add : public Expression
{
private:
	Expression* l_exp;
	Expression* r_exp;

public:
	Add(Expression* left, Expression* right) : Expression("addition"), l_exp(left), r_exp(right) {
		if (l_exp->type != "number") l_exp->add_child(this);
		if (r_exp->type != "number") r_exp->add_child(this);
	}
	double derive();
	double evaluate();
	virtual ~Add() { delete l_exp; delete r_exp; }
};

class Subtract : public Expression
{
private:
	Expression* l_exp;
	Expression* r_exp;

public:
	Subtract(Expression* left, Expression* right) : Expression("subtraction"), l_exp(left), r_exp(right) { 
		if (l_exp->type != "number") l_exp->add_child(this);
		if (r_exp->type != "number") r_exp->add_child(this);
	}
	double derive();
	double evaluate();
	virtual ~Subtract() { delete l_exp; delete r_exp; }
};

class Multiply : public Expression
{
private:
	Expression* l_exp;
	Expression* r_exp;

public:
	Multiply(Expression* left, Expression* right) : Expression("multiplication"), l_exp(left), r_exp(right) { 
		if (l_exp->type != "number") l_exp->add_child(this);
		if (r_exp->type != "number") r_exp->add_child(this);
	}
	double derive();
	double evaluate();
	virtual ~Multiply() { delete l_exp; delete r_exp; }
};

class Divide : public Expression
{
private:
	Expression* l_exp;
	Expression* r_exp;

public:
	Divide(Expression* left, Expression* right) : Expression("division"), l_exp(left), r_exp(right) { 
		if (l_exp->type != "number") l_exp->add_child(this);
		if (r_exp->type != "number") r_exp->add_child(this);
	}
	double derive();
	double evaluate();
	virtual ~Divide() { delete l_exp; delete r_exp; }
};

class Polynomial : public Expression
{
	Expression* base;
	unsigned int power;
public:
	Polynomial(Expression* base, unsigned int power) : Expression("polynomial"), base(base), power(power) { 
		if (this->base->type != "number") this->base->add_child(this);
	}
	double derive();
	double evaluate();
	virtual ~Poynomial() { delete base; }
};

#endif 
