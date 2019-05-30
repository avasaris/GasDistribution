#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>

#include "InitValues.h"

#pragma execution_character_set("utf-8")

using namespace std;


/// ================================== MAIN ===================================

int main() {
	SetConsoleOutputCP(CP_UTF8);
	vector<string> clients_names{};

	{
		Db data_base{ DB_NAME };
		clients_names = data_base.GetClients();
	}

	for (auto cname : clients_names) {
		Client client(cname);
	}
	

	return 0;
}

/// ================================ UTILITIES ================================

double StringToDouble(const string& str) {
	std::locale lcl;
	const auto& facet = std::use_facet<std::numpunct<char>>(lcl);
	char dec_p = facet.decimal_point();
	string decimal_point{ dec_p };

	string tmp_str{ "0" + str };
	int n = tmp_str.find(',');
	if(n!=string::npos)
		tmp_str.replace(n, 1, decimal_point);

	double d;
	try {
		d = std::stod(tmp_str);
	}
	catch (const std::invalid_argument&) {
		std::cerr << "Argument is invalid str='" << tmp_str << "'\n";
		throw;
	}
	catch (const std::out_of_range&) {
		std::cerr << "Argument is out of range for a double str='" << tmp_str << "'\n";
		throw;
	}

	return d;
}
