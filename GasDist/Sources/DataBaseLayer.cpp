#include "DataBaseLayer.h"

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

/// ================ UTILITIES ================

static int ReturnDataIntoVector(void* data, int argc, char** argv, char** azColName) {
	vector<string>* ret_data = (vector<string>*)data;

	for (int i = 0; i < argc; i++) {
		(*ret_data).push_back(argv[i] ? argv[i] : "NULL");
	}

	return 0;
}

const vector<string> Db::SelectToVectorOfStrings (const string& query) const {
	vector<string> ret_vector;
	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query.c_str(), ReturnDataIntoVector, (void*)& ret_vector, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	return ret_vector;
}

void Db::SaveFactToDB(const string& client_name, const string& contract_name, const string& square_name, const string& square_number, const vector<double>& final_fact) const {

	//RemoveFinalFactFromDB(client_name, contract_name, square_name, square_number, "FinalFact");

	string insert_values{ "" };
	insert_values += "'" + client_name + "', ";
	insert_values += "'" + contract_name + "', ";
	insert_values += "'" + square_name + "', ";
	insert_values += "'" + square_number + "', ";
	insert_values += "'FinalFact'";

	for (const double &fact : final_fact) {
//		insert_values += ", '" + boost::lexical_cast<std::string>(fact) + "'";
		std::ostringstream strs;
		strs << fact;
		std::string str = strs.str();
		insert_values += ", '" + str + "'";
	}

	string query = boost::str(boost::format(INSERT_CALCULATED_FACT) % insert_values);

	std::cout << endl << "===================================================" << endl << query;

}