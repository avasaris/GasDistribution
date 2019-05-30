#ifndef INITVALUES_H
#define INITVALUES_H

#include <string>
#include "square.h"
#include "contract.h"
#include "client.h"
#include "DataBaseLayer.h"

using namespace std;

namespace Constants {
	const int DAYS_IN_MONTH{ 30 };
	const char* const DB_NAME{ "Energopromservice.sqlite" };
	const int algo_2_1{ 21 };
	const int algo_2_2{ 22 };
	const int algo_2_3{ 23 };
}

double StringToDouble(const string&);
double Round1000(double);

#endif INITVALUES_H