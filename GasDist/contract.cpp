#include "contract.h"
#include <iostream>

Contract::Contract(const string& name, const string& c_g, const int o_p, const int u_p, const double offset)
	: name{ name }, contracts_group{ c_g },
	overlimit_priority{ o_p }, underlimit_priority{ u_p }, offset_plan{ offset },
	squares{}
{
	/*
	cout << "Contract Name=" << name << " CG=" << contracts_group << " O-p=" << overlimit_priority << " U-p=" << underlimit_priority << " Offset=" << offset_plan << endl;
	*/
	vector<string> query_result{};

	{
		Db data_base{ Constants::DB_NAME };
		query_result = data_base.GetSquares(name);
	}

	for (auto q = query_result.cbegin(); q != query_result.cend(); q += 4, q += 31, q += 31) {
		vector<double> plan(Constants::DAYS_IN_MONTH + 1, 0);
		for (int i = 4; i <= 4 + (Constants::DAYS_IN_MONTH - 1); i++) plan[i - 3] = StringToDouble(q[i]);

		vector<double> fact(Constants::DAYS_IN_MONTH + 1, 0);
		for (int i = 35; i <= 35 + (Constants::DAYS_IN_MONTH - 1); i++) fact[i - 34] = StringToDouble(q[i]);

		Square tmp_square(q[0], q[1], q[2], q[3], Constants::DAYS_IN_MONTH, plan, fact);
		squares.push_back(tmp_square);
	}
};

int Contract::GetOLP() const {
	return this->overlimit_priority;
}

int Contract::GetULP() const {
	return this->underlimit_priority;
}

double Contract::GetMonthlyFact() const {
	double ret_val{ 0 };
	for (auto s : squares) ret_val += s.GetMonthlyFact();

	// self check
	double tmp_val{ 0 };
	for (int day = 1; day <= Constants::DAYS_IN_MONTH; ++day) tmp_val += this->GetDailyFact(day);
	assert(ret_val - tmp_val < 0.00001);

	return ret_val;
}

double Contract::GetMonthlyPlan() const {
	double ret_val{ 0 };
	for (auto s : squares) ret_val += s.GetMonthlyPlan();

	// self check
	double tmp_val{ 0 };
	for (int day = 1; day <= Constants::DAYS_IN_MONTH; ++day) tmp_val += this->GetDailyPlan(day);
	assert(ret_val - tmp_val < 0.00001);

	return ret_val;
}

double Contract::GetDailyFact(int day_of_interest) const {
	double ret_val{ 0 };
	for (auto s : squares) ret_val += s.GetDailyFact(day_of_interest);
	return ret_val;
}

double Contract::GetDailyPlan(int day_of_interest) const {
	double ret_val{ 0 };
	for (auto s : squares) ret_val += s.GetDailyPlan(day_of_interest);
	return ret_val;
}

double Contract::GetDailyOffsetPlan(int day_of_interest) const {
	return Round1000(this->GetDailyPlan(day_of_interest) * offset_plan);
}
