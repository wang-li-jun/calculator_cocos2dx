#ifndef __DB_HELPER_H__
#define __DB_HELPER_H__

#include "cocos2d.h"
/*
#include "sqlite3\include\sqlite3.h"
#pragma comment(lib, "sqlite3")
*/
#include "sqlite3.h"

USING_NS_CC; 
using namespace std;


enum class sqlDataType
{
	LONG,		// long type
	TEXT,		// text type
	DOUBLE		// double type
};

struct sqlData
{
public:
	sqlData()
	{
	}
	sqlData(sqlDataType dataType)
		:_sqlDataType(dataType)
	{
	}
	sqlData(string str)
		:_textData(str)
	{
		_sqlDataType = sqlDataType::TEXT;
	}

	sqlData(double num)
		:_doubleData(num)
	{
		_sqlDataType = sqlDataType::DOUBLE;
	}

	sqlData(long long num)
		:_longData(num)
	{
		_sqlDataType = sqlDataType::LONG;
	}

	float _doubleData;
	long long _longData;
	string _textData;

	sqlDataType	_sqlDataType;
};

class DbHelper {
public:
	static DbHelper* getInstance();
	static bool connect();
	static bool isConnected();
	static int exec(const string& sql);
	static int insert(const string& dbName, unordered_map<string, sqlData>& data);
	static int read(const string& dbName, unordered_map<string, sqlData> rowData, vector<unordered_map<string, sqlData>>& result, string sqlSuffix="");
	static void exec(const string& sql, int(*callback)(void*, int, char**, char**), void* arg);
	static string sqlDataPrint(const sqlData& data);
	static int deleteAll(const string& dbName);
	static void startTrans();
	static void rollback();
	static void commit();
	static void close();
	static void onUpgrade();
	static void onCreate();
	static void setVersion(int version);
	static int getVersion();
private:
	DbHelper(){};
	static DbHelper* m_instance;
	static sqlite3* m_db; 
	static int m_db_version;
	static char* errMsg;



	class GarbageCollector {
	public:
		~GarbageCollector() {
			if (DbHelper::m_instance) {
				delete DbHelper::m_instance;
				DbHelper::m_instance = nullptr;
			}
		}
	};
	static GarbageCollector gc;
};

#endif
