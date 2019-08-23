#ifndef __DB_HELPER_H__
#define __DB_HELPER_H__

#include "cocos2d.h"
#include "sqlite3\include\sqlite3.h"
#pragma comment(lib, "sqlite3")

USING_NS_CC; 
using namespace std;


class DbHelper {
public:
	static DbHelper* getInstance();
	static bool connect();
	static bool isConnected();
	static int exec(const string& sql);
	static int insert(const string& dBname, string (*para)[3], int paraLen);
	static void exec(const string& sql, int(*callback)(void*, int, char**, char**), void* arg);
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
