//
//  LoadingScene.h
//  CCX_Test
//
//  Created by zuffy on 14-5-11.
//
//

#ifndef __CCX_Test__LoadingScene__
#define __CCX_Test__LoadingScene__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

#define TOTAL_PIC_NUM 1

class LoadingScene : public Layer
{
public:
    LoadingScene():currentNum(0),totalNum(TOTAL_PIC_NUM){};
    ~LoadingScene();
    
    static Scene* createScene();
    CREATE_FUNC(LoadingScene);
    virtual bool init();
    
    static bool audioloaded;
private:
    
    void LoadingResource();
    void LoadingPic();
    void LoadingCallback(Ref* pObj);
    void GotonextScene();
    int currentNum;
    int totalNum;
    
};

#endif /* defined(__CCX_Test__LoadingScene__) */
