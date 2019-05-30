#include "client.h"

Client::Client(const string& name)
	: name{ name }, contracts{}
{
	vector<string> query_result{};

	{
		Db data_base{ DB_NAME };
		query_result = data_base.GetContracts(name);
	}

	for (auto q = query_result.cbegin(); q != query_result.cend(); q += 5) {
		Contract tmp_contract(q[0], q[1], stoi(q[2]), stoi(q[3]), StringToDouble(q[4]));
		contracts.push_back(tmp_contract);
	}

	ResortContracts();
}

void Client::ResortContracts() {
	map<int, set<int>> tmp_olp{};
	map<int, set<int>> tmp_ulp{};
	int ind = 0;
	for (auto c : contracts) {
		tmp_olp[c.MyOverlimitPri()].insert(ind);
		tmp_ulp[c.MyUnderlimitPri()].insert(ind);
		ind++;
	}
	for (auto ind_set : tmp_olp) {
		for (auto ind : ind_set.second) {
			contracts_in_overlimit_priority.push_back(ind);
		}
	}
	for (auto ind_set : tmp_ulp) {
		for (auto ind : ind_set.second) {
			contracts_in_underlimit_priority.push_back(ind);
		}
	}
}
