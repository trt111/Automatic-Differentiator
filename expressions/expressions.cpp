#include "expressions.h"

void Expression::add_inner_addresses(Expression* inner) {
	set<Expression*>& addrss = inner->inner_addresses;
	set<Expression*>::iterator itr = addrss.begin();
	for (itr; itr != addrss.end(); itr++) {
		(this->inner_addresses).insert(*itr);
	}
	(this->inner_addresses).insert(inner);
}

double Expression::p_derive(Expression* var_of_deriving, map<Expression*, double>& derivatives_cache, map<Expression*, double>& evaluations_cache) {
	if (var_of_deriving->type == "number") throw "can't derive with respect to a number";
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
double Add::evaluate(map<Expression*, double>& evaluations_cache) { // (f+g)(x) = f(x) + g(x)
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (evaluations_cache.find(this) == end) {
		l_exp->evaluate(evaluations_cache);
		r_exp->evaluate(evaluations_cache);
		evaluations_cache[this] = evaluations_cache[l_exp] + evaluations_cache[r_exp];
	}
	return evaluations_cache[this];
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
double Subtract::evaluate(map<Expression*, double>& evaluations_cache) { // (f-g)(x) = f(x)-g(x)
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (evaluations_cache.find(this) == end) {
		l_exp->evaluate(evaluations_cache);
		r_exp->evaluate(evaluations_cache);
		evaluations_cache[this] = evaluations_cache[l_exp] - evaluations_cache[r_exp];
	}
	return evaluations_cache[this];
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
double Multiply::evaluate(map<Expression*, double>& evaluations_cache) { // (f*g)(x) = f(x)*g(x)
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (evaluations_cache.find(this) == end) {
		l_exp->evaluate(evaluations_cache);
		r_exp->evaluate(evaluations_cache);
		evaluations_cache[this] = evaluations_cache[l_exp] * evaluations_cache[r_exp];
	}
	return evaluations_cache[this];
}
double Multiply::surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) {
	double total = 0;
	if (var_of_deriving == this) return 1;
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (var_of_deriving == l_exp) {
		r_exp->evaluate(evaluations_cache);
		total += evaluations_cache[r_exp];
	}
	if (var_of_deriving == r_exp) {
		l_exp->evaluate(evaluations_cache);
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
double Divide::evaluate(map<Expression*, double>& evaluations_cache) { // (f/g)(x) = f(x)/g(x)
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (evaluations_cache.find(this) == end) {
		l_exp->evaluate(evaluations_cache);
		r_exp->evaluate(evaluations_cache);
		if (evaluations_cache[r_exp] == 0) throw "division by zero";
		evaluations_cache[this] = evaluations_cache[l_exp] / evaluations_cache[r_exp];
	}
	return evaluations_cache[this];
}
double Divide::surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) {
	if (var_of_deriving == this) return 1;
	double total = 0;
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (var_of_deriving == l_exp && var_of_deriving == r_exp) return 0;
	else if (var_of_deriving == l_exp) {
		r_exp->evaluate(evaluations_cache);
		return 1 / evaluations_cache[r_exp];
	}
	else if (var_of_deriving == r_exp) {
		r_exp->evaluate(evaluations_cache);
		l_exp->evaluate(evaluations_cache);
		return -evaluations_cache[l_exp] / (evaluations_cache[r_exp] * evaluations_cache[r_exp]);
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
double Polynomial::evaluate(map<Expression*, double>& evaluations_cache) { // (f^n)(x) = (f(x))^n
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (evaluations_cache.find(this) != end) return evaluations_cache[this];
	double base_evaluate = base->evaluate(evaluations_cache);
	if (base_evaluate == 0) {
		evaluations_cache[this] = 0;
		return 0;
	}
	double total = 1;
	for (int i = 0; i < (this->power); i++) {
		total *= base_evaluate;
	}
	evaluations_cache[this] = total;
	return evaluations_cache[this];
}
double Polynomial::surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) {
	if (var_of_deriving == this) return 1;
	else if (var_of_deriving == base) {
		map<Expression*, double>::iterator end = evaluations_cache.end();
		base->evaluate(evaluations_cache);
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

double SquareRoot::derive(Variable* var_of_deriving) { // (f*g)'(x) = f'(x)g(x) + g'(x)f(x)
	double base_derivative = base->derive(var_of_deriving);
	double base_eval = base->evaluate();

	return base_derivative / (2 * sqrt(base_eval));
}
double SquareRoot::evaluate(map<Expression*, double>& evaluations_cache) { // (f*g)(x) = f(x)*g(x)
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (evaluations_cache.find(this) == end) {
		base->evaluate(evaluations_cache);
		evaluations_cache[this] = sqrt(evaluations_cache[base]);
	}
	return evaluations_cache[this];
}
double SquareRoot::surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) {
	double total = 0;
	if (var_of_deriving == this) return 1;
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (var_of_deriving == base) {
		base->evaluate(evaluations_cache);
		total = 1 / (2 * sqrt(evaluations_cache[base]));
	}
	return total;
}

double Exp::derive(Variable* var_of_deriving) { // (a^f(x))' = a^x * ln(a) * f'(x)
	double inner_derivative = inner->derive(var_of_deriving);
	double inner_eval = inner->evaluate();
	return exp(inner_eval * conversion_base) * inner_derivative * conversion_base;
}
double Exp::evaluate(map<Expression*, double>& evaluations_cache) {
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (evaluations_cache.find(this) == end) {
		inner->evaluate(evaluations_cache);
		evaluations_cache[this] = exp(evaluations_cache[inner] * conversion_base);
	}
	return evaluations_cache[this];
}
double Exp::surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache){
	if (var_of_deriving == this) return 1;
	else if (var_of_deriving == this->inner) {
		inner->evaluate(evaluations_cache);
		return exp(evaluations_cache[inner] * conversion_base) * conversion_base;
	}
	return 0;
}

double Log::derive(Variable* var_of_deriving) { // (loga(f(x)))' = f'(x) / (f(x) * ln(a))
	double inner_derivative = inner->derive(var_of_deriving);
	double inner_eval = inner->evaluate();
	return  inner_derivative * conversion_base / inner_eval;
}
double Log::evaluate(map<Expression*, double>& evaluations_cache) {
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (evaluations_cache.find(this) == end) {
		inner->evaluate(evaluations_cache);
		if (evaluations_cache[inner] == 0) throw "Can't evaluate logarithm of a non-positive value";
		evaluations_cache[this] = log(evaluations_cache[inner]) * conversion_base;
	}
	return evaluations_cache[this];
}
double Log::surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) {
	if (var_of_deriving == this) return 1;
	else if (var_of_deriving == this->inner) {
		inner->evaluate(evaluations_cache);
		if (evaluations_cache[inner] == 0) throw "Logarithms doesn't have a derivative at inner value 0";
		return (1 / evaluations_cache[inner]) * conversion_base;
	}
	return 0;
}

double Sine::derive(Variable* var_of_deriving) { // (sin(f(x)))' = cos(f(x)) * f'(x)
	double inner_derivative = inner->derive(var_of_deriving);
	double inner_eval = inner->evaluate();
	return cos(inner_eval) * inner_derivative;
}
double Sine::evaluate(map<Expression*, double>& evaluations_cache) {
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (evaluations_cache.find(this) == end) {
		inner->evaluate(evaluations_cache);
		evaluations_cache[this] = sin(evaluations_cache[inner]);
	}
	return evaluations_cache[this];
}
double Sine::surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) {
	if (var_of_deriving == this) return 1;
	else if (var_of_deriving == this->inner) {
		inner->evaluate(evaluations_cache);
		return cos(evaluations_cache[inner]);
	}
	return 0;
}

double Cosine::derive(Variable* var_of_deriving) { // (cos(f(x)))' = -sin(f(x)) * f'(x)
	double inner_derivative = inner->derive(var_of_deriving);
	double inner_eval = inner->evaluate();
	return -sin(inner_eval) * inner_derivative;
}
double Cosine::evaluate(map<Expression*, double>& evaluations_cache) {
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (evaluations_cache.find(this) == end) {
		inner->evaluate(evaluations_cache);
		evaluations_cache[this] = cos(evaluations_cache[inner]);
	}
	return evaluations_cache[this];
}
double Cosine::surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) {
	if (var_of_deriving == this) return 1;
	else if (var_of_deriving == this->inner) {
		inner->evaluate(evaluations_cache);
		return -sin(evaluations_cache[inner]);
	}
	return 0;
}

double Tangent::derive(Variable* var_of_deriving) { // (a^f(x))' = a^x * ln(a) * f'(x)
	double inner_derivative = inner->derive(var_of_deriving);
	double inner_eval = inner->evaluate();
	double t = cos(inner_eval);
	return inner_derivative / (t * t);
}
double Tangent::evaluate(map<Expression*, double>& evaluations_cache) {
	map<Expression*, double>::iterator end = evaluations_cache.end();
	if (evaluations_cache.find(this) == end) {
		inner->evaluate(evaluations_cache);
		evaluations_cache[this] = tan(evaluations_cache[inner]);
	}
	return evaluations_cache[this];
}
double Tangent::surface_level_derive(Expression* var_of_deriving, map<Expression*, double>& evaluations_cache) {
	if (var_of_deriving == this) return 1;
	else if (var_of_deriving == this->inner) {
		inner->evaluate(evaluations_cache);
		double t = cos(evaluations_cache[inner]);
		return 1 / (t * t);
	}
	return 0;
}
