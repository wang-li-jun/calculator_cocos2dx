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
	fieldData["description"] = sqlData(sqlDataType::TEXT);
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

int CalcHistoryDB::deleteAll() {
	int result;
	DbHelper::connect();
	result = DbHelper::deleteAll(tbName);
	DbHelper::close();
	return result;
}

int CalcHistoryDB::deleteById(long id) {
	int result;
	DbHelper::connect();
	result = DbHelper::deleteById(tbName,id);
	DbHelper::close();
	return result;
}

unordered_map<string, sqlData> CalcHistoryDB::getItemById(long id) {
	vector<unordered_map<string, sqlData>> result;
	int cmdResult;

	DbHelper::connect();

	unordered_map<string, sqlData> fieldData;
	fieldData["id"] = sqlData(sqlDataType::LONG);
	fieldData["time"] = sqlData(sqlDataType::LONG);
	fieldData["calc_str"] = sqlData(sqlDataType::TEXT);
	fieldData["description"] = sqlData(sqlDataType::TEXT);
	cmdResult = DbHelper::read(tbName, fieldData, result,"where id=" + FuncUtil::longToString(id));

	DbHelper::close();

	if (result.size() > 0) {
		return result.at(0);
	}else{
		unordered_map<string, sqlData> blankData;
		return blankData;
	}
}

int CalcHistoryDB::updateDescriptionById(long id, string& description) {
	int result;
	DbHelper::connect();

	unordered_map<string, sqlData> updateData;
	updateData["description"] = sqlData(description);
	result = DbHelper::update(tbName, updateData, "where id=" + FuncUtil::longToString(id));
	DbHelper::close();
	return result;
}
