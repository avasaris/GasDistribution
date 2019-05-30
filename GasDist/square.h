#ifndef SQUARE_H
#define SQUARE_H

#include <string>
#include <vector>

using namespace std;

class Square {
public:
	Square(const string&, const string&, const string&, const string&, const int, const vector<double>&, const vector<double>&);
	double GetMonthlyFact();
	double GetMonthlyPlan();
	double GetDailyFact(int);
	double GetDailyPlan(int);
private:
	string name;
	string number;
	vector<double> plan;
	vector<double> fact;
	int days_in_month;
	string squares_group;
	string delivery_type;
};

#endif SQUARE_H