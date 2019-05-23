#include <iostream>
#include <vector>
#include "sqlite3.h"
#include <cstdio>
#include <windows.h>

#pragma execution_character_set( "utf-8" )

using namespace std;

const vector<string> tableStructure{ "Client", 
"Contract", "ContractGroup", "Square", "SquareNumber", "OverLimitPriority", "UnderLimitPriority", "SquareGroup", "DeliveryName", "OffsetPlan", "GoodDistribution", "DataType", 
"Day01", "Day02", "Day03", "Day04", "Day05", "Day06", "Day07", "Day08", "Day09", "Day10", "Day11", "Day12", "Day13", "Day14", "Day15", "Day16", "Day17", "Day18", "Day19", "Day20", "Day21", "Day22", "Day23", "Day24", "Day25", "Day26", "Day27", "Day28", "Day29", "Day30", "Day31" };

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
private:
	string name;
	string contracts_group;
	int underload_priority;
	int overload_priority;
	double offset_plan;
	vector<Square> squares;
};

class Client {
private:
	string name;
	vector<Contract> contracts;
};

static int PrintBase(void* data, int argc, char** argv, char** azColName);
static int InitClients(void* data, int argc, char** argv, char** azColName);

int main() {

	SetConsoleOutputCP(CP_UTF8);

	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	string data("Callback function called");

	/* Open database */
	rc = sqlite3_open("Energopromservice.sqlite", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		fprintf(stdout, "Opened database successfully\n");
	}

	/* Create SQL statement */
	string sql ("SELECT DISTINCT 'Client' FROM Energopromservice");

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql.c_str(), PrintBase, (void*)data.c_str(), &zErrMsg);
	//rc = sqlite3_exec(db, sql.c_str(), InitClients, (void*)data.c_str(), &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		/* Run the delivering algorithms */
	}
	sqlite3_close(db);

	return 0;
}

static int PrintBase(void* data, int argc, char** argv, char** azColName) {
	char* dt = (char*)data;
	*dt = 'c';
	
	fprintf(stderr, "%s: ", dt);

	for (int i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}

static int InitClients(void* data, int argc, char** argv, char** azColName) {

	for (int i = 0; i < argc; i++) {

	}

	return 0;
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
