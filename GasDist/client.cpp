#include "client.h"

Client::Client(const string& name)
	: name{ name }, contracts{}
{
	vector<string> query_result{};

	{
		Db data_base{ Constants::DB_NAME };
		query_result = data_base.GetContracts(name);
	}

	cout << "Client: " << name << endl;

	for (auto q = query_result.cbegin(); q != query_result.cend(); q += 5) {
		cout << "Contract: " << q[0] << endl;
		cout << "OLP=" << q[2] << endl;
		cout << "ULP=" << q[3] << endl;
		cout << "Offset=" << q[4] << endl;
		Contract tmp_contract(q[0], q[1], stoi(q[2]), stoi(q[3]), StringToDouble(q[4]));
		contracts.push_back(tmp_contract);
	}

	Reorder();
}

void Client::Reorder() {
	map<int, set<int>> tmp_olp{};
	map<int, set<int>> tmp_ulp{};
	int ind = 0;
	for (auto c : contracts) {
		tmp_olp[c.GetOLP()].insert(ind);
		tmp_ulp[c.GetULP()].insert(ind);
		ind++;
	}
	
	for (auto ind_set : tmp_olp) {
		for (auto ind : ind_set.second) {
			OLP_asc_order.push_back(ind);
		}
	}
	reverse(OLP_asc_order.begin(), OLP_asc_order.end());

	for (auto ind_set : tmp_ulp) {
		for (auto ind : ind_set.second) {
			ULP_desc_order.push_back(ind);
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
			cerr << "{Error in calculating algorithm for pahse 1}";
			assert(false);
		}

		cout << day << " : " << Cli_day_plan << " | " << Cli_day_o_plan << " | " << Cli_day_fact << endl;
	}

}

void Client::CalculatePhase1() {

	for (int day = 1; day <= Constants::DAYS_IN_MONTH; ++day) {
		if (algorithm_phase1[day] == AlgorithmPhase1::N_2_1) {
			double tempo_sum_fact = 0;
			cout << day << " 2." << static_cast<int>(AlgorithmPhase1::N_2_1) << " ";
			for (size_t i = 1; i < OLP_asc_order.size(); ++i) {
				double tempo_fact = contracts[OLP_asc_order[i]].GetDailyOffsetPlan(day);
				contracts[OLP_asc_order[i]].SetDailyFactP1(day, tempo_fact);
				tempo_sum_fact += tempo_fact;
				cout << contracts[OLP_asc_order[i]].GetName() << " " << tempo_fact;
			}
			contracts[OLP_asc_order[Constants::k_min_olp]].SetDailyFactP1(day, GetDailyFact(day) - tempo_sum_fact);
			cout << " " << contracts[OLP_asc_order[Constants::k_min_olp]].GetName() << " " << contracts[OLP_asc_order[Constants::k_min_olp]].GetDailyFactP1(day) << endl;
		}
		else if (algorithm_phase1[day] == AlgorithmPhase1::N_2_2) {
			contracts[ULP_desc_order[Constants::k_max_ulp]].SetDailyFactP1(day, min(GetDailyFact(day), contracts[ULP_desc_order[Constants::k_max_ulp]].GetDailyOffsetPlan(day)));
			// technical debt - you should rewrite this part to be able to work with more than two contracts
			cout << day << " 2." << static_cast<int>(AlgorithmPhase1::N_2_2) << " ";
			contracts[ULP_desc_order[1]].SetDailyFactP1(day, GetDailyFact(day) - contracts[ULP_desc_order[Constants::k_max_ulp]].GetDailyFactP1(day));
			cout << contracts[ULP_desc_order[1]].GetName() << " " << contracts[ULP_desc_order[1]].GetDailyFactP1(day);
			cout << " ";
			cout << contracts[ULP_desc_order[Constants::k_max_ulp]].GetName() << " " << contracts[ULP_desc_order[Constants::k_max_ulp]].GetDailyFactP1(day);
			cout << endl;
		}
		else if (algorithm_phase1[day] == AlgorithmPhase1::N_2_3) {
			contracts[ULP_desc_order[Constants::k_max_ulp]].SetDailyFactP1(day, min(GetDailyFact(day), contracts[ULP_desc_order[Constants::k_max_ulp]].GetDailyPlan(day)));
			// technical debt - you should rewrite this part to be able to work with more than two contracts
			cout << day << " 2." << static_cast<int>(AlgorithmPhase1::N_2_3) << " ";
			contracts[ULP_desc_order[1]].SetDailyFactP1(day, GetDailyFact(day) - contracts[ULP_desc_order[Constants::k_max_ulp]].GetDailyFactP1(day));
			cout << contracts[ULP_desc_order[1]].GetName() << " " << contracts[ULP_desc_order[1]].GetDailyFactP1(day);
			cout << " ";
			cout << contracts[ULP_desc_order[Constants::k_max_ulp]].GetName() << " " << contracts[ULP_desc_order[Constants::k_max_ulp]].GetDailyFactP1(day);
			cout << endl;
		}
		else {
			cerr << "{Error in calculating into phase 1}";
			assert(false);
		}
	}
}

AlgorithmPhase2 Client::CheckIfWeNeedPhase2() {

	bool if_algo1 = false;
	for (size_t i = 1; i < OLP_asc_order.size(); ++i) {
		if_algo1 = if_algo1 || (contracts[OLP_asc_order[i]].GetMonthlyFactP1() > contracts[OLP_asc_order[i]].GetMonthlyPlan());
	}
	if (if_algo1) return AlgorithmPhase2::N_3_1;

	bool if_algo2 = false;
	for (size_t i = 1; i < OLP_asc_order.size(); ++i) {
		if_algo2 = if_algo2 || (contracts[OLP_asc_order[i]].GetMonthlyFactP1() < contracts[OLP_asc_order[i]].GetMonthlyPlan());
	}
	if_algo2 = if_algo2 && (contracts[OLP_asc_order[Constants::k_min_olp]].GetMonthlyFactP1() > contracts[OLP_asc_order[Constants::k_min_olp]].GetMonthlyPlan());
	if (if_algo2) return AlgorithmPhase2::N_3_2;

	bool if_algo3 = false;
	for (size_t i = 1; i < ULP_desc_order.size(); ++i) {
		if_algo3 = if_algo3 || (contracts[ULP_desc_order[i]].GetMonthlyFactP1() > 0);
	}
	if_algo3 = if_algo3 && (contracts[ULP_desc_order[Constants::k_max_ulp]].GetMonthlyFactP1() < contracts[ULP_desc_order[Constants::k_max_ulp]].GetMonthlyPlan());
	if (if_algo3) return AlgorithmPhase2::N_3_3;

	return AlgorithmPhase2::N_3_0;
}

void Client::Phase2Algo3() {
	// technical debt - you should rewrite this part to be able to work with more than two contracts
	// at this moment we think threre is only one contract k_min_ulp=1
	int k_min_ulp = 1;
	double current_F = contracts[ULP_desc_order[k_min_ulp]].GetMonthlyFactP1();
	double current_F_max_ulp = contracts[ULP_desc_order[Constants::k_max_ulp]].GetMonthlyFactP1();
	double current_P_max_ulp = contracts[ULP_desc_order[Constants::k_max_ulp]].GetMonthlyPlan();
	for (int day = 1; day <= Constants::DAYS_IN_MONTH; ++day) {
		double current_f_d_c = contracts[ULP_desc_order[k_min_ulp]].GetDailyFactP1(day);
		double current_f_d = this->GetDailyFact(day); 
		double current_p_ofs_d_max_ulp = contracts[ULP_desc_order[Constants::k_max_ulp]].GetDailyOffsetPlan(day);
		double current_f_d_max_ulp = contracts[ULP_desc_order[Constants::k_max_ulp]].GetDailyFactP1(day);
		std::initializer_list<double> ilist{ current_f_d_c, current_F, current_P_max_ulp - current_F_max_ulp, max(min(current_f_d,current_p_ofs_d_max_ulp)- current_f_d_max_ulp,0.0)};
		double new_current_f_d_c = current_f_d_c - min(ilist);
		contracts[ULP_desc_order[k_min_ulp]].SetDailyFactP2(day, new_current_f_d_c);
		cout << "day=" << day << " " << contracts[ULP_desc_order[k_min_ulp]].GetName() << " " << contracts[ULP_desc_order[k_min_ulp]].GetDailyFactP2(day) << "\t";

		current_F -= (current_f_d_c - new_current_f_d_c);
		current_F_max_ulp += (current_f_d_c - new_current_f_d_c);
		current_f_d_max_ulp += (current_f_d_c - new_current_f_d_c);
		contracts[ULP_desc_order[Constants::k_max_ulp]].SetDailyFactP2(day, current_f_d_max_ulp);
		cout << contracts[ULP_desc_order[Constants::k_max_ulp]].GetName() << " " << contracts[ULP_desc_order[Constants::k_max_ulp]].GetDailyFactP2(day) << "\n";
	}
	
}

void Client::Phase2Algo2() {
	// technical debt - you should rewrite this part to be able to work with more than two contracts
	// at this moment we think threre is only one contract k_max_olp=1
	int k_max_olp = 1;
	double current_F = contracts[OLP_asc_order[k_max_olp]].GetMonthlyFactP1();
	double current_P = contracts[OLP_asc_order[k_max_olp]].GetMonthlyPlan();
	double current_F_min_olp = contracts[OLP_asc_order[Constants::k_min_olp]].GetMonthlyFactP1();
	double current_P_min_olp = contracts[OLP_asc_order[Constants::k_min_olp]].GetMonthlyPlan();
	for (int day = 1; day <= Constants::DAYS_IN_MONTH; ++day) {
		double current_f_d_c = contracts[OLP_asc_order[k_max_olp]].GetDailyFactP1(day);
		double current_p_d_c = contracts[OLP_asc_order[k_max_olp]].GetDailyPlan(day);
		double current_p_d_ofs_max_olp = contracts[OLP_asc_order[k_max_olp]].GetDailyOffsetPlan(day);
		double current_f_d_min_olp = contracts[OLP_asc_order[Constants::k_min_olp]].GetDailyFactP1(day);
		if (isgreater(current_p_d_c, current_f_d_c)) {
			std::initializer_list<double> ilist{ current_p_d_ofs_max_olp - current_f_d_c, current_P - current_F, current_F_min_olp - current_P_min_olp, current_f_d_min_olp };
			double new_current_f_d_c = current_f_d_c + min(ilist);
			contracts[OLP_asc_order[k_max_olp]].SetDailyFactP2(day, new_current_f_d_c);
			cout << "day=" << day << " " << contracts[ULP_desc_order[k_max_olp]].GetName() << " " << contracts[ULP_desc_order[k_max_olp]].GetDailyFactP2(day) << "\t";

			current_F += (new_current_f_d_c - current_f_d_c);
			current_F_min_olp -= (new_current_f_d_c - current_f_d_c);
			current_f_d_min_olp -= (new_current_f_d_c - current_f_d_c);
			contracts[ULP_desc_order[Constants::k_min_olp]].SetDailyFactP2(day, current_f_d_min_olp);
			cout << contracts[ULP_desc_order[Constants::k_min_olp]].GetName() << " " << contracts[ULP_desc_order[Constants::k_min_olp]].GetDailyFactP2(day) << "\n";

		}
		else {
			contracts[OLP_asc_order[k_max_olp]].SetDailyFactP2(day, current_f_d_c);
			cout << "day=" << day << " " << contracts[ULP_desc_order[k_max_olp]].GetName() << " " << contracts[ULP_desc_order[k_max_olp]].GetDailyFactP2(day) << "\t";
			contracts[ULP_desc_order[Constants::k_min_olp]].SetDailyFactP2(day, current_f_d_min_olp);
			cout << contracts[ULP_desc_order[Constants::k_min_olp]].GetName() << " " << contracts[ULP_desc_order[Constants::k_min_olp]].GetDailyFactP2(day) << "\n";
		}
	}

}

void Client::SplitFactToSquares() {
	for (int day = 1; day <= Constants::DAYS_IN_MONTH; ++day) {
		map<string, double> c_summ{};
		for (auto contract : contracts) {
			map<string, double> c_one = contract.GetEachSquareFact(day);
			for (const auto& [name, fact] : c_one) {
				c_summ[name] += fact;
			}
		}
		for (auto contract : contracts) {
			contract.SplitFactToSquares(day, c_summ);
		}
	}

}

void Client::SaveFactToDB() {

}

void Client::Phase2Algo1() {
}
