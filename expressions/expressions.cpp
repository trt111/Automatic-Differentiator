#include "expressions.h"

double d(Expression* f, Expression* x) {
	map<Expression*, double> derivatives_cache;
	map<Expression*, double> evaluations_cache;
	return f->p_derive(x, derivatives_cache, evaluations_cache);
}

void Expression::add_inner_addresses(Expression* inner) {
	set<Expression*>& addrss = inner->inner_addresses;
	set<Expression*>::iterator itr = addrss.begin();
	for (itr; itr != addrss.end(); itr++) {
		(this->inner_addresses).insert(*itr);
	}
	(this->inner_addresses).insert(inner);
}

double Expression::p_derive(Expression* var_of_deriving, map<Expression*, double>& derivatives_cache, map<Expression*, double>& evaluations_cache) {
	if (var_of_deriving == this) return 1;
	else if (var_of_deriving->Ch.size() == 0) { return 0; }
	double derivative = 0;
	double surface_derivative;
	set<Expression*>::iterator itr = var_of_deriving->Ch.begin();
	map<Expression*, double>::iterator cache_end = derivatives_cache.end();
	for (itr; itr != var_of_deriving->Ch.end(); itr++) {
		surface_derivative = (*itr)->surface_level_derive(var_of_deriving, evaluations_cache);
		if (surface_derivative != 0) {
			if (derivatives_cache.find(*itr) == cache_end) derivatives_cache[*itr] = (this)->p_derive(*itr, derivatives_cache, evaluations_cache);
			derivative += derivatives_cache[*itr] * surface_derivative;
		}
	}
	return derivative;
}

double Add::derive(Variable* var_of_deriving) { // (f+g)'(x) = f'(x) + g'(x)
	double l_derivative = l_exp->derive(var_of_deriving);
	double r_derivative = r_exp->derive(var_of_deriving);
	return l_derivative + r_derivative;
}
double Add::evaluate() { // (f+g)(x) = f(x) + g(x)
	double l_evaluate = l_exp->evaluate();
	double r_evaluate = r_exp->evaluate();
	return l_evaluate + r_evaluate;
}
double Add::surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) {
	if (var_of_deriving == this) return 1;
	double total = 0;
	if (var_of_deriving == l_exp) total += 1;
	if (var_of_deriving == r_exp) total += 1;
	return total;
}

double Subtract::derive(Variable* var_of_deriving) { // (f-g)'(x) = f'(x) - g'(x)
	double l_derivative = l_exp->derive(var_of_deriving);
	double r_derivative = r_exp->derive(var_of_deriving);
	return l_derivative - r_derivative;
}
double Subtract::evaluate() { // (f-g)(x) = f(x)-g(x)
	double l_evaluate = l_exp->evaluate();
	double r_evaluate = r_exp->evaluate();
	return l_evaluate - r_evaluate;
}
double Subtract::surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) {
	if (var_of_deriving == this) return 1;
	double total = 0;
	if (var_of_deriving == l_exp) total += 1;
	if (var_of_deriving == r_exp) total += -1;
	return total;
}

double Multiply::derive(Variable* var_of_deriving) { // (f*g)'(x) = f'(x)g(x) + g'(x)f(x)
	double l_derivative = l_exp->derive(var_of_deriving);
	double r_derivative = r_exp->derive(var_of_deriving);
	double l_evaluate = 0;
	double r_evaluate = 0;
	if (l_derivative != 0) r_evaluate = r_exp->evaluate();
	if (r_derivative != 0) l_evaluate = l_exp->evaluate();
	return l_derivative * r_evaluate + r_derivative * l_evaluate;
}
double Multiply::evaluate() { // (f*g)(x) = f(x)*g(x)
	double l_evaluate = l_exp->evaluate();
	double r_evaluate = r_exp->evaluate();
	return l_evaluate * r_evaluate;
}
double Multiply::surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) {
	double total = 0;
	if (var_of_deriving == this) return 1;
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (var_of_deriving == l_exp) {
		if (evaluations_cache.find(r_exp) == end) evaluations_cache[r_exp] = r_exp->evaluate();
		total += evaluations_cache[r_exp];
	}
	if (var_of_deriving == r_exp) {
		if (evaluations_cache.find(l_exp) == end) evaluations_cache[l_exp] = l_exp->evaluate();
		total += evaluations_cache[l_exp];
	}
	return total;
}

double Divide::derive(Variable* var_of_deriving) { // (f/g)'(x) = (f'(x)g(x) - g'(x)f(x))/(g(x))^2
	double l_derivative = l_exp->derive(var_of_deriving);
	double r_derivative = r_exp->derive(var_of_deriving);
	double l_evaluate = 0;
	double r_evaluate = 0;
	if (l_derivative != 0) r_evaluate = r_exp->evaluate();
	if (r_evaluate == 0) throw "division by zero";
	if (r_derivative != 0) l_evaluate = l_exp->evaluate();
	return (l_derivative * r_evaluate - r_derivative * l_evaluate) / (r_evaluate * r_evaluate);
}
double Divide::evaluate() { // (f/g)(x) = f(x)/g(x)
	double r_evaluate = r_exp->evaluate();
	if (r_evaluate == 0) throw "division by zero";
	double l_evaluate = l_exp->evaluate();
	return l_evaluate / r_evaluate;
}
double Divide::surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) {
	if (var_of_deriving == this) return 1;
	double total = 0;
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (var_of_deriving == l_exp && var_of_deriving == r_exp) return 0;
	else if (var_of_deriving == l_exp) {
		if (evaluations_cache.find(r_exp) == end) evaluations_cache[r_exp] = r_exp->evaluate();
		return 1 / evaluations_cache[r_exp];
	}
	else if (var_of_deriving == r_exp) {
		if (evaluations_cache.find(r_exp) == end) evaluations_cache[r_exp] = r_exp->evaluate();
		if (evaluations_cache.find(l_exp) == end) evaluations_cache[l_exp] = l_exp->evaluate();
		return -evaluations_cache[l_exp] / evaluations_cache[r_exp] * evaluations_cache[r_exp];
	}
	return 0;
}

double Polynomial::derive(Variable* var_of_deriving) { // (f^n)'(x) = n*f'(x)*(f(x))^(n-1)
	double base_evaluate = base->evaluate();
	if (base_evaluate == 0) return 0;
	double base_derivative = base->derive(var_of_deriving);
	double total = power * base_derivative;
	for (int i = 0; i < (this->power)-1; i++) {
		total *= base_evaluate;
	}

	return total;
}
double Polynomial::evaluate() { // (f^n)(x) = (f(x))^n
	double base_evaluate = base->evaluate();
	if (base_evaluate == 0) return 0;
	double total = 1;
	for (int i = 0; i < (this->power); i++) {
		total *= base_evaluate;
	}
	return total;
}
double Polynomial::surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) {
	if (var_of_deriving == this) return 1;
	else if (var_of_deriving == base) {
		map<Expression*, double>::iterator end = evaluations_cache.end();
		if (evaluations_cache.find(base) == end) evaluations_cache[base] = base->evaluate();
		double eval_base = evaluations_cache[base];
		if (eval_base == 0) return 0;
		double total = power;
		for (int i = 0; i < (this->power) - 1; i++) {
			total *= eval_base;
		}
		return total;
	}
	return 0;
}

Expression& Expression::operator + (Expression* other) { return (*(new Add(this, other))); }
Expression& Expression::operator - (Expression* other) { return (*(new Subtract(this, other))); }
Expression& Expression::operator * (Expression* other) { return (*(new Multiply(this, other))); }
Expression& Expression::operator / (Expression* other) { return (*(new Divide(this, other))); }
Expression& Expression::operator ^ (unsigned int power) { return (*(new Polynomial(this, power))); }