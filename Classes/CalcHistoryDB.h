#ifndef __CALC_HISTORY_DB_H__
#define __CALC_HISTORY_DB_H__

#include "DbHelper.h"
#include "FuncUtil.h"

class CalcHistoryDB {
public:
	static int add(const string& calcStr);
	static vector<unordered_map<string, sqlData>> getFullData();
	static int updateDescriptionById(long id, string& description);
	static int deleteAll();
	static int deleteById(long id);
	static unordered_map<string, sqlData> getItemById(long id);
private:
	static string tbName;

};

#endif

