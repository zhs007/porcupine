#ifndef __PWORLD_SCHEDULEEX_HPP__
#define __PWORLD_SCHEDULEEX_HPP__

#include "cocos2d.h"
#include "basedef.h"

struct ScheduleExNode{
    cocos2d::SEL_SCHEDULE callback;
    cocos2d::Ref* target;
};

class ScheduleEx{
    typedef std::list<ScheduleExNode> __List;
public:
    static ScheduleEx* getSingleton();
protected:
    ScheduleEx();
    virtual ~ScheduleEx();
public:
    void add(cocos2d::SEL_SCHEDULE callback, cocos2d::Ref* target);
    
    void remove(cocos2d::SEL_SCHEDULE callback, cocos2d::Ref* target);
    
    bool has(cocos2d::SEL_SCHEDULE callback, cocos2d::Ref* target);
    
    void onResume();
    
    void onPause();
    
    int64_t getCurrentTime();
protected:
    __List m_lst;
    int64_t m_timePause;
};

#endif //__PWORLD_SCHEDULEEX_HPP__
