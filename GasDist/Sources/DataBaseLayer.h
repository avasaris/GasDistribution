#ifndef DATABASELAYER_H
#define DATABASELAYER_H

#include <string>

#include <iostream> // - temporary

#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"
#include "sqlite3.h"

using namespace std;

/* ==== Table structure ====
"Client","Contract", "ContractGroup", "Square", "SquareNumber", "OverLimitPriority", "UnderLimitPriority", "SquareGroup", "DeliveryName", "OffsetPlan", "GoodDistribution", "DataType",
"Day01", "Day02", "Day03", "Day04", "Day05", "Day06", "Day07", "Day08", "Day09", "Day10",
"Day11", "Day12", "Day13", "Day14", "Day15", "Day16", "Day17", "Day18", "Day19", "Day20",
"Day21", "Day22", "Day23", "Day24", "Day25", "Day26", "Day27", "Day28", "Day29", "Day30", "Day31"
*/

const string QUERY_ALL_CLIENTS("SELECT DISTINCT Client FROM RawData");
const string QUERY_ALL_CONTRACTS("SELECT DISTINCT Contract, ContractGroup, OverLimitPriority, UnderLimitPriority, 0 FROM RawData WHERE Client LIKE '%s'");
const string QUERY_OFFSET_FOR_ALL_CONTRACTS("SELECT DISTINCT OffsetPlan FROM RawData WHERE Contract LIKE '%s' AND DataType LIKE 'Plan'");
const string QUERY_ALL_SQUARES(
	"SELECT raw.Square, raw.SquareNumber, raw.SquareGroup, raw.DeliveryName, \
	raw.Day01 as Plan01, raw.Day02 as Plan02, raw.Day03 as Plan03, raw.Day04 as Plan04, raw.Day05 as Plan05, \
	raw.Day06 as Plan06, raw.Day07 as Plan07, raw.Day08 as Plan08, raw.Day09 as Plan09, raw.Day10 as Plan10, \
	raw.Day11 as Plan11, raw.Day12 as Plan12, raw.Day13 as Plan13, raw.Day14 as Plan14, raw.Day15 as Plan15, \
	raw.Day16 as Plan16, raw.Day17 as Plan17, raw.Day18 as Plan18, raw.Day19 as Plan19, raw.Day20 as Plan20, \
	raw.Day21 as Plan21, raw.Day22 as Plan22, raw.Day23 as Plan23, raw.Day24 as Plan24, raw.Day25 as Plan25, \
	raw.Day26 as Plan26, raw.Day27 as Plan27, raw.Day28 as Plan28, raw.Day29 as Plan29, raw.Day30 as Plan30, raw.Day31 as Plan31, \
	rawfact.Day01 as Fact01, rawfact.Day02 as Fact02, rawfact.Day03 as Fact03, rawfact.Day04 as Fact04, rawfact.Day05 as Fact05, \
	rawfact.Day06 as Fact06, rawfact.Day07 as Fact07, rawfact.Day08 as Fact08, rawfact.Day09 as Fact09, rawfact.Day10 as Fact10, \
	rawfact.Day11 as Fact11, rawfact.Day12 as Fact12, rawfact.Day13 as Fact13, rawfact.Day14 as Fact14, rawfact.Day15 as Fact15, \
	rawfact.Day16 as Fact16, rawfact.Day17 as Fact17, rawfact.Day18 as Fact18, rawfact.Day19 as Fact19, rawfact.Day20 as Fact20, \
	rawfact.Day21 as Fact21, rawfact.Day22 as Fact22, rawfact.Day23 as Fact23, rawfact.Day24 as Fact24, rawfact.Day25 as Fact25, \
	rawfact.Day26 as Fact26, rawfact.Day27 as Fact27, rawfact.Day28 as Fact28, rawfact.Day29 as Fact29, rawfact.Day30 as Fact30, rawfact.Day31 as Fact31 \
	FROM rawdata raw \
	LEFT OUTER JOIN rawdata AS rawfact ON \
	(raw.Client=rawfact.Client AND raw.Contract=rawfact.Contract AND raw.SquareNumber=rawfact.SquareNumber AND rawfact.DataType LIKE 'Fact') \
	WHERE raw.DataType LIKE 'Plan' AND raw.Contract LIKE '%s'");

const string INSERT_FINAL_FACT("INSERT INTO RawData (Client, Contract, Square, SquareNumber, DataType, \
	Day01, Day02, Day03, Day04, Day05, Day06, Day07, Day08, Day09, Day10, \
	Day11, Day12, Day13, Day14, Day15, Day16, Day17, Day18, Day19, Day20, \
	Day21, Day22, Day23, Day24, Day25, Day26, Day27, Day28, Day29, Day30, \
	Day31) VALUES ('%s', '%s', '%s', '%s', '%s' %s)");

const string REMOVE_FINAL_FACT("DELETE FROM RawData WHERE Client='%s' AND Contract='%s' AND SquareNumber='%s' AND DataType='FinalFact'");

class Db {
public:
	Db(const string&);
	~Db();

	vector<string> GetClients() const;
	vector<string> GetContracts(const string& client_name) const;
	vector<string> GetSquares(const string& contract_name) const;

	void SaveFactToDB(const string& client_name, const string& contract_name, const string& square_name, const string& square_number, const vector<double>& final_fact) const;

private:
	sqlite3* db;

	const vector<string> SelectToVectorOfStrings (const string& query) const;
	void RemoveFinalFactFromDB(const string& client_name, const string& contract_name, const string& square_number, const string& data_type) const;
};

#endif  DATABASELAYER_H