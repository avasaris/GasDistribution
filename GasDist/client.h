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

	void LinkToCaclulateAlgorithmForPhase1() { CaclulateAlgorithmForPhase1(); };
	void LinkToCalculatePhase1() { CalculatePhase1(); };

private:
	string name;
	vector<Contract> contracts;

	vector<int> contracts_desc_overlimit_priority;
	vector<int> contracts_desc_underlimit_priority;
	void ResortContracts();

	vector<AlgorithmPhase1> algorithm_phase1;
	void CaclulateAlgorithmForPhase1();

	vector<vector<double>> fact_phase1;
	void CalculatePhase1();
	
};



#endif CLIENT_H