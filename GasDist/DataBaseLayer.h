#pragma once
#include <string>

const char* DB_NAME = "Energopromservice.sqlite";

using namespace std;

static int ReturnDataIntoVector(void*, int, char**, char**);

class Db {
public:
	Db(const string&);
	~Db();

	vector<string> GetClients();

private:
	sqlite3* db;
	int rc;

	vector<string> SelectToVectorOfStrings(const string& query);
};

Db::Db(const string&) {
	rc = sqlite3_open(DB_NAME, &this->db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	}
}

Db::~Db() {
	sqlite3_close(db);
}

vector<string> Db::GetClients() {
	vector<string> ret_vector = SelectToVectorOfStrings("SELECT DISTINCT Client FROM RawData LIMIT 1");

	return ret_vector;
}

vector<string> Db::SelectToVectorOfStrings(const string& query){
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