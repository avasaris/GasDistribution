#ifndef INITVALUES_H
#define INITVALUES_H

#include <string>
#include "square.h"
#include "contract.h"
#include "client.h"
#include "DataBaseLayer.h"

using namespace std;

static const int DAYS_IN_MONTH = 30;
static const char* DB_NAME = "Energopromservice.sqlite";

double StringToDouble(const string&);

#endif INITVALUES_H