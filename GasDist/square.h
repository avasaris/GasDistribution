#ifndef SQUARE_H
#define SQUARE_H

#include <string>
#include <vector>


using namespace std;

class Square {
public:
	Square(const string&, const string&, const string&, const string&, const int, const vector<double>&, const vector<double>&);
	double GetMonthlyFact() const;
	double GetMonthlyPlan() const;
	double GetDailyFact(int) const;
	double GetDailyPlan(int) const;
	const string& GetName() const { return name; };
	const string& GetNumber() const { return number; };

	void SetDailyFinalFact(int day, double fact) { final_fact[day] = fact; };

private:
	string name;
	string number;
	vector<double> plan;
	vector<double> fact;
	int days_in_month;
	string squares_group;
	string delivery_type;

	vector<double> final_fact{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
};

#endif SQUARE_H