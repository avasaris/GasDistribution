#pragma once
#include <string>
#include "boost/format.hpp"

using namespace std;

const string QUERY_ALL_CLIENTS ("SELECT DISTINCT Client FROM RawData");
const string QUERY_ALL_CONTRACTS ("SELECT DISTINCT Contract, ContractGroup, OverLimitPriority, UnderLimitPriority, OffsetPlan FROM RawData WHERE Client LIKE '%s'");
class Contract;

static int ReturnDataIntoVector(void*, int, char**, char**);

class Db {
public:
	Db(const string&);
	~Db();

	vector<string> GetClients();
	vector<string> GetContracts(const string& client_name);

private:
	sqlite3* db;
	int rc;

	vector<string> SelectToVectorOfStrings(const string& query);
};

Db::Db(const string& db_name) {
	rc = sqlite3_open(db_name.c_str(), &this->db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	}
}

Db::~Db() {
	sqlite3_close(db);
}

vector<string> Db::GetContracts(const string& client_name) {
	string query;
	query = boost::str(boost::format(QUERY_ALL_CONTRACTS) % client_name);
	vector<string> query_result = SelectToVectorOfStrings(query);

	return query_result;
}

vector<string> Db::GetClients() {
	vector<string> query_result = SelectToVectorOfStrings(QUERY_ALL_CLIENTS);

	return query_result;
}

vector<string> Db::SelectToVectorOfStrings(const string& query) {
	vector<string> ret_vector;
	char* zErrMsg = 0;

	rc = sqlite3_exec(db, query.c_str(), ReturnDataIntoVector, (void*)& ret_vector, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	return ret_vector;
}

static int ReturnDataIntoVector(void* data, int argc, char** argv, char** azColName) {
	vector<string>* ret_data = (vector<string>*)data;

	for (int i = 0; i < argc; i++) {
		(*ret_data).push_back(argv[i] ? argv[i] : "NULL");
	}

	return 0;
}
