#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

namespace Const {
	const char* const csv_file{"Input.csv"};

};

using namespace std;
vector<string> split_at_semicolon(const string& row);

class CSVConnect {
private:
	ifstream& fileStream;

	CSVConnect(ifstream& ifs) : fileStream(ifs) {}
	CSVConnect& operator=(CSVConnect&);
	CSVConnect(const CSVConnect&);
public:
	static CSVConnect& instance(const string& fileName) {
		ifstream fileStream(fileName);
		static CSVConnect csv_conn(fileStream);
		return csv_conn;
	}
	ifstream& getFileStream() { return fileStream; };
};

class CSVfile {
private:
	CSVConnect& csv;
	map <string, int> HeaderInfo;

	void initHeaderInfo();

public:
	CSVfile(CSVConnect& csv) : csv(csv) {
		initHeaderInfo();

	}
	

};

int main() {
	CSVfile csv(CSVConnect::instance(Const::csv_file));

	



	return 0;
}

void CSVfile::initHeaderInfo()
{
	string row0;
	getline(csv.getFileStream(), row0);
	vector<string> tmpV = split_at_semicolon(row0);
	int i = 0;
	for (auto s : tmpV) HeaderInfo[s] = i++;

}


vector<string> split_at_semicolon(const string& row)
{
	vector<string> res;
	istringstream buf(row);
	string s;
	while (getline(buf, s, ';')) res.push_back(s);
	return res;
}