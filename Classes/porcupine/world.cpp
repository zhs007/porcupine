#include "world.hpp"
#include "json/document.h"

//// on "init" you need to initialize your instance
//bool WorldLayer::init()
//{
//    if (!Layer::init())
//    {
//        return false;
//    }
//    
//    auto spr = cocos2d::Sprite::create("res/ly.png");
//    spr->setAnchorPoint(cocos2d::Vec2::ZERO);
//    addChild(spr);
//    
//    auto sz = spr->getContentSize();
//    setContentSize(sz);
//    
//    setAnchorPoint(cocos2d::Vec2::ZERO);
//    
////    Size visibleSize = Director::getInstance()->getVisibleSize();
////    Vec2 origin = Director::getInstance()->getVisibleOrigin();
////    
////    initMap();
////    
////    setTouchEnabled(true);
////    
////    m_bMouseDown = false;
////    
////    auto mouseListener = EventListenerMouse::create();
////    mouseListener->onMouseScroll = CC_CALLBACK_1(WorldMapLayer::onMouseScroll, this);
////    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
////    
////    auto touchListener = EventListenerTouchOneByOne::create();
////    touchListener->onTouchBegan = CC_CALLBACK_2(WorldMapLayer::_onTouchBegan, this);
////    touchListener->onTouchMoved = CC_CALLBACK_2(WorldMapLayer::_onTouchMoved, this);
////    touchListener->onTouchEnded = CC_CALLBACK_2(WorldMapLayer::_onTouchEnded, this);
////    touchListener->onTouchCancelled = CC_CALLBACK_2(WorldMapLayer::_onTouchEnded, this);
////    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
////    
////    auto touchesListener = EventListenerTouchAllAtOnce::create();
////    touchesListener->onTouchesBegan = CC_CALLBACK_2(WorldMapLayer::_onTouchesBegan, this);
////    touchesListener->onTouchesMoved = CC_CALLBACK_2(WorldMapLayer::_onTouchesMoved, this);
////    touchesListener->onTouchesEnded = CC_CALLBACK_2(WorldMapLayer::_onTouchesEnded, this);
////    touchesListener->onTouchesCancelled = CC_CALLBACK_2(WorldMapLayer::_onTouchesEnded, this);
////    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchesListener, this);
////    
////    schedule(schedule_selector(WorldMapLayer::onIdle));
//    
//    return true;
//}
//
//void WorldLayer::addObj(BaseObject* pObj)
//{
//    m_lstObj.push_back(pObj);
//    
//    addChild(pObj->m_mapSpr["normal"]);
//}

WorldLayer::WorldLayer()
{
    
}

WorldLayer::~WorldLayer()
{
    
}

void WorldLayer::load(const char* filename)
{
    rapidjson::Document readdoc;
//    ssize_t sz = 0;
//    unsigned char* fd = cocos2d::FileUtils::getInstance()->getFileData(filename, "r", &sz);
//    char* rfd = (char*)malloc(sz + 1);
//    memcpy(rfd, fd, sz);
//    rfd[sz] = '\0';
//    readdoc.Parse(rfd);
//    if (readdoc.HasParseError()) {
//        CCLOG("WorldLayer::load(%s) err %d", filename, readdoc.GetParseError());
//        
//        release();
//        
//        return ;
//    }
    
    loadJsonFile(filename, readdoc);
    
    if(!readdoc.IsObject()) {
        CCLOG("WorldLayer::load(%s) file format err", filename);
        
        release();
        
        return ;
    }
    
    m_name = readdoc["name"].GetString();
    
    if (readdoc.HasMember("layer")) {
        rapidjson::Value& arr = readdoc["layer"];
        if (!arr.IsArray()) {
            CCLOG("WorldLayer::load(%s) layer is not array", filename);
            
            release();
            
            return ;
        }
        
        for (int i = 0; i < arr.Size(); ++i) {
            rapidjson::Value& cur = arr[i];
            if (!cur.IsObject()) {
                CCLOG("WorldLayer::load(%s) layer[%d] is not object", filename, i);
                
                release();
                
                return ;
            }
            
            if (cur.HasMember("filename")) {
                createLayer(cur["name"].GetString(), cur["type"].GetString(), cur["filename"].GetString());
            }
            else {
                createLayer(cur["name"].GetString(), cur["type"].GetString(), "");
            }
        }
    }
    
    if (readdoc.HasMember("mask")) {
        rapidjson::Value& arr = readdoc["mask"];
        if (!arr.IsArray()) {
            CCLOG("WorldLayer::load(%s) mask is not array", filename);
            
            release();
            
            return ;
        }
        
        for (int i = 0; i < arr.Size(); ++i) {
            rapidjson::Value& cur = arr[i];
            if (!cur.IsObject()) {
                CCLOG("WorldLayer::load(%s) mask[%d] is not object", filename, i);
                
                release();
                
                return ;
            }
            
            createMask(cur["name"].GetString(), cur["filename"].GetString());
        }
    }
    
    if (readdoc.HasMember("objlist")) {
        rapidjson::Value& arr = readdoc["objlist"];
        if (!arr.IsArray()) {
            CCLOG("WorldLayer::load(%s) objlist is not array", filename);
            
            release();
            
            return ;
        }
        
        for (int i = 0; i < arr.Size(); ++i) {
            rapidjson::Value& cur = arr[i];
            if (!cur.IsObject()) {
                CCLOG("WorldLayer::load(%s) objlist[%d] is not object", filename, i);
                
                release();
                
                return ;
            }
            
            addObj(cur);
        }
    }
    
    return ;
errend:
    //free(rfd);
    release();
}

void WorldLayer::createLayer(const char* name, const char* type, const char* filename)
{
    cocos2d::Layer* pLayer = cocos2d::Layer::create();
    
    if (strcmp(type, "image") == 0) {
        cocos2d::Sprite* pSpr = cocos2d::Sprite::create(filename);
        pSpr->setAnchorPoint(cocos2d::Vec2::ZERO);
        pLayer->addChild(pSpr);
        
        auto sz = pSpr->getContentSize();
        pLayer->setContentSize(sz);
        pLayer->setAnchorPoint(cocos2d::Vec2::ZERO);
    }
    else {
        pLayer->setAnchorPoint(cocos2d::Vec2::ZERO);
    }
    
    m_mapLayer[name] = pLayer;
}

void WorldLayer::release()
{
    
}

BaseObject* WorldLayer::addObj(rapidjson::Value& val)
{
    BaseObject* pObj = new BaseObject(*this);
    
    pObj->init(val);
    
//    cocos2d::Sprite* pSpr = pObj->m_mapSpr["normal"];
//    m_mapLayer["normal"]->addChild(pSpr);
    
    m_lstObj.push_back(pObj);
    
    return pObj;
}

void WorldLayer::setRoot(cocos2d::Layer* pLayer)
{
    pLayer->addChild(m_mapLayer["back"]);
    pLayer->addChild(m_mapLayer["normal"]);
}

void WorldLayer::onIdle(float ot)
{
    for (int i = 0; i < m_lstObj.size(); ++i) {
        m_lstObj[i]->onIdle(ot);
    }
}

void WorldLayer::createMask(const char* name, const char* filename)
{
    Mask* pMask = new Mask;
    
    pMask->load(filename);
    
    m_mapMask[name] = pMask;
}
