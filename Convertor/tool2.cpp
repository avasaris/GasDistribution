#include <iostream>
#include <string>

using namespace std;

class CSVConnect {
private:
	string filename;

	CSVConnect(string fn) filename(fn) {};
	CSVConnect& operator=(CSVConnect&);
	CSVConnect(const CSVConnect&);
public:
	static CSVConnect c;

};

int main() {






	return 0;
}