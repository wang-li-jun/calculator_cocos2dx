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
};
#endif