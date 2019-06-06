#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>
#include "InitValues.h"

enum class AlgorithmPhase1 {
	N_2_0, N_2_1, N_2_2, N_2_3
};

enum class AlgorithmPhase2 {
	N_3_0, N_3_1, N_3_2, N_3_3
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
	
	AlgorithmPhase2 LinkToCheckIfWeNeedPhase2() { return CheckIfWeNeedPhase2(); };
	void LinkToPhase2Algo3() { return Phase2Algo3(); };

private:
	string name;
	vector<Contract> contracts;

	vector<int> OLP_asc_order;
	vector<int> ULP_desc_order;
	void Reorder();

	vector<AlgorithmPhase1> algorithm_phase1;
	void CaclulateAlgorithmForPhase1();
	void CalculatePhase1();

	AlgorithmPhase2 CheckIfWeNeedPhase2();
	void Phase2Algo3();

};



#endif CLIENT_H