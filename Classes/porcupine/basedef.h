#ifndef __PWORLD_BASEDEF_HPP__
#define __PWORLD_BASEDEF_HPP__

#include "cocos2d.h"
#include "json/document.h"

enum DIRTYPE{
    DIR_ZERO    =   0,
    DIR_ONE     =   1,
    DIR_TWO     =   2
};

struct ActionInfo{
    std::string     name;
    DIRTYPE         dirtype;
    float           frametime;
    int             speed;
    bool            isloop;
    
    std::vector<cocos2d::SpriteFrame*>  lstFrame;
};

struct ActQueueNode{
    std::string     name;
    std::string     act;
    
    float           time;
    std::string     path;
};

typedef std::vector<cocos2d::Vec2> RoadPath;

void loadRoadPath(const char* filename, RoadPath& path);

bool loadJsonFile(const char* filename, rapidjson::Document& doc);

class WorldLayer;

#endif //__PWORLD_BASEDEF_HPP__