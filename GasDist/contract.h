#ifndef CONTRACT_H
#define CONTRACT_H

#include <string>
#include <vector>
#include "InitValues.h"

using namespace std;

class Contract {
public:
	Contract(const string&, const string&, const int, const int, const double);
	int GetOLP() const;
	int GetULP() const;

	double GetMonthlyFact() const;
	double GetMonthlyPlan() const;
	double GetDailyFact(int) const;
	double GetDailyPlan(int) const;
	double GetDailyOffsetPlan(int) const;
	string GetName() const { return name; };

	void SetDailyFactP1(int, double);
	double GetDailyFactP1(int) const;
	double GetMonthlyFactP1() const;

private:
	string name;
	string contracts_group;
	int overlimit_priority;
	int underlimit_priority;
	double offset_plan;
	vector<Square> squares;

	//-- fields for phase1
	vector<double> fact_p1;

	//-- fields for phase2
	vector<double> fact_p2;

};


#endif CONTRACT_H
