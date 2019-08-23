#include "DbHelper.h"
#include "FuncUtil.h"

DbHelper* DbHelper::m_instance = new DbHelper;
sqlite3* DbHelper::m_db = NULL;
char* DbHelper::errMsg = NULL;
int DbHelper::m_db_version = 1;

DbHelper* DbHelper::getInstance() {
	return m_instance;
}

bool DbHelper::connect() {
	if (isConnected()) {
		return true;
	}
	std::string dbFile = CCFileUtils::sharedFileUtils()->getWritablePath()+ "default.db";
	int result = sqlite3_open(dbFile.c_str(), &m_db);
	if (result == SQLITE_OK) {
		log("Database open successful.");
		onUpgrade();
		return true;
	}else{
		log("Database open failed. code :%d / cause :%s", result, sqlite3_errmsg(m_db));
		return false;
	}
}

bool DbHelper::isConnected() {
	if (m_db) {
		return true;
	}else{
		return false;
	}
}

void DbHelper::onCreate() {

}

void DbHelper::onUpgrade() {
	int oldVersion = getVersion();
	if (oldVersion < m_db_version) {
		for (int i = oldVersion + 1; i <= m_db_version; i++) {
			switch (i) {
			case 1:
				log("upgrading db ver 1");
				if (exec("CREATE TABLE calc_history (\
					id	INTEGER PRIMARY KEY AUTOINCREMENT,\
					time	INTEGER NOT NULL,\
					calc_str	TEXT NOT NULL,\
					is_deleted	INTEGER NOT NULL DEFAULT 0,\
					delete_time	INTEGER NOT NULL DEFAULT 0,\
					description	TEXT NOT NULL DEFAULT \"\"\
				);") != SQLITE_OK) return;
				break;
			default:
				break;
			}
		}
	
		setVersion(m_db_version);
	}
}

void DbHelper::startTrans() {
	exec("begin transaction");
}

void DbHelper::rollback() {
	exec("rollback transaction");
}

void DbHelper::commit() {
	exec("commit transaction");
}

void DbHelper::close() {
	if (isConnected()) {
		sqlite3_close(m_db);
		m_db = NULL;
		log("Database close successful.");
	}
}

int DbHelper::exec(const string& sql) {
	int result;
	if (isConnected()) {
		result = sqlite3_exec(m_db, sql.c_str(), NULL, NULL, &errMsg);
		if (result != SQLITE_OK) {
			result = 1;
			log("SQL execution error. cmd :%s / code :%d / cause :%s", sql.c_str(), result, errMsg);
		}else {
			result = 0;
			log("SQL execution success. cmd :%s", sql.c_str());
		}
		sqlite3_free(errMsg);
	}else{
		result = 0;
		log("SQL execution error. cmd :%s / cause :%s", sql.c_str(),"Database is not opened");
	}
	return result;
}

int DbHelper::insert(const string& dbName, string(*para)[3], int paraLen) {
	int result;
	int i,j;
	sqlite3_stmt* stmt = NULL;

	//sql should be INSERT INTO TABLE_NAME [(column1, column2, column3,...columnN)] VALUES(?, ?, ?, ...?); 
	string sql = "INSERT INTO " + dbName + " (";
	for (i = 0; i < paraLen; i++) {
		if (i == 0) {
			sql += para[i][0];
		}else{
			sql += "," + para[i][0];
		}
	}
	sql += ") VALUES(";
	for (i = 0; i < paraLen; i++) {
		if (i == 0) {
			sql += "?";
		}
		else {
			sql += ",?";
		}
	}
	sql += ")";
	//log(sql.c_str());

	if (!isConnected()) {
		log("SQL error. cmd :%s / cause :%s", sql, "Database is not opened");
	}

	if (sqlite3_prepare_v2(m_db, sql.c_str(), strlen(sql.c_str()), &stmt, NULL) != SQLITE_OK) {
		if (stmt) {
			sqlite3_finalize(stmt);
		}
		log("SQL preparation error. cmd :%s", sql.c_str());
		return 0;
	}

	for (i = 0; i < paraLen; i++) {
		j = i + 1;
		if (para[i][1] == "int") {
			if (sqlite3_bind_int(stmt, j, FuncUtil::stringToInt(para[i][2])) != SQLITE_OK) {
				log("SQL bind int error. cmd :%s column %s", sql.c_str(), para[i][0]);
				sqlite3_finalize(stmt);
				return 0;
			}
		}
		else if (para[i][1] == "text") {
			if (sqlite3_bind_text(stmt, j, para[i][2].c_str(), strlen(para[i][2].c_str()), SQLITE_STATIC) != SQLITE_OK) {
				log("SQL bind text error. cmd :%s column %s", sql.c_str(), para[i][0]);
				sqlite3_finalize(stmt);
				return 0;
			}
		}
		else if (para[i][1] == "double") {
			if (sqlite3_bind_double(stmt, j, FuncUtil::stringToDouble(para[i][2])) != SQLITE_OK) {
				log("SQL bind double error. cmd :%s column %s", sql.c_str(), para[i][0]);
				sqlite3_finalize(stmt);
				return 0;
			}
		}
		else {
			log("SQL bind error. cmd :%s type %s for %s is wrong", sql.c_str(), para[i][1], para[i][0]);
			sqlite3_finalize(stmt);
			return 0;
		}
	}

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		log("SQL step error. cmd :%s", sql.c_str());
		sqlite3_finalize(stmt);
		return 0;
	}
	log("SQL insert successful. cmd :%s", sql.c_str());
	sqlite3_finalize(stmt);
	return 1;
}

void DbHelper::setVersion(int version) {
	string sql = "PRAGMA user_version = " + FuncUtil::intToString(version);
	exec(sql);
}

int DbHelper::getVersion() {
	sqlite3_stmt * stmt;
	int dbVersion;
	char* cmd = "PRAGMA user_version;";

	if (isConnected()) {
		if (sqlite3_prepare_v2(m_db, cmd, strlen(cmd), &stmt, NULL) == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				dbVersion = sqlite3_column_int(stmt, 0);
			}
		}
		else {
			log("SQL preparation error. cmd :%s / cause :%s", cmd, sqlite3_errmsg(m_db));
		}
		sqlite3_finalize(stmt);

		return dbVersion;
	}
	else {
		log("SQL error. cmd :%s / cause :%s", cmd, "Database is not opened");
		return INT_MAX;
	}

	
}