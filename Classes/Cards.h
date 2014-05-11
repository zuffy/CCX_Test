//
//  Cards.h
//  CCX_Test
//
//  Created by zuffy on 14-4-18.
//
//

#ifndef __CCX_Test__Cards__
#define __CCX_Test__Cards__

#include "cocos2d.h"

USING_NS_CC;

class Cards;
typedef struct MERGEFROM {
    Cards *f;
    Cards *m;
} Mergefrom;

class Cards : public cocos2d::Sprite{
public:
    static Cards* CreateSprite(int num, Point pos);
    int unitSize;
    virtual bool init();
    CREATE_FUNC(Cards);
    /*
    void moveLeft(int num, const Point pt);
    void moveRight(int num, const Point pt);
    void moveUp(int num, const Point pt);
    void moveDown(int num, const Point pt);
    */
    Mergefrom mg;
    Point pos;
    Point prevPos;
    void setNumber(int num);
    int getNumber();
    void mergeRemove(float aniDelay);
    float updatePosition(Point pt, bool removed = false);
    void animationStop(Ref *sender);
    void genAnimated(Ref *sender);
    void getToOriginalStat();
    int getX();
    int getY();
    
    bool isAminating = false;
    bool isMerging;
private:
    int number;
    void enemyInit(int num, int x, int y);
    // void enemyInit(int num, int w, int h, float x, float y);
    cocos2d::Label *cardNum;
    cocos2d::LayerColor *cardBg;
};



#endif /* defined(__CCX_Test__Cards__) */
