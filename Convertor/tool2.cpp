#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

namespace Constants {
	const char* const csv_file{"Dorinda.csv"};

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
	map <string, int> headerInfo;

	void initHeaderInfo();

public:
	CSVfile(CSVConnect& csv) : csv(csv) {
		initHeaderInfo();
	}
	
	void printHeaderInfo();

};

int main() {
	CSVConnect& csvConnector = CSVConnect::instance(Constants::csv_file);

	CSVfile csv(csvConnector);

	//csv.printHeaderInfo();



	return 0;
}

void CSVfile::initHeaderInfo()
{
	string row0;
	ifstream tmpStream = csv.getFileStream();
	getline(csv.getFileStream(), row0);
	vector<string> tmpV = split_at_semicolon(row0);
	int i = 0;
	for (auto s : tmpV) headerInfo[s] = i++;

}

void CSVfile::printHeaderInfo()
{
	for (auto hi : headerInfo) cout << hi.first << "|" << hi.second << endl;
}


vector<string> split_at_semicolon(const string& row)
{
	vector<string> res;
	istringstream buf(row);
	string s;
	while (getline(buf, s, ';')) res.push_back(s);
	return res;
}