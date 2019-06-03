#include "client.h"

Client::Client(const string& name)
	: name{ name }, contracts{}
{
	vector<string> query_result{};

	{
		Db data_base{ Constants::DB_NAME };
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

double Client::GetDailyFact(int day_of_interest) const {
	double ret_val{ 0 };
	for (auto c : contracts) ret_val += c.GetDailyFact(day_of_interest);
	return ret_val;
}

double Client::GetDailyPlan(int day_of_interest) const {
	double ret_val{ 0 };
	for (auto c : contracts) ret_val += c.GetDailyPlan(day_of_interest);
	return ret_val;
}

double Client::GetDailyOffsetPlan(int day_of_interest) const {
	double ret_val{ 0 };
	for (auto c : contracts) ret_val += c.GetDailyOffsetPlan(day_of_interest);
	return ret_val;
}

double Client::GetMonthlyFact() const {
	double ret_val{ 0 };
	for (auto c : contracts) ret_val += c.GetMonthlyFact();
	return ret_val;
}

double Client::GetMonthlyPlan() const {
	double ret_val{ 0 };
	for (auto c : contracts) ret_val += c.GetMonthlyPlan();
	return ret_val;
}

void Client::CaclulateAlgorithmForPhase1() {

	algorithm_phase1.resize(Constants::DAYS_IN_MONTH + 1, AlgorithmPhase1::N_2_0);

	for (int day = 1; day <= Constants::DAYS_IN_MONTH; ++day) {
		double Cli_day_plan = GetDailyPlan(day);
		double Cli_day_o_plan = GetDailyOffsetPlan(day);
		double Cli_day_fact = GetDailyFact(day);

		if (Cli_day_fact >= Cli_day_o_plan) {
			algorithm_phase1[day] = AlgorithmPhase1::N_2_1;
		}
		else if (Cli_day_fact < Cli_day_o_plan && Cli_day_fact >= Cli_day_plan) {
			algorithm_phase1[day] = AlgorithmPhase1::N_2_2;
		}
		else if (Cli_day_fact < Cli_day_plan) {
			algorithm_phase1[day] = AlgorithmPhase1::N_2_3;
		}
		else {
			assert(false);
		}

		cout << day << " : " << Cli_day_plan << " | " << Cli_day_o_plan << " | " << Cli_day_fact << " | 2." << static_cast<int>(algorithm_phase1[day]) << endl;
	}

}

void Client::CalculatePhase1() {
	for (size_t cs = 0; cs < contracts.size(); ++cs) {
		vector<double> c(Constants::DAYS_IN_MONTH + 1, 0);
		fact_phase1.push_back(c);
	}

	for (int day = 1; day <= Constants::DAYS_IN_MONTH; ++day) {
		if (algorithm_phase1[day] == AlgorithmPhase1::N_2_1) {
			double tempo_sum_fact = 0;
			cout << day << " 2." << static_cast<int>(AlgorithmPhase1::N_2_1) << " ";
			for (size_t contract_i = 1; contract_i < contracts_in_overlimit_priority.size(); ++contract_i) {
				double tempo_fact = contracts[contracts_in_overlimit_priority[contract_i]].GetDailyOffsetPlan(day);
				fact_phase1[contracts_in_overlimit_priority[contract_i]][day] = tempo_fact;
				tempo_sum_fact += tempo_fact;
				cout << contracts[contracts_in_overlimit_priority[contract_i]].GetName() << " " << tempo_fact;
			}
			fact_phase1[contracts_in_overlimit_priority[0]][day] = GetDailyFact(day) - tempo_sum_fact;
			cout << " " << contracts[contracts_in_overlimit_priority[0]].GetName() << " " << fact_phase1[contracts_in_overlimit_priority[0]][day] << endl;
		}
		else if (algorithm_phase1[day] == AlgorithmPhase1::N_2_2) {
			double tempo_sum_fact = 0;
			cout << day << " 2." << static_cast<int>(AlgorithmPhase1::N_2_2) << " ";
			for (size_t contract_i = 1; contract_i < contracts_in_overlimit_priority.size(); ++contract_i) {
				double tempo_fact = contracts[contracts_in_overlimit_priority[contract_i]].GetDailyPlan(day);
				fact_phase1[contracts_in_overlimit_priority[contract_i]][day] = tempo_fact;
				tempo_sum_fact += tempo_fact;
				cout << contracts[contracts_in_overlimit_priority[contract_i]].GetName() << " " << tempo_fact;
			}
			fact_phase1[contracts_in_overlimit_priority[0]][day] = GetDailyFact(day) - tempo_sum_fact;
			cout << " " << contracts[contracts_in_overlimit_priority[0]].GetName() << " " << fact_phase1[contracts_in_overlimit_priority[0]][day] << endl;
		}
		else if (algorithm_phase1[day] == AlgorithmPhase1::N_2_3) {

		}
		else {
			assert(false);
		}
	}
}