#include "scheduleex.hpp"

ScheduleEx* ScheduleEx::getSingleton()
{
    static ScheduleEx s_singleton;
    
    return &s_singleton;
}

ScheduleEx::ScheduleEx()
{
    m_timePause = 0;
}

ScheduleEx::~ScheduleEx()
{
    
}

void ScheduleEx::remove(cocos2d::SEL_SCHEDULE callback, cocos2d::Ref* target)
{
    for (__List::iterator it = m_lst.begin(); it != m_lst.end(); ++it) {
        if (it->callback == callback && it->target == target) {
            it->target->release();
            
            m_lst.erase(it);
            
            return ;
        }
    }
}

bool ScheduleEx::has(cocos2d::SEL_SCHEDULE callback, cocos2d::Ref* target)
{
    for (__List::iterator it = m_lst.begin(); it != m_lst.end(); ++it) {
        if (it->callback == callback && it->target == target) {
            return true;
        }
    }
    
    return false;
}

void ScheduleEx::add(cocos2d::SEL_SCHEDULE callback, cocos2d::Ref* target)
{
    ScheduleExNode n;
    
    n.callback = callback;
    n.target = target;
    
    m_lst.push_back(n);
    
    target->retain();
}

void ScheduleEx::onResume()
{
    m_timePause = getCurrentTime();
}

void ScheduleEx::onPause()
{
    int64_t ct = getCurrentTime();
    if (m_timePause > 0 && ct > m_timePause) {
        float ot = (ct - m_timePause) / 1000.0f;
        
        for (__List::iterator it = m_lst.begin(); it != m_lst.end(); ++it) {
            (it->target->*(it->callback))(ot);
        }
        
        m_timePause = 0;
    }
}

int64_t ScheduleEx::getCurrentTime()
{
    timeval tv;
    gettimeofday(&tv, NULL);
    int64_t time = ((int64_t)tv.tv_sec) * 1000;
    time += tv.tv_usec / 1000;
    return time;
}
