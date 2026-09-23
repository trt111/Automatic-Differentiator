#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H
#include <string>
#include <iostream>
using namespace std;

class Expression
{
private:
	string type;
public:
	Expression(string type) {
		this->type = type;
	}
	virtual double derive() = 0;
	virtual double evaluate() = 0;
	string get_type() { return type; }
	virtual ~Expression() {}
};

class Number : public Expression
{
private:
	double value;
public:
	Number(double value) : Expression("number") {
		this->value = value;
	}
	double derive() { return 0; }
	double evaluate() { return value; }
	virtual ~Number() {}

};

class Variable : public Expression 
{
private:
	double value;
public:
	Variable(double value) : Expression("variable") {
		this->value = value;
	}
	double derive() { return 1; }
	double evaluate() { return value; }
	virtual ~Variable() {}

};

class Add
{
private:
	Expression& l_exp;
	Expression& r_exp;

public:
	Add(Expression& left, Expression& right) : Expression("addition") {
		this->l_exp = left;
		this->r_exp = right;
	}
	virtual double derive() {
		double l_derivative = l_exp.derive();
		double r_derivative = r_exp.derive();
		return l_derivative + r_derivative;
	}
	virtual double evaluate() {
		double l_evaluate = l_exp.evaluate();
		double r_evaluate = r_exp.evaluate();
		return l_evaluate + r_evaluate;
	}
	virtual ~Add() {
		delete l_exp;
		delete r_exp;
	}
};

class Subtract
{
private:
	Expression& l_exp;
	Expression& r_exp;

public:
	Subtract(Expression& left, Expression& right) : Expression("subtraction") {
		this->l_exp = left;
		this->r_exp = right;
	}
	virtual double derive() {
		double l_derivative = l_exp.derive();
		double r_derivative = r_exp.derive();
		return l_derivative - r_derivative;
	}
	virtual double evaluate() {
		double l_evaluate = l_exp.evaluate();
		double r_evaluate = r_exp.evaluate();
		return l_evaluate - r_evaluate;
	}
	virtual ~Subtract() {
		delete l_exp;
		delete r_exp;
	}
};

class Multiply
{
private:
	Expression& l_exp;
	Expression& r_exp;

public:
	Multiply(Expression& left, Expression& right) : Expression("multiplication") {
		this->l_exp = left;
		this->r_exp = right;
	}
	virtual double derive() {
		double l_derivative = l_exp.derive();
		double r_derivative = r_exp.derive();
		double l_evaluate = l_exp.evaluate();
		double r_evaluate = r_exp.evaluate();
		return l_derivative * r_evaluate + r_derivative * l_evaluate;
	}
	virtual double evaluate() {
		double l_evaluate = l_exp.evaluate();
		double r_evaluate = r_exp.evaluate();
		return l_evaluate * r_evaluate;
	}
	virtual ~Multiply() {
		delete l_exp;
		delete r_exp;
	}
};

class Divide
{
private:
	Expression& t_exp;
	Expression& b_exp;

public:
	Divide(Expression& top, Expression& buttom) : Expression("division") {
		this->t_exp = top;
		this->b_exp = buttom;
	}
	virtual double derive() {
		double t_derivative = t_exp.derive();
		double b_derivative = b_exp.derive();
		double t_evaluate = t_exp.evaluate();
		double b_evaluate = b_exp.evaluate();
		if (b_evaluate == 0) throw "division by zero";
		return (t_derivative * b_evaluate - b_derivative * t_evaluate)/(b_evaluate*b_evaluate);
	}
	virtual double evaluate() {
		double t_evaluate = t_exp.evaluate();
		double b_evaluate = b_exp.evaluate();
		if (b_evaluate == 0) throw "division by zero";
		return t_evaluate / b_evaluate;
	}
	virtual ~Divide() {
		delete t_exp;
		delete b_exp;
	}
};

#endif 
