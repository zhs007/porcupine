#ifndef __PWORLD_FDIRSPRITE_HPP__
#define __PWORLD_FDIRSPRITE_HPP__

#include "cocos2d.h"
#include "basedef.h"
#include "basesprite.hpp"

enum FDIR{
    FDIR_RIGHT_DOWN = 0,
    FDIR_RIGHT_UP = 1,
    FDIR_LEFT_UP = 2,
    FDIR_LEFT_DOWN = 3,
};

class FDirSprite : public BaseSprite{
public:
    FDirSprite();
    virtual ~FDirSprite();
public:
    static FDirSprite* create(std::vector<cocos2d::SpriteFrame*>& lst, float frameTime);
public:
    bool init(std::vector<cocos2d::SpriteFrame*>& lst, float frameTime);
    
    void release();
    
    bool onIdle(float dt);
    
    void chgDir(float dx, float dy);
protected:
    std::vector<cocos2d::SpriteFrame*>          m_lstFrames;
    
    float                                       m_lastTime;
    float                                       m_frameTime;
    int                                         m_curIndex;
    //std::string                                 m_curact;
    
    int                                         m_curDir;
    
    int                                         m_frameNums;
};

#endif //__PWORLD_FDIRSPRITE_HPP__
