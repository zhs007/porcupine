#ifndef __PWORLD_ANISPRITE_HPP__
#define __PWORLD_ANISPRITE_HPP__

#include "cocos2d.h"
#include "basedef.h"
#include "basesprite.hpp"

class AniSprite : public BaseSprite{
public:
    AniSprite();
    virtual ~AniSprite();
public:
    static AniSprite* create(std::vector<cocos2d::SpriteFrame*>& lst, float frameTime, bool isloop, float time);
public:
    bool init(std::vector<cocos2d::SpriteFrame*>& lst, float frameTime, bool isloop, float time);
    
//    void setSpeed(int speed) {
//        m_speed = speed;
//    }
    
    void release();
    
    bool onIdle(float dt);
    
    virtual void chgDir(float dx, float dy) {}
protected:
    std::vector<cocos2d::SpriteFrame*>          m_lstFrames;
    
    float                                       m_lastTime;
    float                                       m_frameTime;
    int                                         m_curIndex;
    //std::string                                 m_curact;
    
    //int                                         m_speed;
    
    bool                                        m_isloop;
    float                                       m_anitime;
};

#endif //__PWORLD_ANISPRITE_HPP__
