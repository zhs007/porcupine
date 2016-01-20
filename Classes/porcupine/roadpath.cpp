#include "basedef.h"

void loadRoadPath(const char* filename, RoadPath& path)
{
    cocos2d::Data filedata = cocos2d::FileUtils::getInstance()->getDataFromFile(filename);
    
    if(!filedata.isNull())
    {
        int pnums;
        
        memcpy(&pnums, filedata.getBytes(), sizeof(int));
        
        for(int j = 0; j < pnums; ++j)
        {
            int x;
            int y;
            
            int sp = sizeof(int) + sizeof(int) * j * 2;
            
            memcpy(&x, filedata.getBytes() + sp, sizeof(int));
            memcpy(&y, filedata.getBytes() + sp + sizeof(int), sizeof(int));
            
            cocos2d::Vec2 pnode;
            
            pnode.x = x;
            pnode.y = y;
            
            path.push_back(pnode);
        }
    }
}