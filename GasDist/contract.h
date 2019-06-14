#ifndef CONTRACT_H
#define CONTRACT_H

#include <string>
#include <vector>
#include <map>
#include "InitValues.h"

using namespace std;

class Contract {
public:
	Contract(const string&, const string&, const int, const int, const double);
	int GetOLP() const;
	int GetULP() const;

	double GetMonthlyFact() const;
	double GetMonthlyPlan() const;
	double GetDailyFact(int day) const;
	double GetDailyPlan(int day) const;
	double GetDailyOffsetPlan(int day) const;
	string GetName() const { return name; };
	map<string, double> GetEachSquareFact(int day);

	void SetDailyFactP1(int, double);
	double GetDailyFactP1(int) const;
	double GetMonthlyFactP1() const;

	void SetDailyFactP2(int, double);
	double GetDailyFactP2(int) const;

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
