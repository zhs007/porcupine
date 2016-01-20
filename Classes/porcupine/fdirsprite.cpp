#include "fdirsprite.hpp"

FDirSprite::FDirSprite()
    : m_curDir(-1)
{
    
}

FDirSprite::~FDirSprite()
{
    
}

FDirSprite* FDirSprite::create(std::vector<cocos2d::SpriteFrame*>& lst, float frameTime)
{
    FDirSprite* pRet = new(std::nothrow) FDirSprite();
    if (pRet && pRet->init(lst, frameTime)) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool FDirSprite::init(std::vector<cocos2d::SpriteFrame*>& lst, float frameTime)
{
    for (int i = 0; i < lst.size(); ++i) {
        m_lstFrames.push_back(lst[i]);
    }
    
    m_frameTime = frameTime;
    m_lastTime = m_frameTime;
    m_curIndex = 0;
    
    m_frameNums = m_lstFrames.size() / 2;
    
    return this->initWithSpriteFrame(m_lstFrames[m_curIndex]);
}

void FDirSprite::release()
{
}

bool FDirSprite::onIdle(float dt)
{
    bool ischg = false;
    m_lastTime -= dt;
    
    while (m_lastTime <= 0) {
        m_curIndex++;
        if (m_curIndex >= m_frameNums) {
            m_curIndex = 0;
        }
        
        m_lastTime += m_frameTime;
        
        ischg = true;
    }
    
    if (ischg) {
        bool up = (m_curDir == FDIR_RIGHT_UP || m_curDir == FDIR_LEFT_UP);
        
        //CCLOG("%d", up ? (m_frameNums + m_curIndex) : m_curIndex);
        
        setSpriteFrame(m_lstFrames[up ? (m_frameNums + m_curIndex) : m_curIndex]);
    }
    
    return false;
}

void FDirSprite::chgDir(float dx, float dy)
{
    const cocos2d::Vec2& pos = getPosition();
    bool left = (m_curDir == FDIR_LEFT_UP || m_curDir == FDIR_LEFT_DOWN);
    bool up = (m_curDir == FDIR_RIGHT_UP || m_curDir == FDIR_LEFT_UP);
    
    if (pos.x > dx) {
        left = true;
    }
    else if (pos.x < dx) {
        left = false;
    }
    
    if (pos.y > dy) {
        up = false;
    }
    else if (pos.y < dy) {
        up = true;
    }
    
    int dir = left ? (up ? FDIR_LEFT_UP : FDIR_LEFT_DOWN) : (up ? FDIR_RIGHT_UP : FDIR_RIGHT_DOWN);
    
    if (dir != m_curDir) {
        m_curDir = dir;
        
        setScaleX(left ? -1 : 1);
        setSpriteFrame(m_lstFrames[up ? (m_frameNums + m_curIndex) : m_curIndex]);
    }
}
