#ifndef __FUNC_UTIL_H__
#define __FUNC_UTIL_H__
#include "cocos2d.h"
#include "json/document.h"

class FuncUtil : public Ref
{
public:
	static string getLang(const string& name) {
		rapidjson::Document doc;
	
		string filePath = FileUtils::getInstance()->fullPathForFilename("lang/chinese.lang");
		string buffer = FileUtils::getInstance()->getStringFromFile(filePath);

		doc.Parse<0>(buffer.c_str());
		if (doc.HasParseError()) {
			return "ERR";
		}
		else {
			if (doc.HasMember(name.c_str())) {
				return doc[name.c_str()].GetString();
			}
			else {
				return "NNF";
			}
		}
	}

	// ×=>* ÷=>/
	static string markToOperator(const string& str) {
		if (str == FuncUtil::getLang("divideMark")) {
			return "/";
		}else if (str == FuncUtil::getLang("multiplyMark")) {
			return "*";
		}else if (str == FuncUtil::getLang("leftArrowMark")) {
			return "<=";
		}else{
			return str;
		}
	}

	// *=>× /=>÷
	static string operatorToMark(const string& str) {
		string output;
		for (size_t i = 0; i < str.size(); i++) {
			if (str[i] == '*') {
				output += FuncUtil::getLang("multiplyMark");
			}else if (str[i] == '/') {
				output += FuncUtil::getLang("divideMark");
			}
			else {
				output += str[i];
			}
		}
		return output;
	}

	static string intToString(int number) {
		stringstream ss;
		ss << number;
		string result = ss.str();
		return result;
	}

	static int stringToInt(const string& str) {
		return atoi(str.c_str());
	}

	static double stringToDouble(const string& str)
	{
		istringstream iss(str);
		double num;
		iss >> num;
		return num;
	}

	static long long getCurrentTimeStamp() {
		time_t time1 = time(NULL);
		return (long long)time1;
	}

	static string showTime(time_t time) {
		char timebuf[128];
		tm* lt = localtime(&time);
		strftime(timebuf, 128, "%F %H:%M:%S", lt);
		return timebuf;
	}
};
#endif