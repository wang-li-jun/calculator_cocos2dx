#include "CalcHistoryDB.h"

string CalcHistoryDB::tbName = "calc_history";

int CalcHistoryDB::add(const string& calcStr) {
	int result;
	DbHelper::connect();

	unordered_map<string, sqlData> data;
	data["calc_str"] = sqlData(calcStr);
	data["time"] = sqlData(FuncUtil::getCurrentTimeStamp());
	result = DbHelper::insert(tbName, data);

	DbHelper::close();
	return result;
}

vector<unordered_map<string, sqlData>> CalcHistoryDB::getFullData() {
	vector<unordered_map<string, sqlData>> result;
	int cmdResult;

	DbHelper::connect();

	unordered_map<string, sqlData> fieldData;
	fieldData["id"] = sqlData(sqlDataType::LONG);
	fieldData["time"] = sqlData(sqlDataType::LONG);
	fieldData["calc_str"] = sqlData(sqlDataType::TEXT);
	cmdResult = DbHelper::read(tbName, fieldData, result);
	
	DbHelper::close();
	//log(to_string(result.size()).c_str());

	/* view data
	for (auto row : result)
	{
		auto column = row.begin();

		while (column != row.end())
		{
			log((column->first + "  " + DbHelper::sqlDataPrint(column->second)).c_str());
			column++;
		}
	}
	*/
	return result;
}

