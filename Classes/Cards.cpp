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
    unitSize = (Director::getInstance()->getVisibleSize().width-28)/4;
    return true;
}

int Cards::getX(){
    return pos.x;
}

int Cards::getY(){
    return pos.y;
}

void Cards::updatePosition(Point pt, bool removed){
    this->pos = Point(pt.x, pt.y);
    this->isAminating = removed;
    int dx = this->unitSize * pos.x + 20, dy = unitSize * pos.y + 50;
    Sequence *sq = Sequence::create(
                                    MoveTo::create(.1, Point(dx,dy)),
                                    CallFuncN::create(CC_CALLBACK_1(Cards::animationStop, this)),
                                    nullptr);
    
    //cardBg->setPosition(Point(dx, dy));
    this->runAction(sq);
    
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
    number = num;
    if (number >= 0) {
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
    }
    
    if(number == 0){
        cardBg->setColor(cocos2d::Color3B(200,190,180));
    }
    if (number == 2) {
        cardBg->setColor(cocos2d::Color3B(240,230,220));
    }
    if (number == 4) {
        cardBg->setColor(cocos2d::Color3B(240,220,200));
    }
    if (number == 8) {
        cardBg->setColor(cocos2d::Color3B(240,180,120));
    }
    if (number == 16) {
        cardBg->setColor(cocos2d::Color3B(240,140,90));
    }
    if (number == 32) {
        cardBg->setColor(cocos2d::Color3B(240,120,90));
    }
    if (number == 64) {
        cardBg->setColor(cocos2d::Color3B(240,90,60));
    }
    if (number == 128) {
        cardBg->setColor(cocos2d::Color3B(240,90,60));
    }
    if (number == 256) {
        cardBg->setColor(cocos2d::Color3B(240,200,70));
    }
    if (number == 512) {
        cardBg->setColor(cocos2d::Color3B(240,200,70));
    }
    if (number == 1024) {
        cardBg->setColor(cocos2d::Color3B(0,130,0));
    }
    if (number == 2048) {
        cardBg->setColor(cocos2d::Color3B(0,130,0));
    }
    
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
        cardNum = Label::create(__String::createWithFormat("%i", num)->getCString(),"HirakakuProN-w6",100);
    }
    else{
        cardNum = Label::create("","HirakakuProN-w6",100);
    }
    cardNum->setPosition(Point(cardBg->getContentSize().width * .5, cardBg->getContentSize().height *.5));
    cardBg->addChild(cardNum);
    this->addChild(cardBg);
    setNumber(num);
    this->setScale(.5, .5);
    float _w = unitSize;
    float _h = unitSize;
    this->setPosition(Point(x + _w *.25, y + _h*.25));
    this->runAction(Sequence::create(
                        Spawn::create(
                            ScaleTo::create(.08, 1.2f, 1.2f),
                            MoveTo::create(.08, Point(x - _w *.1, y - _w *.1)), NULL),
                        Spawn::create(
                            ScaleTo::create(.04, 1.0f, 1.0f),
                                      MoveTo::create(.04, Point(x,y)), NULL),
                                     CallFuncN::create(CC_CALLBACK_1(Cards::genAnimated,this)),NULL));
}
/*
float move_time = 0.24;
void Cards::moveLeft(int num, const Point pt){
    number = num;
    this->runAction(Sequence::create(MoveTo::create(move_time, pt), CallFuncN::create([=](Node* pSender){
        this->setNumber(num);
    }), NULL));
}
void Cards::moveRight(int num, const Point pt){

}
void Cards::moveUp(int num, const Point pt){

}
void Cards::moveDown(int num, const Point pt){

}*/





