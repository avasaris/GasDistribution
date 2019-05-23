#include <iostream>
#include <vector>
#include "sqlite3.h"
#include <cstdio>
#include <windows.h>

#pragma execution_character_set( "utf-8" )

using namespace std;

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

class Castomer {
private:
	string name;
	vector<Contract> contracts;
};

static int callback(void* data, int argc, char** argv, char** azColName);

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
	string sql ("SELECT * from Input_new1_UTF8");

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data.c_str(), &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		//fprintf(stdout, "Table created successfully\n");
	}
	sqlite3_close(db);

	return 0;
}

static int callback(void* data, int argc, char** argv, char** azColName) {
	int i;
	fprintf(stderr, "%s: ", (const char*)data);

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
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
