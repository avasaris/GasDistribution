#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include "InitValues.h"

enum class AlgorithmPhase1 {
	N_2_0, N_2_1, N_2_2, N_2_3
};

class Contract;

using namespace std;

class Client {
public:
	Client(const string&);

	double GetMonthlyFact() const;
	double GetMonthlyPlan() const;
	double GetDailyFact(int) const;
	double GetDailyPlan(int) const;
	double GetDailyOffsetPlan(int) const;

	const string& SayMyName() const { return name; };

	void LinkToCaclulatePhase1() { CaclulatePhase1(); };

private:
	string name;
	vector<Contract> contracts;

	vector<int> contracts_in_overlimit_priority;
	vector<int> contracts_in_underlimit_priority;
	void ResortContracts();

	vector<AlgorithmPhase1> phase1;
	void CaclulatePhase1();
	
};



#endif CLIENT_H