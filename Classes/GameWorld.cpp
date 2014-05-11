//
//  GameWord.cpp
//  CCX_Test
//
//  Created by zuffy on 14-5-11.
//
//

#include "GameWorld.h"
#include "GameLayer.h"
USING_NS_CC;

GameWorld::~GameWorld()
{
    
}

Scene* GameWorld::createScene()
{
    auto scene = Scene::create();
    
    auto layer = GameWorld::create();
    layer->setTag(100);
    scene->addChild(layer, 2);
    
    // add warning layer
    auto warningLayer = LayerColor::create(Color4B(255, 0, 0, 60));
    warningLayer->setOpacity(0);
    warningLayer->setTag(456);
    scene->addChild(warningLayer,7);
    
    return scene;
}

bool GameWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    auto sb = GameLayer::create();
    sb->setTag(123);
    //sb->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    addChild(sb);
    
    return true;
}

void GameWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}