#ifndef CONTRACT_H
#define CONTRACT_H

#include <string>
#include <vector>
#include "InitValues.h"

using namespace std;

class Contract {
public:
	Contract(const string&, const string&, const int, const int, const double);
	int MyOverlimitPri() const;
	int MyUnderlimitPri() const;

	double GetMonthlyFact() const;
	double GetMonthlyPlan() const;
	double GetDailyFact(int) const;
	double GetDailyPlan(int) const;
	double GetDailyOffsetPlan(int) const;

private:
	string name;
	string contracts_group;
	int overlimit_priority;
	int underlimit_priority;
	double offset_plan;
	vector<Square> squares;

	//-- recalc fields
	vector<double> rf_plan;
	vector<double> rf_fact;


};


#endif CONTRACT_H
