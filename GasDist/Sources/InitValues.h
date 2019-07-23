#ifndef INITVALUES_H
#define INITVALUES_H

#include <string>
#include "DataBaseLayer.h"

using namespace std;

namespace Constants {
	const int DAYS_IN_MONTH{ 30 };
//	const char* const DB_FOLDER{ "Databases\\" };
//	const char* const DB_NAME{ "Databases\\Dorinda.sqlite" };
	const char* const DB_NAME{ "Databases\\Energopromservice.sqlite" };
	const int k_min_olp{ 0 };
	const int k_max_ulp{ 0 };
}

double StringToDouble(const string&);
double Round1000(double);


#endif INITVALUES_H