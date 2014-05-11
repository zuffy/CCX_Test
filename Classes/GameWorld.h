//
//  GameWord.h
//  CCX_Test
//
//  Created by zuffy on 14-5-11.
//
//

#ifndef __CCX_Test__GameWord__
#define __CCX_Test__GameWord__


#include "cocos2d.h"

class GameWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(GameWorld);
    
    ~GameWorld();
    
};

#endif /* defined(__CCX_Test__GameWord__) */
