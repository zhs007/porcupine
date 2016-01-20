#ifndef __PWORLD_BASESPRITE_HPP__
#define __PWORLD_BASESPRITE_HPP__

#include "cocos2d.h"
#include "basedef.h"

class BaseSprite : public cocos2d::Sprite{
public:
    BaseSprite() {}
    virtual ~BaseSprite() {}
public:
public:
    static BaseSprite* create();
public:
    bool init();
public:
    virtual bool onIdle(float dt) {}
    
    virtual void chgDir(float dx, float dy) {}
};

#endif //__PWORLD_BASESPRITE_HPP__
