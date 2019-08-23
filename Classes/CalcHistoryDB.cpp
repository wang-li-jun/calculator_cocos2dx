#include "CalcHistoryDB.h"

string CalcHistoryDB::tbName = "calc_history";

int CalcHistoryDB::add(const string& calcStr) {
	int result;
	DbHelper::connect();
	string data[2][3] = { {"calc_str","text","1+2=3"},{"time","int","1"} };
	result = DbHelper::insert(tbName, data, 2);
	DbHelper::close();
	return result;
}