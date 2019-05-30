#ifndef CONTRACT_H
#define CONTRACT_H

#include <string>
#include <vector>
#include "InitValues.h"

using namespace std;

class Contract {
public:
	Contract(const string&, const string&, const int, const int, const double);
	int MyOverlimitPri();
	int MyUnderlimitPri();

	/*double GetFullFact();
	double GetFullPlan();
	double GetFactForDay(int);
	double GetPlanForDay(int);*/
private:
	string name;
	string contracts_group;
	int overlimit_priority;
	int underlimit_priority;
	double offset_plan;
	vector<Square> squares;
};


#endif CONTRACT_H