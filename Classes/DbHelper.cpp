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
	std::string dbFile = CCFileUtils::getInstance()->getWritablePath()+ "default.db";
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
			result = 0;
			log("SQL execution error. cmd :%s / code :%d / cause :%s", sql.c_str(), result, errMsg);
		}else {
			result = 1;
			log("SQL execution success. cmd :%s", sql.c_str());
		}
		sqlite3_free(errMsg);
	}else{
		result = 0;
		log("SQL execution error. cmd :%s / cause :%s", sql.c_str(),"Database is not opened");
	}
	return result;
}

int DbHelper::insert(const string& dbName, unordered_map<string, sqlData>& data) {
	int result;
	int stmtIndex;
	sqlite3_stmt* stmt = NULL;

	//sql should be INSERT INTO TABLE_NAME [(column1, column2, column3,...columnN)] VALUES(?, ?, ?, ...?); 
	string sql = "INSERT INTO " + dbName + " (";

	unordered_map<string, sqlData>::iterator it;
	it = data.begin();

	while (it != data.end())
	{
		if (it == data.begin()) {
			sql += it->first;
		}
		else {
			sql += "," + it->first;
		}
		it++;
	}
	sql += ") VALUES(";
	it = data.begin();
	while (it != data.end())
	{
		if (it == data.begin()) {
			sql += "?";
		}
		else {
			sql += ",?";
		}
		it++;
	}
	sql += ")";
	//log(sql.c_str());

	if (!isConnected()) {
		log("SQL error. cmd :%s / cause :%s", sql.c_str(), "Database is not opened");
	}

	if (sqlite3_prepare_v2(m_db, sql.c_str(), strlen(sql.c_str()), &stmt, NULL) != SQLITE_OK) {
		if (stmt) {
			sqlite3_finalize(stmt);
		}
		log("SQL preparation error. cmd :%s", sql.c_str());
		return 0;
	}

	it = data.begin();
	stmtIndex = 1;
	while (it != data.end())
	{
		if (it->second._sqlDataType == sqlDataType::LONG) {
			if (sqlite3_bind_int64(stmt, stmtIndex, it->second._longData) != SQLITE_OK) {
				log("SQL bind int64 error. cmd :%s column %s", sql.c_str(), it->first.c_str());
				sqlite3_finalize(stmt);
				return 0;
			}
		}else if (it->second._sqlDataType == sqlDataType::DOUBLE) {
			if (sqlite3_bind_double(stmt, stmtIndex, it->second._doubleData) != SQLITE_OK) {
				log("SQL bind double error. cmd :%s column %s", sql.c_str(), it->first.c_str());
				sqlite3_finalize(stmt);
				return 0;
			}
		}else if (it->second._sqlDataType == sqlDataType::TEXT) {
			if (sqlite3_bind_text(stmt, stmtIndex, it->second._textData.c_str(), strlen(it->second._textData.c_str()), SQLITE_STATIC) != SQLITE_OK) {
				log("SQL bind text error. cmd :%s column %s", sql.c_str(), it->first.c_str());
				sqlite3_finalize(stmt);
				return 0;
			}
		}else {
			log("SQL bind error. cmd :%s type for %s is wrong", sql.c_str(), it->first.c_str());
			sqlite3_finalize(stmt);
			return 0;
		}
		it++;
		stmtIndex++;
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

int DbHelper::read(const string& dbName, unordered_map<string, sqlData> fieldData, vector<unordered_map<string, sqlData>>& result, string sqlSuffix) {
	sqlite3_stmt* stmt = NULL;

	string sql = "SELECT ";
	unordered_map<string, sqlData>::iterator it;
	it = fieldData.begin();

	while (it != fieldData.end())
	{
		if (it == fieldData.begin()) {
			sql += it->first;
		}
		else {
			sql += "," + it->first;
		}
		it++;
	}
	sql += " FROM " + dbName;
	if (sqlSuffix != "") {
		sql += " " + sqlSuffix;
	}
	
	//log(sql.c_str());

	if (!isConnected()) {
		log("SQL error. cmd :%s / cause :%s", sql.c_str(), "Database is not opened");
	}
	if (sqlite3_prepare_v2(m_db, sql.c_str(), strlen(sql.c_str()), &stmt, NULL) != SQLITE_OK) {
		if (stmt) {
			sqlite3_finalize(stmt);
		}
		log("SQL preparation error. cmd :%s", sql.c_str());
		return 0;
	}
	int colIndex;
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		colIndex = 0;
		it = fieldData.begin();

		while (it != fieldData.end())
		{
			if (it->second._sqlDataType == sqlDataType::LONG) {
				it->second._longData = sqlite3_column_int64(stmt, colIndex);
			}else if (it->second._sqlDataType == sqlDataType::TEXT) {
				it->second._textData = FuncUtil::constCharStarToString(sqlite3_column_text(stmt, colIndex));
			}else if (it->second._sqlDataType == sqlDataType::DOUBLE) {
				it->second._doubleData = sqlite3_column_double(stmt, colIndex);
			}
			colIndex++;
			it++;
		}
		result.push_back(fieldData);
	}
	log("SQL read successful. cmd :%s", sql.c_str());
	sqlite3_finalize(stmt);
	//log(to_string(result.size()).c_str());
}

string DbHelper::sqlDataPrint(const sqlData& data) {
	if (data._sqlDataType == sqlDataType::TEXT) {
		return data._textData;
	}
	else if (data._sqlDataType == sqlDataType::LONG) {
		return FuncUtil::longToString(data._longData);
	}
	else if (data._sqlDataType == sqlDataType::DOUBLE) {
		return FuncUtil::doubleToString(data._doubleData, 9);
	}
}

void DbHelper::setVersion(int version) {
	string sql = "PRAGMA user_version = " + FuncUtil::intToString(version);
	exec(sql);
}

int DbHelper::getVersion() {
	sqlite3_stmt * stmt;
	int dbVersion;
	const char* cmd = "PRAGMA user_version;";

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

int DbHelper::deleteAll(const string& dbName) {
	return exec("DELETE FROM " + dbName);
}