#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/city/cityperson.plist", "res/city/cityperson.pvr.ccz");
    
    m_pLayer = new WorldLayer();
    m_pLayer->load("res/city/5055/city.json");
    m_pLayer->setRoot(this);
    
    schedule(schedule_selector(HelloWorld::onIdle));
    
    //auto rootNode = CSLoader::createNode("MainScene.csb");

    //addChild(rootNode);

    return true;
}

void HelloWorld::onIdle(float dt)
{
    m_pLayer->onIdle(dt);
}