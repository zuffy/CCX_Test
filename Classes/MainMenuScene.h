//
//  MainMenuScene.h
//  CCX_Test
//
//  Created by zuffy on 14-5-11.
//
//

#ifndef __CCX_Test__MainMenuScene__
#define __CCX_Test__MainMenuScene__

#include "cocos2d.h"

USING_NS_CC;

class MainMenuScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MainMenuScene);
    
private:
    MenuItemSprite* endlessmode_item;
    MenuItemSprite* challengemode_item;
    void startEndlessGame(Ref* pSender);
};

#endif /* defined(__CCX_Test__MainMenuScene__) */
