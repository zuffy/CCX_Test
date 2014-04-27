#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Cards.h"
USING_NS_CC;

typedef struct POSTION {
    Point fathest;
    Point next;
} Position;


class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
public:
    
private:
    void autoCreateCardNumber();
    void doCheckGameOver();
    void initEvents();
    void initData();
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    void createCardSprite(cocos2d::Size size);
    bool doLeft();
    bool doLeft2();
    bool doRight();
    bool doRight2();
    bool doUp();
    bool doUp2();
    bool doDown();
    bool doDown2();
    
    // grid
    bool withinBounds(Point cell);
    bool cellAvailable(Point cell);
    Position checkfathest(Point cell, Point dir);
    bool isCellOcuppiedAt(int x, int y);
    Cards* getTile(int x, int y);
    bool moveTile(Cards *tile, Point pos);
    void addCardAt(int x, int y, int value);
    
private:
    Cards* cardArr[4][4];
    int firstX,firstY,endX,endY;
    Sprite *btn1;
    Sprite *btn2;
    Sprite *btn3;
    Sprite *btn4;
    int score;
    Label *scoreLabel;
    
    int map[4][4];
};

#endif // __HELLOWORLD_SCENE_H__
