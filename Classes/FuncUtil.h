#ifndef __FUNC_UTIL_H__
#define __FUNC_UTIL_H__
#include "cocos2d.h"
#include "json/document.h"

class FuncUtil : public Ref
{
public:
	static std::string getLang(std::string name) {
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
};
#endif