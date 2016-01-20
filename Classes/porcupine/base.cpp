#include "basedef.h"

bool loadJsonFile(const char* filename, rapidjson::Document& doc)
{
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
    
    doc.Parse(str.c_str());
    
    if (doc.HasParseError()) {
        CCLOG("loadJsonFile(%s) err %d", filename, doc.GetParseError());
        
        return false;
    }
    
    return true;
}