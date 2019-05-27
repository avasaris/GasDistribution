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
	Square(string, string, vector<double>&, vector<double>&, int);
	double GetFullFact();
	double GetFullPlan();
	double GetFactForDay(int);
	double GetPlanForDay(int);
private:
	string address;
	string number;
	vector<double> plan;
	vector<double> fact;
	int days_in_month;
	string squares_group;
	string delivery_type;
};

class Contract {
public:
	Contract(const string& name, const string& c_g, const int o_p, const int u_p, const double offset) 
		: name(name), 
		contracts_group(c_g), 
		overlimit_priority(o_p), 
		underlimit_priority(u_p), 
		offset_plan(offset),
		squares{} 
	{ cout << endl << "Name=" << name << " CG=" << contracts_group << " O-p=" << overlimit_priority << " U-p=" << underlimit_priority << " Offset=" << offset_plan;  };
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
	vector<string> clients_names;

	{
		Db data_base{ DB_NAME };
		clients_names = data_base.GetClients();
	}

	for (auto cname : clients_names) {
		Client client(cname);
	}
	

	return 0;
}



	




Client::Client(const string& name) : name(name), contracts{} {

	Db data_base{DB_NAME};

	vector<string> q_res = data_base.GetContracts(name);

	for (auto i = q_res.begin(); i != q_res.end(); i+=5) {
		Contract tmp_contract(i[0],i[1],stoi(i[2]), stoi(i[3]), StringToDouble(i[4]));
		contracts.push_back(tmp_contract);
	}


}

Square::Square(string new_address, string new_number, vector<double>& new_plan, vector<double>& new_fact, int new_days) {
	this->address = new_address;
	this->number = new_number;
	this->days_in_month = new_days;
	this->plan = new_plan;
	this->fact = new_fact;
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
