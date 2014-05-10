//
//  Cards.cpp
//  CCX_Test
//
//  Created by zuffy on 14-4-18.
//
//

#include "Cards.h"

Cards* Cards::CreateSprite(int num, Point pos){
    Cards* enemyCard = new Cards();
    if (enemyCard && enemyCard->init()) {
        enemyCard->autorelease();
        enemyCard->pos = pos;
        enemyCard->prevPos = Point(-1, -1);
        enemyCard->enemyInit(num, pos.x, pos.y);
        return enemyCard;
    }
    CC_SAFE_DELETE(enemyCard);
    return NULL;
}

bool Cards::init(){
    if (!Sprite::init()) {
        return false;
    }
    unitSize = (Director::getInstance()->getVisibleSize().width-50)/4;
    return true;
}

int Cards::getX(){
    return pos.x;
}

int Cards::getY(){
    return pos.y;
}

void Cards::mergeRemove(float aniDelay){
    this->isAminating = true;
    Sequence *sq = Sequence::create(
                                    DelayTime::create(aniDelay),
                                    CallFuncN::create(CC_CALLBACK_1(Cards::animationStop, this)),
                                    nullptr);
    this->runAction(sq);
}
void Cards::getToOriginalStat(){
    this->stopAllActions();
    this->setScale(1.0, 1.0);
}

int getDel(int num){
    int ret = 0;
    do{
        ret++;
        num >>= 1;
    }while (num > 1 && ret < 100);
    return ret;
}

float Cards::updatePosition(Point pt, bool removed){
    this->pos = Point(pt.x, pt.y);
    this->isAminating = removed;
    getToOriginalStat();
    int de = MIN(getDel(number), 4);
    float time = CCRANDOM_0_1()* de/ 5.0 +.1;
    int dx = this->unitSize * pos.x + 20, dy = unitSize * pos.y + 50;
    Sequence *sq = Sequence::create(
                                    //MoveTo::create(time, Point(dx,dy)),
                                    JumpTo::create(time,Point(dx,dy),30.0f,2),
                                    CallFuncN::create(CC_CALLBACK_1(Cards::animationStop, this)),
                                    nullptr);
    
    //cardBg->setPosition(Point(dx, dy));
    this->runAction(sq);
    return time;
}
void Cards::genAnimated(Object *sender){
    this->isMerging = false;
}
void Cards::animationStop(Object *sender){
    if (this->isAminating) {
        this->isAminating = false;
        this->stopAllActions();
        this->removeFromParent();
    }
}

void Cards::setNumber(int num){
    Sprite *tmp = (Sprite*)cardBg->getChildByTag(1);
    if (nullptr != tmp) {
        cardBg->removeChildByTag(1);
    }
    
    number = num;
    /*if (number >= 0) {
        cardNum->setFontSize(100);
    }
    if (number >= 16) {
        cardNum->setFontSize(90);
    }
    if(number >= 128){
        cardNum->setFontSize(60);
    }
    if(number >= 1024){
        cardNum->setFontSize(40);
    }*/
    tmp = Sprite::create("pic1.png");
    if(number == 0){
        cardBg->setColor(cocos2d::Color3B(200,190,180));
    }
    if (number == 2) {
        cardBg->setColor(cocos2d::Color3B(240,230,220));
        tmp = Sprite::create("pic1.png");
    }
    if (number == 4) {
        cardBg->setColor(cocos2d::Color3B(240,220,200));
        tmp = Sprite::create("pic2.png");
    }
    if (number == 8) {
        cardBg->setColor(cocos2d::Color3B(240,180,120));
        tmp = Sprite::create("pic3.png");
    }
    if (number == 16) {
        cardBg->setColor(cocos2d::Color3B(240,140,90));
        tmp = Sprite::create("pic4.png");
    }
    if (number == 32) {
        cardBg->setColor(cocos2d::Color3B(240,120,90));
        tmp = Sprite::create("pic5.png");
    }
    if (number == 64) {
        cardBg->setColor(cocos2d::Color3B(240,90,60));
        tmp = Sprite::create("pic6.png");
    }
    if (number == 128) {
        cardBg->setColor(cocos2d::Color3B(240,90,60));
        tmp = Sprite::create("pic7.png");
    }
    if (number == 256) {
        cardBg->setColor(cocos2d::Color3B(240,200,70));
        tmp = Sprite::create("pic8.png");
    }
    if (number == 512) {
        cardBg->setColor(cocos2d::Color3B(240,200,70));
        tmp = Sprite::create("pic9.png");
    }
    if (number == 1024) {
        cardBg->setColor(cocos2d::Color3B(0,130,0));
        tmp = Sprite::create("pic10.png");
    }
    if (number == 2048) {
        cardBg->setColor(cocos2d::Color3B(0,130,0));
        tmp = Sprite::create("pic11.png");
    }
    tmp->setPosition(Point(cardBg->getContentSize().width * .5, cardBg->getContentSize().height *.5));
    cardBg->addChild(tmp, 0, 1);
    
    if (num > 0) {
        cardNum->setString(__String::createWithFormat("%i",num)->getCString());
        cardBg->setVisible(true);
    }
    else{
        cardNum->setString("");
        cardBg->setVisible(false);
    }
}
int Cards::getNumber(){
    return number;
}
void Cards::enemyInit(int num, int _x, int _y){
    int w = unitSize, h = unitSize, x =  unitSize * _x + 20, y = unitSize * _y + 50;
    cardBg = LayerColor::create(Color4B(200,190,180,255), w-15, h-15);
    
    //this->setAnchorPoint(Point(1,.5));
    if(num > 0){
        cardNum = Label::create(__String::createWithFormat("%i", num)->getCString(),"HirakakuProN-w6",20);
    }
    else{
        cardNum = Label::create("","HirakakuProN-w6",10);
    }
    // cardNum->setPosition(Point(cardBg->getContentSize().width * .5, cardBg->getContentSize().height *.5));
    cardNum->setPosition(Point(15, 15));
    cardBg->addChild(cardNum,1);
    this->addChild(cardBg);
    setNumber(num);
    this->setScale(.8, .8);
    float _w = unitSize;
    float _h = unitSize;
    this->setPosition(Point(-100, -100));
    this->runAction(Sequence::create(
                        DelayTime::create(.1),
                        Place::create(Point(x + _w *.1, y + _h*.1)),
                        Spawn::create(
                            ScaleTo::create(.1, 1.1f, 1.1f),
                            MoveTo::create(.1, Point(x - _w *.05, y - _w *.05)), NULL),
                        Spawn::create(
                            ScaleTo::create(.08, 1.0f, 1.0f),
                                      MoveTo::create(.08, Point(x,y)), NULL),
                                     CallFuncN::create(CC_CALLBACK_1(Cards::genAnimated,this)),NULL));
}




