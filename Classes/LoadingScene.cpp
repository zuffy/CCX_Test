//
//  LoadingScene.cpp
//  CCX_Test
//
//  Created by zuffy on 14-5-11.
//
//

#include "LoadingScene.h"
#include "GameWorld.h"

bool LoadingScene::audioloaded = false;

LoadingScene::~LoadingScene()
{
}

Scene* LoadingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoadingScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool LoadingScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    LoadingResource();
    return true;
}

void LoadingScene::LoadingResource()
{
    /*
    if(!particleloaded)
        LoadingParticle();
    //Loading Music
    if(!audioloaded)
        LoadingMusic();
    */
    //Loading Picture
    LoadingPic();
}

void LoadingScene::LoadingPic()
{
    auto TexureCache=Director::getInstance()->getTextureCache();
    TexureCache->addImageAsync("HelloWorld.png",CC_CALLBACK_1(LoadingScene::LoadingCallback,this));
    
}

void LoadingScene::LoadingCallback(Ref* pObj)
{
    ++currentNum;
    if (currentNum == totalNum) {
        GotonextScene();
    }
}

void LoadingScene::GotonextScene()
{
    audioloaded = true;
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game_scene.plist","game_scene.png");
    this->removeAllChildren();
    auto gameScene=GameWorld::createScene();
    Director::getInstance()->replaceScene(TransitionZoomFlipX::create(1.0f,gameScene));
}
