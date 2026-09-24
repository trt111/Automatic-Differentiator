#include "expressions.h"

double Add::derive() { // (f+g)'(x) = f'(x) + g'(x)
	double l_derivative = l_exp->derive();
	double r_derivative = r_exp->derive();
	return l_derivative + r_derivative;
}
double Add::evaluate() { // (f+g)(x) = f(x) + g(x)
	double l_evaluate = l_exp->evaluate();
	double r_evaluate = r_exp->evaluate();
	return l_evaluate + r_evaluate;
}

double Subtract::derive() { // (f-g)'(x) = f'(x) - g'(x)
	double l_derivative = l_exp->derive();
	double r_derivative = r_exp->derive();
	return l_derivative - r_derivative;
}
double Subtract::evaluate() { // (f-g)(x) = f(x)-g(x)
	double l_evaluate = l_exp->evaluate();
	double r_evaluate = r_exp->evaluate();
	return l_evaluate - r_evaluate;
}

double Multiply::derive() { // (f*g)'(x) = f'(x)g(x) + g'(x)f(x)
	double l_derivative = l_exp->derive();
	double r_derivative = r_exp->derive();
	double l_evaluate = l_exp->evaluate();
	double r_evaluate = r_exp->evaluate();
	return l_derivative * r_evaluate + r_derivative * l_evaluate;
}
double Multiply::evaluate() { // (f*g)(x) = f(x)*g(x)
	double l_evaluate = l_exp->evaluate();
	double r_evaluate = r_exp->evaluate();
	return l_evaluate * r_evaluate;
}

double Divide::derive() { // (f/g)'(x) = (f'(x)g(x) - g'(x)f(x))/(g(x))^2
	double l_derivative = l_exp->derive();
	double r_derivative = r_exp->derive();
	double l_evaluate = l_exp->evaluate();
	double r_evaluate = r_exp->evaluate();
	if (r_evaluate == 0) throw "division by zero";
	return (l_derivative * r_evaluate - r_derivative * l_evaluate) / (r_evaluate * r_evaluate);
}
double Divide::evaluate() { // (f/g)(x) = f(x)/g(x)
	double l_evaluate = l_exp->evaluate();
	double r_evaluate = r_exp->evaluate();
	if (r_evaluate == 0) throw "division by zero";
	return l_evaluate / r_evaluate;
}

double Polynomial::derive() { // (f^n)'(x) = n*f'(x)*(f(x))^(n-1)
	double base_evaluate = base->evaluate();
	if (base_evaluate == 0) return 0;
	double base_derivative = base->derive();
	double total = power;
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