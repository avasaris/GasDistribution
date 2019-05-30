#include "contract.h"

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
		vector<double> plan{};
		for (int i = 4; i <= 34; i++) plan.push_back(StringToDouble(q[i]));

		vector<double> fact{};
		for (int i = 35; i <= 65; i++) fact.push_back(StringToDouble(q[i]));

		Square tmp_square(q[0], q[1], q[2], q[3], Constants::DAYS_IN_MONTH, plan, fact);
		squares.push_back(tmp_square);
	}
};

int Contract::MyOverlimitPri() {
	return this->overlimit_priority;
}

int Contract::MyUnderlimitPri() {
	return this->underlimit_priority;
}

double Contract::GetMonthlyFact() {
	double ret_val{ 0 };
	for (auto s : squares) ret_val += s.GetMonthlyFact();

	// self check
	double tmp_val{ 0 };
	for (int day = 1; day <= Constants::DAYS_IN_MONTH; ++day) tmp_val += this->GetDailyFact(day);
	assert(ret_val == tmp_val);

	return ret_val;
}

double Contract::GetMonthlyPlan() {
	double ret_val{ 0 };
	for (auto s : squares) ret_val += s.GetMonthlyPlan();

	// self check
	double tmp_val{ 0 };
	for (int day = 1; day <= Constants::DAYS_IN_MONTH; ++day) tmp_val += this->GetDailyPlan(day);
	assert(ret_val == tmp_val);

	return ret_val;
}

double Contract::GetDailyFact(int day_of_interest) {
	double ret_val{ 0 };
	for (auto s : squares) ret_val += s.GetDailyFact(day_of_interest);
	return ret_val;
}

double Contract::GetDailyPlan(int day_of_interest) {
	double ret_val{ 0 };
	for (auto s : squares) ret_val += s.GetDailyPlan(day_of_interest);
	return ret_val;
}

double Contract::GetDailyOffsetPlan(int day_of_interest) {
	double ret_val{ 0 };
	for (auto s : squares) ret_val += s.GetDailyPlan(day_of_interest);
	ret_val *= offset_plan;
	return ret_val;
}
