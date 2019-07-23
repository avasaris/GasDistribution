#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class CSVfile {
	CSVfile(string fn) : fileName(fn) { initFieldsStructure() };
	const vector<string> getHeader() const;
	const vector<vector<string>> getContent() const;
private:
	string fileName;
	map<string, int> fieldsStructure;

	void initFieldsStructure();
};

void CSVfile::initFieldsStructure() {
	ifstream inpStream;
	string row0;
	getline(inpStream, row0);
	vector<string> values = split_at_semicolon(row0);
}

vector<string> split_at_semicolon(const string& row)
{
	vector<string> res;
	istringstream buf(row);
	string s;
	while (getline(buf, s, ';')) res.push_back(s);
	return res;
}

int main() {


	// строка к файлу
	// строка к базе
	// открытие и чтение csv
	// открытие базы
	// создание соответствующей таблицы в базе
	// закрытие базы






	// вычислим разделитель целой и дробной части в рабочей локали
	std::locale lcl;
	const auto& facet = std::use_facet<std::numpunct<char>>(lcl);
	char decimal_point = facet.decimal_point();

	string row;

	ifstream wInv("Input2.csv");
	if (!wInv.bad()) {
		int lineNum = 0;
		while (getline(wInv, row)) {
			vector<string> values = split_at_semicolon(row);
			if (lineNum > 0) {
				int ContractNum = 0;
				istringstream issCN(values[0]);
				issCN >> ContractNum;

				int GazPointNum = 0;
				istringstream issGPN(values[1]);
				issGPN >> GazPointNum;

				for (int k = 3; k < values.size(); k++) {
					double ftmp = 0.0;
					replace(values[k].begin(), values[k].end(), ',', decimal_point);
					istringstream istmp;
					istmp.str(values[k]);
					if (values[2] == "P") {
						istmp >> plan[ContractNum][GazPointNum][k - 2];
					}
					else if (values[2] == "F") {
						istmp >> fact0[ContractNum][GazPointNum][k - 2];
					}
				}
			}
			lineNum++;
		}
		wInv.close();
	}
	else {
		cout << "Can't open input data file" << endl;
	}
}
