#include <iostream>
#include <vector>
#include <algorithm>
#include <windows.h>

#include "sqlite3.h"
#include "DataBaseLayer.h"

#pragma execution_character_set("utf-8")
const char* DB_NAME = "Energopromservice.sqlite";

using namespace std;

const vector<string> tableStructure { "Client", 
"Contract", "ContractGroup", "Square", "SquareNumber", "OverLimitPriority", "UnderLimitPriority", "SquareGroup", "DeliveryName", "OffsetPlan", "GoodDistribution", "DataType", 
"Day01", "Day02", "Day03", "Day04", "Day05", "Day06", "Day07", "Day08", "Day09", "Day10", "Day11", "Day12", "Day13", "Day14", "Day15", "Day16", "Day17", "Day18", "Day19", "Day20", "Day21", "Day22", "Day23", "Day24", "Day25", "Day26", "Day27", "Day28", "Day29", "Day30", "Day31" };

double StringToDouble(const string&);

class Square {
public:
	Square(string, string, vector<double>&, vector<double>&, int);
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
};

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

/// ================ CLIENTS ================

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
}

/// ================ CONTRACTS ================

Contract::Contract(const string& name, const string& c_g, const int o_p, const int u_p, const double offset)
	: name{ name }, contracts_group{ c_g },
	overlimit_priority{ o_p }, underlimit_priority{ u_p }, offset_plan{ offset },
	squares{}
{
	cout << "Name=" << name << " CG=" << contracts_group << " O-p=" << overlimit_priority << " U-p=" << underlimit_priority << " Offset=" << offset_plan << endl;
};

/// ================ SQUARES ================

Square::Square(string name, string number, vector<double>& new_plan, vector<double>& new_fact, int days_in_month)
	: name{ name }, number{ number }, days_in_month{ days_in_month },
	plan{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
	fact{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
{
	cout << "Address=" << name << " Number=" << number << endl;
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

/// ================ UTILITIES ================

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
