#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H


#include <iostream>
#include <string>
#include <set>

using namespace std;

class Expression
{
protected:
	string type;
	set<Expression*> Ch;
	set<Expression*> inner_addresses;

	void add_inner_addresses(Expression* inner) {
		set<Expression*>& addrss = inner->get_inner_addresses();
		set<Expression*>::iterator itr = addrss.begin();
		for (itr; itr != addrss.end(); itr++) {
			(this->inner_addresses).insert(*itr);
		}
		(this->inner_addresses).insert(inner);
	}
	void empty_addresses_set() {
		(this->inner_addresses).clear();
	}
	void empty_children_set() {
		(this->Ch).clear();
	}
public:
	Expression(string type) { this->type = type; }
	virtual double derive(Expression* var_of_deriving) = 0;
	virtual double evaluate() = 0;
	void add_child(Expression* child) { Ch.insert(child); }
	string get_type() { return type; }
	set<Expression*>& get_inner_addresses() { return inner_addresses; }
	virtual ~Expression() {
		set<Expression*>::iterator itr = (this->inner_addresses).begin();
		for (itr; itr != (this->inner_addresses).end(); itr++) {
			if (*itr) {
				(*itr)->empty_addresses_set();
				(*itr)->empty_children_set();
				delete* itr;
			}
		}
	}
};

class Number : public Expression
{
private:
	double value;
public:
	Number(double value) : Expression("number") { this->value = value; }
	double derive(Expression* var_of_deriving) { return 0; }
	double evaluate() { return value; }
	operator int() { return (int)value; }
	operator double() { return value; }
	virtual ~Number() = default;
};

class Variable : public Expression
{
private:
	double value;
public:
	Variable(double value) : Expression("variable") { this->value = value; }
	double derive(Expression* var_of_deriving) { return var_of_deriving == this; }
	double evaluate() { return value; }
	virtual ~Variable() = default;

};

class Add : public Expression
{
private:
	Expression* l_exp;
	Expression* r_exp;

public:
	Add(Expression* left, Expression* right) : Expression("addition"), l_exp(left), r_exp(right) {
		if (l_exp->get_type() != "number") l_exp->add_child(this);
		if (r_exp->get_type() != "number") r_exp->add_child(this);
		add_inner_addresses(l_exp);
		add_inner_addresses(r_exp);
	}
	double derive(Expression* var_of_deriving);
	double evaluate();
	virtual ~Add() = default;
};

class Subtract : public Expression
{
private:
	Expression* l_exp;
	Expression* r_exp;

public:
	Subtract(Expression* left, Expression* right) : Expression("subtraction"), l_exp(left), r_exp(right) { 
		if (l_exp->get_type() != "number") l_exp->add_child(this);
		if (r_exp->get_type() != "number") r_exp->add_child(this);
		add_inner_addresses(l_exp);
		add_inner_addresses(r_exp);
	}
	double derive(Expression* var_of_deriving);
	double evaluate();
	virtual ~Subtract() = default;
};

class Multiply : public Expression
{
private:
	Expression* l_exp;
	Expression* r_exp;

public:
	Multiply(Expression* left, Expression* right) : Expression("multiplication"), l_exp(left), r_exp(right) { 
		if (l_exp->get_type() != "number") l_exp->add_child(this);
		if (r_exp->get_type() != "number") r_exp->add_child(this);
		add_inner_addresses(l_exp);
		add_inner_addresses(r_exp);
	}
	double derive(Expression* var_of_deriving);
	double evaluate();
	virtual ~Multiply() = default;
};

class Divide : public Expression
{
private:
	Expression* l_exp;
	Expression* r_exp;

public:
	Divide(Expression* left, Expression* right) : Expression("division"), l_exp(left), r_exp(right) { 
		if (l_exp->get_type() != "number") l_exp->add_child(this);
		if (r_exp->get_type() != "number") r_exp->add_child(this);
		add_inner_addresses(l_exp);
		add_inner_addresses(r_exp);
	}
	double derive(Expression* var_of_deriving);
	double evaluate();
	virtual ~Divide() = default;
};

class Polynomial : public Expression
{
	Expression* base;
	unsigned int power;
public:
	Polynomial(Expression* base, unsigned int power) : Expression("polynomial"), base(base), power(power) { 
		if (this->base->get_type() != "number") this->base->add_child(this);
		add_inner_addresses(base);
	}
	double derive(Expression* var_of_deriving);
	double evaluate();
	virtual ~Polynomial() = default;
};

class Function: public Expression
{
	Expression* func;
public:
	Function(Expression* f) : Expression("function"), func(f) {
		add_inner_addresses(func);
	}
	double derive(Expression* var_of_deriving) { 
		if (var_of_deriving == this) return 1;
		return func->derive(var_of_deriving); 
	}
	double evaluate() { return func->evaluate(); }
	virtual ~Function() = default;
};

#endif 
