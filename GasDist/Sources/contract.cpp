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

		vector<double> empty_vec(Constants::DAYS_IN_MONTH + 1, 0);
		fact_p1 = { empty_vec };
		fact_p2 = { empty_vec };


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
	for (const auto& s : squares) ret_val += s.GetMonthlyFact();

	// self check
	double tmp_val{ 0 };
	for (int day = 1; day <= Constants::DAYS_IN_MONTH; ++day) tmp_val += this->GetDailyFact(day);
	assert(ret_val - tmp_val < 0.00001);

	return ret_val;
}

double Contract::GetMonthlyPlan() const {
	double ret_val{ 0 };
	for (const auto& s : squares) ret_val += s.GetMonthlyPlan();

	// self check
	double tmp_val{ 0 };
	for (int day = 1; day <= Constants::DAYS_IN_MONTH; ++day) tmp_val += this->GetDailyPlan(day);
	assert(ret_val - tmp_val < 0.00001);

	return ret_val;
}

double Contract::GetDailyFact(int day_of_interest) const {
	double ret_val{ 0 };
	for (const auto& s : squares) ret_val += s.GetDailyFact(day_of_interest);
	return ret_val;
}

double Contract::GetDailyPlan(int day_of_interest) const {
	double ret_val{ 0 };
	for (const auto& s : squares) ret_val += s.GetDailyPlan(day_of_interest);
	return ret_val;
}

double Contract::GetDailyOffsetPlan(int day_of_interest) const {
	return Round1000(this->GetDailyPlan(day_of_interest) * offset_plan);
}

void Contract::SetDailyFactP1(int day, double value) {
	fact_p1[day] = value;
}

void Contract::SetDailyFactP2(int day, double value) {
	fact_p2[day] = value;
}

double Contract::GetDailyFactP1(int day) const {
	return fact_p1[day];
}

double Contract::GetDailyFactP2(int day) const {
	return fact_p2[day];
}

double Contract::GetMonthlyFactP1() const {
	double ret_val{ 0 };
	for (int day = 1; day <= Constants::DAYS_IN_MONTH; ++day) ret_val += this->GetDailyFactP1(day);

	return ret_val;
}

map<string, double> Contract::GetEachSquareFact(int day) const {
	map<string, double> ret_val{};

	for (const auto& square : squares) {
		ret_val[square.GetNumber()] = square.GetDailyFact(day);
	}

	return ret_val;
}

void Contract::SplitFactToSquares(int day, map<string, double> squares_total_fact) {
	cout << "Squares fact overall day=" << day << endl;
	
	double whole_fact{ 0 };
	for (const auto& [sq_number, fact] : squares_total_fact) {
		whole_fact += fact;
	}

	assert(whole_fact > 0.0);

	for (const auto& [sq_number, fact] : squares_total_fact) {
		for (auto& square : squares) {
			if (square.GetNumber() == sq_number) {
				double curr_fact = GetDailyFactP2(day);
				double new_fact = Round1000(curr_fact * fact / whole_fact);
				square.SetDailyFinalFact(day, new_fact);
				cout << this->GetName() << ":\t" << sq_number << ":\t" << new_fact << endl;
			}
		}
	}

	
}

void Contract::SaveFactToDB(const string& client_name) const {
	for (const auto& square : squares) {
		square.SaveFactToDB(client_name, this->GetName());
	}
}