#include "DataBaseLayer.h"

string ReplaceFacet(const string& str);

Db::Db(const string& db_name) {
	int rc = sqlite3_open(db_name.c_str(), &this->db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	}
}

Db::~Db() {
	sqlite3_close(db);
}

vector<string> Db::GetSquares(const string& contract_name) const {
	string query = boost::str(boost::format(QUERY_ALL_SQUARES) % contract_name);
	vector<string> query_result = SelectToVectorOfStrings(query);

	return query_result;
}

vector<string> Db::GetContracts(const string& client_name) const {
	string query = boost::str(boost::format(QUERY_ALL_CONTRACTS) % client_name);
	vector<string> query_result = SelectToVectorOfStrings(query);
	for (auto q = query_result.begin(); q != query_result.end(); q += 5) {
		string op_query = boost::str(boost::format(QUERY_OFFSET_FOR_ALL_CONTRACTS) % q[0]);

		vector<string> op_query_result = SelectToVectorOfStrings(op_query);
		q[4] = op_query_result.at(0);
	}

	return query_result;
}

vector<string> Db::GetClients() const {
	vector<string> query_result = SelectToVectorOfStrings(QUERY_ALL_CLIENTS);

	return query_result;
}

void Db::SaveFactToDB(const string& client_name, const string& contract_name, const string& square_name, const string& square_number, const vector<double>& final_fact) const {

	std::cout << std::endl << "===================================================" << std::endl;

	this->RemoveFinalFactFromDB(client_name, contract_name, square_number, "FinalFact");

	string insert_values{ "" };

	for (const double &fact : final_fact) {
		std::ostringstream strs;
		strs << fact;
		std::string str = strs.str();
		insert_values += ", '" + ReplaceFacet(str) +"'";
	}

	string query = boost::str(boost::format(INSERT_FINAL_FACT) % client_name % contract_name % square_name % square_number % "FinalFact" % insert_values);
	vector<string> query_result = SelectToVectorOfStrings(query);

	std::cout << std::endl << query << std::endl;

}

void Db::RemoveFinalFactFromDB(const string& client_name, const string& contract_name, const string& square_number, const string& data_type) const {
	string query = boost::str(boost::format(REMOVE_FINAL_FACT) % client_name % contract_name % square_number);
	vector<string> query_result = SelectToVectorOfStrings(query);

	std::cout << std::endl << query << std::endl;
}


/// ================ UTILITIES ================

static int ReturnDataIntoVector(void* data, int argc, char** argv, char** azColName) {
	vector<string>* ret_data = (vector<string>*)data;

	for (int i = 0; i < argc; i++) {
		(*ret_data).push_back(argv[i] ? argv[i] : "NULL");
	}

	return 0;
}

const vector<string> Db::SelectToVectorOfStrings(const string& query) const {
	vector<string> ret_vector;
	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query.c_str(), ReturnDataIntoVector, (void*)& ret_vector, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	return ret_vector;
}

string ReplaceFacet(const string& str) {
	string ret_string{ str };

	for (char& c : ret_string) if (c == '.') c = ',';
	
	return ret_string;
}
