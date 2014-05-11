//
//  MainMenuScene.cpp
//  CCX_Test
//
//  Created by zuffy on 14-5-11.
//
//

#include "MainMenuScene.h"
#include "GameWorld.h"
#include "LoadingScene.h"
USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = MainMenuScene::create();
    
    scene->addChild(layer);
    
    return scene;
}


bool MainMenuScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("menu_scene.plist", "menu_scene.png");
    
    // CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Star_Chaser.mp3");
    
    auto background = Sprite::createWithSpriteFrameName("menu_scene_bg.jpg");
    background->setAnchorPoint(Point(0,0));
    this->addChild(background, -1, -1);
    
    auto btn_normal = Sprite::createWithSpriteFrameName("menu_scene_btn.png");
    auto btn_press = Sprite::createWithSpriteFrameName("menu_scene_btn_down.png");

    endlessmode_item = MenuItemSprite::create(btn_normal, btn_press, CC_CALLBACK_1
        (MainMenuScene::startEndlessGame, this));
    endlessmode_item->setPosition(visibleSize.width/2,visibleSize.height/7*4);
    
    btn_normal = Sprite::createWithSpriteFrameName("menu_scene_btn.png");
    btn_press = Sprite::createWithSpriteFrameName("menu_scene_btn_down.png");
    challengemode_item = MenuItemSprite::create(btn_normal, btn_press, CC_CALLBACK_1
                                                (MainMenuScene::startEndlessGame, this));
    challengemode_item->setPosition(visibleSize.width/2, visibleSize.height/7*4-50);
    
    auto menu = Menu::create(endlessmode_item, challengemode_item, NULL);
    menu->setPosition(Director::getInstance()->getVisibleOrigin());
    addChild(menu,3);
    
    return true;
}

void MainMenuScene::startEndlessGame(Ref* pSender)
{
    //CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    auto scene = (LoadingScene::audioloaded) ? GameWorld::createScene() : LoadingScene::createScene();
    Director::getInstance()->replaceScene(scene);
}