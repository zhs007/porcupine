#ifndef __PWORLD_WORLD_HPP__
#define __PWORLD_WORLD_HPP__

#include "cocos2d.h"
#include "baseobj.hpp"
#include "mask.hpp"

class WorldLayer{
    friend class BaseObject;
public:
    WorldLayer();
    ~WorldLayer();
public:
    //void addObj(BaseObject* pObj);
    
    BaseObject* addObj(rapidjson::Value& val);
public:
    void load(const char* filename);
    
    void release();
    
    void createLayer(const char* name, const char* type, const char* filename);
    
    void createMask(const char* name, const char* filename);
    
    void setRoot(cocos2d::Layer* pLayer);
    
    void onIdle(float ot);
    
    const Mask* getMask(const char* name) {
        std::map<std::string, Mask*>::const_iterator it = m_mapMask.find(name);
        if (it != m_mapMask.end()) {
            return it->second;
        }
        
        return NULL;
    }
protected:
    std::string                             m_name;
    
    std::vector<BaseObject*>                m_lstObj;
    
    std::map<std::string, cocos2d::Layer*>  m_mapLayer;
    
    std::map<std::string, Mask*>             m_mapMask;
};

#endif //__PWORLD_WORLD_HPP__