#include "CalcHistoryDB.h"

string CalcHistoryDB::tbName = "calc_history";

int CalcHistoryDB::add(const string& calcStr) {
	int result;
	DbHelper::connect();
	string data[2][3] = { {"calc_str","text","1+2=3"},{"time","int",FuncUtil::intToString(FuncUtil::getCurrentTimeStamp())} };
	result = DbHelper::insert(tbName, data, 2);
	DbHelper::close();
	return result;
}

