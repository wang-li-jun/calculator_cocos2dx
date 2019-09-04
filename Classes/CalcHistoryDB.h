#ifndef __CALC_HISTORY_DB_H__
#define __CALC_HISTORY_DB_H__

#include "DbHelper.h"
#include "FuncUtil.h"

class CalcHistoryDB {
public:
	static int add(const string& calcStr);
	static vector<unordered_map<string, sqlData>> getFullData();
private:
	static string tbName;

};

#endif

