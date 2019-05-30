#include "square.h"

Square::Square(const string& name, const string& number, const string& s_g, const string& d_t, const int days_in_month, const vector<double>& plan, const vector<double>& fact)
	: name{ name }, number{ number }, squares_group{ s_g }, delivery_type{ d_t }, days_in_month{ days_in_month },
	plan{ plan }, fact{ fact }
{
	/*
	cout << "Square Name=" << name << " Number=" << number << endl;
	cout << "Plan: ";
	for (auto p : plan) {
		cout << p << ", ";
	}
	cout << endl;
	cout << "Fact: ";
	for (auto p : fact) {
		cout << p << ", ";
	}
	cout << endl;
	*/
}

double Square::GetFactForDay(int day_of_interest) {
	return fact.at(day_of_interest);
}

double Square::GetPlanForDay(int day_of_interest) {
	return plan.at(day_of_interest);
}

double Square::GetFullFact() {
	double ret_fact = 0;
	for (int i = 1; i <= days_in_month; ++i) ret_fact += GetFactForDay(i);
	return ret_fact;
}

double Square::GetFullPlan() {
	double ret_plan = 0;
	for (int i = 1; i <= days_in_month; ++i) ret_plan += GetPlanForDay(i);
	return ret_plan;
}
