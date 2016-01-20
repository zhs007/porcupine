#ifndef __PWORLD_MASK_HPP__
#define __PWORLD_MASK_HPP__

#include "cocos2d.h"
#include "basedef.h"

class Mask{
public:
    Mask();
    virtual ~Mask();
public:
    void load(const char* filename);
    
    void release();
    
    int getData(int x, int y) const;
protected:
    int m_width;
    int m_height;
    
    int* m_pData;
};

#endif //__PWORLD_MASK_HPP__
