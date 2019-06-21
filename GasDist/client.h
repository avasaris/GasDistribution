#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>
#include "InitValues.h"
#include "contract.h"

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

	const string& GetName() const { return name; };

	void LinkToCaclulateAlgorithmForPhase1() { 
		cout << " Daily plan | Daily plan offset | Daily fact" << endl;
		CaclulateAlgorithmForPhase1(); 
	};

	void LinkToCalculatePhase1() {
		cout << "--- Phase 1 ---" << endl;
		CalculatePhase1();
	};
	
	//some Proxies to private members.  We should remove it later.
	AlgorithmPhase2 LinkToCheckIfWeNeedPhase2() { return CheckIfWeNeedPhase2(); };
	void LinkToPhase2Algo1() { return Phase2Algo1(); };
	void LinkToPhase2Algo2() { return Phase2Algo2(); };
	void LinkToPhase2Algo3() { return Phase2Algo3(); };
	void LinkToSplitFactToSquares() { return SplitFactToSquares(); };
	void LinkToSaveFactToDB() { return SaveFactToDB(); };

private:
	string name;
	vector<Contract> contracts;

	vector<int> OLP_asc_order;  // k_min_olp is the first element
	vector<int> ULP_desc_order; // k_max_ulp is the first element
	void Reorder();

	vector<AlgorithmPhase1> algorithm_phase1;
	void CaclulateAlgorithmForPhase1();
	void CalculatePhase1();

	AlgorithmPhase2 CheckIfWeNeedPhase2();
	void Phase2Algo1();
	void Phase2Algo2();
	void Phase2Algo3();

	void SplitFactToSquares();
	void SaveFactToDB() const;

};



#endif CLIENT_H