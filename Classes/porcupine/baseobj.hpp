#ifndef __PWORLD_BASEOBJ_HPP__
#define __PWORLD_BASEOBJ_HPP__

#include "cocos2d.h"
#include "basedef.h"
#include "basesprite.hpp"

class BaseObject{
    friend class WorldLayer;
public:
    BaseObject(WorldLayer& world);
    ~BaseObject();
public:
    void load(const char* filename);
    
    void init(rapidjson::Value& val);
    
    void chgAct(const char* act);
    
    void release();
    
    void onIdle(float dt);
    
    void startPath();
//    void setPath(const char* filename);
    
//    void setPath(RoadPath& rp) {
//        m_path = rp;
//    }
    
    void setPosition(float x, float y);
    
    bool procPath(float dt);
    
    void addActQueue(ActQueueNode& node);
    
    int getActQueueNode(const char* aqnn);
    
    void randPath();
protected:
    std::string                                 m_name;
    
    std::map<std::string, ActionInfo>           m_mapAct;
    
    std::map<std::string, BaseSprite*>          m_mapSpr;
    std::string                                 m_curact;
    
    int                                         m_curaqn;
    
    RoadPath                                    m_path;
    int                                         m_curPathIndex;
    float                                       m_curPathDistance;
    bool                                        m_reverse;
    
    cocos2d::Vec2                               m_curPosition;
    
    WorldLayer&                                 m_world;
    
    std::vector<ActQueueNode>                   m_queueAct;
    
    int                                         m_scalex;
    bool                                        m_isfirstrand;
};

#endif //__PWORLD_BASEOBJ_HPP__
