//
//  baseobj.cpp
//  pixelworld
//
//  Created by 赵洪松 on 16/1/6.
//
//

#include "baseobj.hpp"
#include "world.hpp"
#include "anisprite.hpp"
#include "fdirsprite.hpp"
#include "json/document.h"
#include "mask.hpp"

BaseObject::BaseObject(WorldLayer& world)
    : m_world(world)
    , m_curaqn(-1)
    , m_isfirstrand(true)
{
    
}

BaseObject::~BaseObject()
{
    
}

void BaseObject::load(const char* filename)
{
    rapidjson::Document readdoc;
    loadJsonFile(filename, readdoc);
//    ssize_t sz = 0;
//    unsigned char* fd = cocos2d::FileUtils::getInstance()->getFileData(filename, "r", &sz);
//    char* rfd = (char*)malloc(sz + 1);
//    memcpy(rfd, fd, sz);
//    rfd[sz] = '\0';
//    readdoc.Parse(rfd);
//    if (readdoc.HasParseError()) {
//        CCLOG("BaseObject::load(%s) err %d", filename, readdoc.GetParseError());
//        
//        goto errend;
//    }
    
    if(!readdoc.IsObject()) {
        CCLOG("BaseObject::load(%s) file format err", filename);
        
        goto errend;
    }
    
    m_name = readdoc["name"].GetString();
    
    if (readdoc.HasMember("act")) {
        rapidjson::Value& actarr = readdoc["act"];
        if (!actarr.IsArray()) {
            CCLOG("BaseObject::load(%s) act is not array", filename);
            
            goto errend;
        }
        
        for (int i = 0; i < actarr.Size(); ++i) {
            rapidjson::Value& cur = actarr[i];
            if (!cur.IsObject()) {
                CCLOG("BaseObject::load(%s) act[%d] is not object", filename, i);
                
                goto errend;
            }
            
            ActionInfo ai;
            
            ai.name = cur["name"].GetString();
            ai.dirtype = (DIRTYPE)cur["dir"].GetInt();
            ai.frametime = cur["frametime"].GetDouble();
            ai.speed = 0;
            ai.isloop = true;
            
            if (cur.HasMember("loop")) {
                ai.isloop = cur["loop"].GetBool();
            }
            
            if (cur.HasMember("speed")) {
                ai.speed = cur["speed"].GetInt();
            }
            
            int frames = cur["frames"].GetInt();
            std::string filename = cur["filename"].GetString();
            
            if (ai.dirtype == DIR_ONE) {
                for (int j = 0; j < frames; ++j) {
                    char rf[1024];
                    sprintf(rf, filename.c_str(), j);
                    
                    //cocos2d::Sprite* pSpr = cocos2d::Sprite::create(rf);
                    cocos2d::SpriteFrame* pFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(rf);//pSpr->getSpriteFrame();//cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(rf);
                    pFrame->retain();
                    ai.lstFrame.push_back(pFrame);
                }
            }
            else if (ai.dirtype == DIR_TWO) {
                for (int k = 1; k <= 2; ++k) {
                    for (int j = 0; j < frames; ++j) {
                        char rf[1024];
                        sprintf(rf, filename.c_str(), k, j);
                        
                        //cocos2d::Sprite* pSpr = cocos2d::Sprite::create(rf);
                        cocos2d::SpriteFrame* pFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(rf);//cocos2d::SpriteFrame* pFrame = pSpr->getSpriteFrame();//cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(rf);
                        pFrame->retain();
                        ai.lstFrame.push_back(pFrame);
                    }
                }
            }
            else if (ai.dirtype == DIR_ZERO) {
            }
            
            m_mapAct[ai.name] = ai;
        }
    }
    return ;
errend:
    release();
}

void BaseObject::release()
{
    
}

void BaseObject::onIdle(float dt)
{
    if (m_curaqn < 0) {
        BaseSprite* pSpr = m_mapSpr["normal"];
        pSpr->onIdle(dt);
        
        if (m_curact == "walk" && !m_path.empty()) {
            procPath(dt);
        }
    }
    else {
        bool isend = false;
        
        BaseSprite* pSpr = m_mapSpr["normal"];
        isend = pSpr->onIdle(dt);
        
        if (m_curact == "walk" && !m_path.empty()) {
            isend = procPath(dt);
        }
        
        if (isend) {
            m_curaqn++;
            if (m_curaqn >= m_queueAct.size()) {
                m_curaqn = 0;
            }
            
            chgAct(m_queueAct[m_curaqn].name.c_str());
        }
    }
}

void BaseObject::chgAct(const char* act)
{
    std::string curact = act;
    float t = 0;
    
    if (!m_queueAct.empty()) {
        m_curaqn = getActQueueNode(act);
        if (m_curaqn < 0) {
            m_curaqn = 0;
        }
        
        curact = m_queueAct[m_curaqn].act;
        t = m_queueAct[m_curaqn].time;
        
        //CCLOG("%s %d %f", act, m_curaqn, t);
    }
    
    if (m_mapSpr["normal"] != NULL) {
        m_mapSpr["normal"]->getParent()->removeChild(m_mapSpr["normal"]);
    }
    
    if (m_mapAct[curact].dirtype == DIR_ONE) {
        BaseSprite* pSpr = AniSprite::create(m_mapAct[curact].lstFrame, m_mapAct[curact].frametime, m_mapAct[curact].isloop, t);
        m_mapSpr["normal"] = pSpr;
        
        m_world.m_mapLayer["normal"]->addChild(pSpr);
    }
    else if (m_mapAct[curact].dirtype == DIR_TWO) {
        BaseSprite* pSpr = FDirSprite::create(m_mapAct[curact].lstFrame, m_mapAct[curact].frametime);
        m_mapSpr["normal"] = pSpr;
        
        m_world.m_mapLayer["normal"]->addChild(pSpr);
        
        startPath();
    }
    else if (m_mapAct[curact].dirtype == DIR_ZERO) {
        BaseSprite* pSpr = BaseSprite::create();
        m_mapSpr["normal"] = pSpr;
        
        m_world.m_mapLayer["normal"]->addChild(pSpr);
        
        startPath();
    }
    
    setPosition(m_curPosition.x, m_curPosition.y);
    
    m_curact = curact;
    
    if (m_scalex != 0) {
        BaseSprite* pSpr = m_mapSpr["normal"];
        pSpr->setScaleX(m_scalex);
    }
}

//void BaseObject::setPath(const char* filename)
//{
//    loadRoadPath(filename, m_path);
//    
//    m_curPathIndex = 0;
//    m_curPathDistance = 0;
//    
//    FDirSprite* pSpr = (FDirSprite*)(m_mapSpr["normal"]);
//    setPosition(m_path[m_curPathIndex].x, m_path[m_curPathIndex].y);
//    pSpr->chgDir(m_path[m_curPathIndex + 1].x, m_path[m_curPathIndex + 1].y);
//}

void BaseObject::startPath()
{
    if (m_isfirstrand) {
        m_curPathIndex = 0;
        m_isfirstrand = false;
    }
    else {
        m_curPathIndex = rand() % m_path.size();
    }
    
    m_curPathDistance = 0;
    
    m_reverse = false;
    
    if (m_curPosition.x != m_path[m_curPathIndex].x && m_curPosition.y != m_path[m_curPathIndex].y) {
        RoadPath r;
        
        for (RoadPath::reverse_iterator it = m_path.rbegin(); it != m_path.rend(); ++it) {
            r.push_back(*it);
        }
        
        m_path = r;
    }
    
    FDirSprite* pSpr = (FDirSprite*)(m_mapSpr["normal"]);
    setPosition(m_path[m_curPathIndex].x, m_path[m_curPathIndex].y);
    pSpr->chgDir(m_path[m_curPathIndex + 1].x, m_path[m_curPathIndex + 1].y);
}

void BaseObject::setPosition(float x, float y)
{
    m_curPosition.x = x;
    m_curPosition.y = y;
    
    BaseSprite* pSpr = m_mapSpr["normal"];
    
    const Mask* pMask = m_world.getMask("alpha");
    if (pMask != NULL) {
        int yy = 640 - y;
        
        if (pMask->getData(x / 8, yy / 8) == 0) {
            pSpr->setOpacity(255);
        }
        else {
            pSpr->setOpacity(128);
        }
//        int xx = x / 10;
//        int yy = y / 10;
//        //xx = pImg->getWidth() - xx;
//        yy = pImg->getHeight() - yy;
//        unsigned char* pData = pImg->getData();
//        int bpp = pImg->getBitPerPixel() / 8;
//        pData += bpp * pImg->getWidth() * (int)yy;
//        //CCLOG("alpha %d, %d, %d", xx, yy, pData[(pImg->getBitPerPixel() / 8) * (int)xx + 3]);
//        if (pData[bpp * (int)xx + 3] != 0) {
//            pSpr->setOpacity(128);
//        }
//        else {
//            pSpr->setOpacity(255);
//        }
    }
    
    pSpr->setPosition(x, y);
    pSpr->getParent()->reorderChild(pSpr, 640 - y);
    
//    for (std::map<std::string, BaseSprite*>::iterator it = m_mapSpr.begin(); it != m_mapSpr.end(); ++it) {
//        it->second->setPosition(x, y);
//    }
}

void BaseObject::randPath()
{
    m_curPathIndex = rand() % m_path.size();
}

bool BaseObject::procPath(float dt)
{
    int speed = m_mapAct[m_curact].speed;
    if (m_curPathIndex == m_path.size() - 1) {
        setPosition(m_path[m_curPathIndex].x, m_path[m_curPathIndex].y);
        
        return true;
    }
    else {
        float dis = m_path[m_curPathIndex].getDistance(m_path[m_curPathIndex + 1]);
        
        m_curPathDistance += speed * dt;
        
        if (m_curPathDistance >= dis) {
            m_curPathIndex++;
            setPosition(m_path[m_curPathIndex].x, m_path[m_curPathIndex].y);
            m_curPathDistance = 0;
            
            if (m_curPathIndex < m_path.size() - 1) {
                FDirSprite* pSpr = (FDirSprite*)(m_mapSpr["normal"]);
                pSpr->chgDir(m_path[m_curPathIndex + 1].x, m_path[m_curPathIndex + 1].y);
            }
        }
        else {
            cocos2d::Vec2 off = m_path[m_curPathIndex + 1] - m_path[m_curPathIndex];
            off *= m_curPathDistance / dis;
            off += m_path[m_curPathIndex];
            
            setPosition(off.x, off.y);
        }
    }
    
    return false;
}

void BaseObject::addActQueue(ActQueueNode& node)
{
    m_queueAct.push_back(node);
}

void BaseObject::init(rapidjson::Value& val)
{
    load(val["filename"].GetString());
    
    int x = 0;
    int y = 0;
    
    if (val.HasMember("scalex")) {
        m_scalex = val["scalex"].GetInt();
    }
    else {
        m_scalex = 0;
    }
    
    if (val.HasMember("path")) {
        loadRoadPath(val["path"].GetString(), m_path);
    }
    
    if (val.HasMember("pos")) {
        if (strcmp(val["pos"].GetString(), "pathbegin") == 0 && !m_path.empty()) {
            x = m_path[0].x;
            y = m_path[0].y;
        }
    }
    else {
        if (val.HasMember("x")) {
            x = val["x"].GetInt();
        }
        
        if (val.HasMember("y")) {
            y = val["y"].GetInt();
        }
    }
    
    if (val.HasMember("beginact")) {
        chgAct(val["beginact"].GetString());
        
        setPosition(x, y);
    }
    else if (val.HasMember("actlist")) {
        rapidjson::Value& arr = val["actlist"];
        for (int i = 0; i < arr.Size(); ++i) {
            rapidjson::Value& cur = arr[i];
            if (!cur.IsObject()) {
                CCLOG("BaseObject::init() act[%d] is not object", i);
                
                return ;
            }
            
            ActQueueNode node;
            
            node.name = cur["name"].GetString();
            node.act = cur["act"].GetString();
            node.time = 0;
            
            if (cur.HasMember("time")) {
                node.time = cur["time"].GetDouble();
            }
            
            if (cur.HasMember("path")) {
                node.path = cur["path"].GetString();
            }
            
            m_queueAct.push_back(node);
        }
        
        chgAct(m_queueAct[rand() % m_queueAct.size()].act.c_str());
        
        setPosition(x, y);
    }
    
    if (!m_path.empty()) {
        m_curPathIndex = 0;
        m_curPathDistance = 0;
        
        BaseSprite* pSpr = m_mapSpr["normal"];
        setPosition(m_path[m_curPathIndex].x, m_path[m_curPathIndex].y);
        pSpr->chgDir(m_path[m_curPathIndex + 1].x, m_path[m_curPathIndex + 1].y);
    }
}

int BaseObject::getActQueueNode(const char* aqnn)
{
    for (int i = 0; i < m_queueAct.size(); ++i) {
        if (m_queueAct[i].name == aqnn) {
            return i;
        }
    }
    
    return -1;
}
