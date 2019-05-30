#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include "InitValues.h"

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

private:
	string name;
	vector<Contract> contracts;

	vector<int> contracts_in_overlimit_priority;
	vector<int> contracts_in_underlimit_priority;
	void ResortContracts();

};


class ClientCalculations : private Client {

};

#endif CLIENT_H