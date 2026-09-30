#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H


#include <iostream>
#include <string>
#include <set>
#include <map>

using namespace std;

class Variable;

class Expression
{
private:
	string type;
	set<Expression*> Ch;
	set<Expression*> inner_addresses;

protected:
	void add_inner_addresses(Expression* inner);
	void empty_addresses_set() { (this->inner_addresses).clear(); }
	void empty_children_set() { (this->Ch).clear(); }

public:
	Expression(string type) { this->type = type; }
	virtual double derive(Variable* var_of_deriving) = 0;
	virtual double surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) = 0;
	double p_derive(Expression* var_of_deriving, map<Expression*, double>& derivatives_cache, map<Expression*, double>& evaluations_cache);
	virtual double evaluate(map<Expression*, double>& evaluations_cache) = 0;
	double evaluate() {
		map<Expression*, double>m;
		return this->evaluate(m);
	}
	void add_child(Expression* child) { Ch.insert(child); }
	string get_type() { return type; }

	Expression& operator + (Expression* other);
	Expression& operator - (Expression* other);
	Expression& operator * (Expression* other);
	Expression& operator / (Expression* other);
	friend Expression& operator + (Expression* other, Expression& this_exp) { return this_exp + other; }
	friend Expression& operator - (Expression* other, Expression& this_exp) { return (*other) - (&this_exp); }
	friend Expression& operator * (Expression* other, Expression& this_exp) { return this_exp * other; }
	friend Expression& operator / (Expression* other, Expression& this_exp) { return (*other) / (&this_exp); }
	Expression& operator + (Expression& other) { return (*this) + (&other); }
	Expression& operator - (Expression& other) { return (*this) - (&other); }
	Expression& operator * (Expression& other) { return (*this) * (&other); }
	Expression& operator / (Expression& other) { return (*this) / (&other); }
	Expression& operator ^ (unsigned int power);

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

class BinaryExpression : public Expression
{
protected:
	Expression* l_exp;
	Expression* r_exp;

public:
	BinaryExpression(Expression* left, Expression* right, string type) : Expression(type), l_exp(left), r_exp(right) {
		if (l_exp->get_type() != "number") l_exp->add_child(this);
		if (r_exp->get_type() != "number") r_exp->add_child(this);
		add_inner_addresses(l_exp);
		add_inner_addresses(r_exp);
	}
	virtual double derive(Variable* var_of_deriving) = 0;
	virtual double surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) = 0;
	double evaluate(map<Expression*, double>& evaluations_cache) = 0;
	virtual ~BinaryExpression() = default;
};					
					
class Number : public Expression
{					
private:			
	double value;	
					
public:				
	Number(double value) : Expression("number") { this->value = value; }
	double derive(Variable* var_of_deriving) { return 0; }
	double surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) { return 0; }
	double evaluate(map<Expression*, double>& evaluations_cache) { evaluations_cache[this] = value; return value; }
	operator int() { return (int)value; }
	operator unsigned int() { if (value < 0) throw "Negative number cannot convert to a positive one"; return (int)value; }
	operator double() { return value; }
	virtual ~Number() = default;
};

class Variable : public Expression
{
private:
	double value;

public:
	Variable(double value) : Expression("variable") { this->value = value; }
	double derive(Variable* var_of_deriving) { return var_of_deriving == this; }
	double surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache){ return var_of_deriving == this; }
	double evaluate(map<Expression*, double>& evaluations_cache) { evaluations_cache[this] = value; return value; }
	double get_value() const { return value; }
	void operator = (double new_val) { (this->value) = new_val; }
	void operator = (const Variable& var) { (this->value) = var.get_value(); }
	virtual ~Variable() = default;

};

class Add : public BinaryExpression
{

public:
	Add(Expression* left, Expression* right) : BinaryExpression(left,right,"addition") {}
	Add(Expression& left, Expression& right): Add(&left, &right) {}
	double derive(Variable* var_of_deriving);
	double surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache);
	double evaluate(map<Expression*, double>& evaluations_cache);
	virtual ~Add() = default;
};

class Subtract : public BinaryExpression
{

public:
	Subtract(Expression* left, Expression* right) : BinaryExpression(left, right, "subtraction") {}
	Subtract(Expression& left, Expression& right) : Subtract(&left, &right) {}
	double derive(Variable* var_of_deriving);
	double surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache);
	double evaluate(map<Expression*, double>& evaluations_cache);
	virtual ~Subtract() = default;
};

class Multiply : public BinaryExpression
{

public:
	Multiply(Expression* left, Expression* right) : BinaryExpression(left,right,"Multiplication") {}
	Multiply(Expression& left, Expression& right): Multiply(&left, &right) {}
	double derive(Variable* var_of_deriving);
	double surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache);
	double evaluate(map<Expression*, double>& evaluations_cache);
	virtual ~Multiply() = default;
};

class Divide : public BinaryExpression
{

public:
	Divide(Expression* left, Expression* right) : BinaryExpression(left, right, "division") {}
	Divide(Expression& left, Expression& right) : Divide(&left, &right) {}
	double derive(Variable* var_of_deriving);
	double surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache);
	double evaluate(map<Expression*, double>& evaluations_cache);
	virtual ~Divide() = default;
};

class Polynomial : public Expression
{
private:
	Expression* base;
	unsigned int power;

public:
	Polynomial(Expression* base, unsigned int power) : Expression("polynomial"), base(base), power(power) { 
		if (this->base->get_type() != "number") this->base->add_child(this);
		add_inner_addresses(base);
	}
	Polynomial(Expression& base, unsigned int power): Polynomial(&base, power) {}
	double derive(Variable* var_of_deriving);
	double surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache);
	double evaluate(map<Expression*, double>& evaluations_cache);
	virtual ~Polynomial() = default;
};

class Function: public Expression
{
private:
	Expression* func;

public:
	Function(Expression* f) : Expression("function"), func(f) {
		add_inner_addresses(func);
	}
	Function(Expression& f): Function(&f) {}
	double derive(Variable* var_of_deriving) { return func->derive(var_of_deriving); }
	double surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) { return var_of_deriving == this || var_of_deriving == func; }
	double evaluate(map<Expression*, double>& evaluations_cache) { return func->evaluate(evaluations_cache); }
	virtual ~Function() = default;
};

double d(Expression* f, Expression* x);

#endif 
