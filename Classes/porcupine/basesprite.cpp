//
//  basesprite.cpp
//  pixelworld
//
//  Created by 赵洪松 on 16/1/7.
//
//

#include "basesprite.hpp"

BaseSprite* BaseSprite::create()
{
    BaseSprite* pRet = new(std::nothrow) BaseSprite();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool BaseSprite::init()
{
    this->setVisible(false);
    
    return true;
}