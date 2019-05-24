#pragma once
#include <string>

const char* DB_NAME = "Energopromservice.sqlite";

using namespace std;

static int ReturnDataIntoString(void*, int, char**, char**);

class Db {
public:
	Db(const string&);
	~Db();

	string GetClients();

private:
	sqlite3* db;
	int rc;

	string SelectToString(const string& query);
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

string Db::GetClients() {
	string ret_string = SelectToString("SELECT DISTINCT Client FROM RawData LIMIT 1");
	return ret_string;
}

string Db::SelectToString(const string& query){
	string ret_string("");
	char* zErrMsg = 0;

	rc = sqlite3_exec(db, query.c_str(), ReturnDataIntoString, (void*)& ret_string, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	return ret_string;
}

static int ReturnDataIntoString(void* data, int argc, char** argv, char** azColName) {
	string* ret_data = (string*)data;

	*ret_data = argv[0];

	return 0;
}