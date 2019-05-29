#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <windows.h>

#include "sqlite3.h"
#include "DataBaseLayer.h"

#pragma execution_character_set("utf-8")
const char* DB_NAME = "Energopromservice.sqlite";
const int DAYS_IN_MONTH = 30;

using namespace std;

double StringToDouble(const string&);

class Square {
public:
	Square(const string&, const string&, const string&, const string&, const int, const vector<double>&, const vector<double>&);
	double GetFullFact();
	double GetFullPlan();
	double GetFactForDay(int);
	double GetPlanForDay(int);
private:
	string name;
	string number;
	vector<double> plan;
	vector<double> fact;
	int days_in_month;
	string squares_group;
	string delivery_type;
};

class Contract {
public:
	Contract(const string&, const string&, const int, const int, const double);
	int MyOverlimitPri();
	int MyUnderlimitPri();
	
	double GetFullFact();
	double GetFullPlan();
	double GetFactForDay(int);
	double GetPlanForDay(int);
private:
	string name;
	string contracts_group;
	int overlimit_priority;
	int underlimit_priority;
	double offset_plan;
	vector<Square> squares;
};

class Client {
public:
	Client(const string&);
private:
	string name;
	vector<Contract> contracts;

	vector<int> contracts_in_overlimit_priority;
	vector<int> contracts_in_underlimit_priority;
	void ResortContracts();
};

/// ================================ MAIN ================================

int main() {
	SetConsoleOutputCP(CP_UTF8);
	vector<string> clients_names{};

	{
		Db data_base{ DB_NAME };
		clients_names = data_base.GetClients();
	}

	for (auto cname : clients_names) {
		Client client(cname);
	}
	

	return 0;
}

/// ================================ CLIENTS ================================

Client::Client(const string& name)
	: name{ name }, contracts{}
{
	vector<string> query_result{};

	{
		Db data_base{ DB_NAME };
		query_result = data_base.GetContracts(name);
	}

	for (auto q = query_result.cbegin(); q != query_result.cend(); q+=5) {
		Contract tmp_contract(q[0], q[1], stoi(q[2]), stoi(q[3]), StringToDouble(q[4]));
		contracts.push_back(tmp_contract);
	}

	ResortContracts();
}

void Client::ResortContracts() {
	map<int, set<int>> tmp_olp{};
	map<int, set<int>> tmp_ulp{};
	int ind=0;
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

/// ================================ CONTRACTS ================================

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
		Db data_base{ DB_NAME };
		query_result = data_base.GetSquares(name);
	}

	for (auto q = query_result.cbegin(); q != query_result.cend(); q += 4, q += 31, q += 31) {
		vector<double> plan{};
		for (int i = 4; i <= 34; i++) plan.push_back(StringToDouble(q[i]));

		vector<double> fact{};
		for (int i = 35; i <= 65; i++) fact.push_back(StringToDouble(q[i]));

		Square tmp_square(q[0], q[1], q[2], q[3], DAYS_IN_MONTH, plan, fact);
		squares.push_back(tmp_square);
	}
};

int Contract::MyOverlimitPri() {
	return this->overlimit_priority;
}

int Contract::MyUnderlimitPri() {
	return this->underlimit_priority;
}
/// ================================ SQUARES ================================

Square::Square(const string& name, const string& number, const string& s_g, const string& d_t, const int days_in_month, const vector<double>& plan, const vector<double>& fact)
	: name{ name }, number{ number }, squares_group{ s_g }, delivery_type{ d_t }, days_in_month{ days_in_month },
	plan{ plan }, fact{ fact }
{
	/*
	cout << "Square Name=" << name << " Number=" << number << endl;
	cout << "Plan: ";
	for (auto p : plan) {
		cout << p << ", ";
	}
	cout << endl;
	cout << "Fact: ";
	for (auto p : fact) {
		cout << p << ", ";
	}
	cout << endl;
	*/
}

double Square::GetFactForDay(int day_of_interest) {
	return fact.at(day_of_interest);
}

double Square::GetPlanForDay(int day_of_interest) {
	return plan.at(day_of_interest);
}

double Square::GetFullFact() {
	double ret_fact = 0;
	for (int i = 1; i <= days_in_month; ++i) ret_fact += GetFactForDay(i);
	return ret_fact;
}

double Square::GetFullPlan() {
	double ret_plan = 0;
	for (int i = 1; i <= days_in_month; ++i) ret_plan += GetPlanForDay(i);
	return ret_plan;
}

/// ================================ UTILITIES ================================

double StringToDouble(const string& str) {
	std::locale lcl;
	const auto& facet = std::use_facet<std::numpunct<char>>(lcl);
	char dec_p = facet.decimal_point();
	string decimal_point{ dec_p };

	string tmp_str{ "0" + str };
	int n = tmp_str.find(',');
	if(n!=string::npos)
		tmp_str.replace(n, 1, decimal_point);

	double d;
	try {
		d = std::stod(tmp_str);
	}
	catch (const std::invalid_argument&) {
		std::cerr << "Argument is invalid str='" << tmp_str << "'\n";
		throw;
	}
	catch (const std::out_of_range&) {
		std::cerr << "Argument is out of range for a double str='" << tmp_str << "'\n";
		throw;
	}

	return d;
}
