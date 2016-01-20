#include "anisprite.hpp"

AniSprite::AniSprite()
{
    
}

AniSprite::~AniSprite()
{
    
}

AniSprite* AniSprite::create(std::vector<cocos2d::SpriteFrame*>& lst, float frameTime, bool isloop, float time)
{
    AniSprite* pRet = new(std::nothrow) AniSprite();
    if (pRet && pRet->init(lst, frameTime, isloop, time)) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool AniSprite::init(std::vector<cocos2d::SpriteFrame*>& lst, float frameTime, bool isloop, float time)
{
    for (int i = 0; i < lst.size(); ++i) {
        m_lstFrames.push_back(lst[i]);
    }
    
    m_frameTime = frameTime;
    m_lastTime = m_frameTime;
    m_curIndex = 0;
    
    m_isloop = isloop;
    m_anitime = time;
    
    return this->initWithSpriteFrame(m_lstFrames[m_curIndex]);
}

void AniSprite::release()
{
}

bool AniSprite::onIdle(float dt)
{
    if (m_anitime > 0 && m_anitime - dt <= 0) {
        return true;
    }
    
    m_anitime -= dt;
    
    bool ischg = false;
    m_lastTime -= dt;
    
    while (m_lastTime <= 0) {
        m_curIndex++;
        if (m_curIndex >= m_lstFrames.size()) {
            if (!m_isloop) {
                return true;
            }
            
            m_curIndex = 0;
        }
        
        m_lastTime += m_frameTime;
        
        ischg = true;
    }
    
    if (ischg) {
        setSpriteFrame(m_lstFrames[m_curIndex]);
    }
    
    return false;
}