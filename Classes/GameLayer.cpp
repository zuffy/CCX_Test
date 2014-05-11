#include "GameLayer.h"
#include "extensions/cocos-ext.h"
#include "SqliteUtil.h"
#include "ContentValue.h"

USING_NS_CC_EXT;

int columns = 4;
int cells = 4;


int selectCallBack(void * para, int n_column, char ** column_value, char ** column_name )
{
    ContentValue* value = (ContentValue *)para;
    for (int i = 0; i < n_column; i++)
    {
        value->setValue(column_name[i],column_value[i]);
    }
    return 0;
}
int selectAllCallBack(void * para, int n_column, char ** column_value, char ** column_name )
{
    list<ContentValue*>* value = (list<ContentValue*>*)para;
    ContentValue* raw = new ContentValue();
    for (int i = 0; i < n_column; i++)
    {
        raw->setValue(column_name[i],column_value[i]);
    }
    value->push_back(raw);
    return 0;
}


Scene* GameLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    content = new ContentValue();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // Point origin = Director::getInstance()->getVisibleOrigin();
    
    timeval tv;
    gettimeofday(&tv, NULL);
    //都转化为毫秒
    unsigned long reed = tv.tv_sec*1000+tv.tv_usec/1000;
    //srand()中传入一个随机数种子
    srand(reed);
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    auto background = Sprite::createWithSpriteFrameName("game_scene_bg.png");
    background->setAnchorPoint(Point(0,0));
    this->addChild(background, 0, 0);
    
    btn1 = Sprite::create("CloseNormal.png");
    btn2 = Sprite::create("CloseNormal.png");
    btn3 = Sprite::create("CloseNormal.png");
    btn4 = Sprite::create("CloseNormal.png");
    
    /*
     btn1->setPosition(Point(40, visibleSize.height-150));
     btn2->setPosition(Point(110,visibleSize.height-150));
     btn3->setPosition(Point(75,visibleSize.height-115));
     btn4->setPosition(Point(75,visibleSize.height-185));
     
     addChild(btn1);
     addChild(btn2);
     addChild(btn3);
     addChild(btn4);
     */
    
    initData();
    
    createCardSprite(visibleSize);
    
    initEvents();
    
    autoCreateCardNumber();
    autoCreateCardNumber();
    
    recordData();
    //    addCardAt(0,0,2);
    //    addCardAt(0,1,2);
    //    addCardAt(0,2,4);
    //    addCardAt(0,3,2);
    
    //加入“分数”label
    auto labelTTFCardNumberName = LabelTTF::create("SCORE","HiraKakuProN-W6", 32);
    labelTTFCardNumberName->setPosition(Point(visibleSize.width*.25,visibleSize.height-40));
    addChild(labelTTFCardNumberName);
    
    //加入具体的分数
    scoreLabel = Label::create("0","HiraKakuProN-w6",30);
    scoreLabel->setPosition(Point(visibleSize.width*.8, visibleSize.height-50));
    addChild(scoreLabel);
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameLayer::menuCloseCallback, this));
    closeItem->setScale(1.5);
	closeItem->setPosition(Point(20, visibleSize.height-100));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    
    Scale9Sprite *backgroundButton = Scale9Sprite::createWithSpriteFrameName("button.png");
    backgroundButton->setPreferredSize(Size(50,50));
    Scale9Sprite *backgroundHighlightedButton = Scale9Sprite::createWithSpriteFrameName("buttonHighlighted.png");
    backgroundHighlightedButton->setPreferredSize(Size(50,50));
    
    ControlButton* button = ControlButton::create(backgroundButton);
    button->setBackgroundSpriteForState(backgroundHighlightedButton, Control::State::HIGH_LIGHTED);
    button->setTitleForState("R", Control::State::NORMAL);
    button->setTitleForState("hi", Control::State::HIGH_LIGHTED);
    button->setAdjustBackgroundImage(false);
    button->setTag(15);
    button->addTargetWithActionForControlEvents(this,  cccontrol_selector(GameLayer::reverseTiles), Control::EventType::TOUCH_UP_INSIDE );
    
    button->setPosition(100, visibleSize.height-100);
    this->addChild(button);
    
    
    canReverse = false;
    
    return true;
}

void GameLayer::reverseTiles(Ref*ref, Control::EventType e){
    if (!canReverse) {
        return;
    }
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            map[i][j] = last_map[i][j];
        }
    }
    createCardSprite(Size(0,0));
    score = last_score;
    scoreLabel->setString(String::createWithFormat("%i",score)->getCString());
    canReverse = false;
}
//创建卡片
void GameLayer::createCardSprite(cocos2d::Size size)
{
    //求出单元格的宽度和高度
    //4*4的单元格
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            Cards* card = cardArr[i][j];
            if (nullptr != card) {
                card->removeFromParentAndCleanup(true);
            }
            cardArr[i][j] = nullptr;
            if (map[i][j]>0) {
                card = Cards::CreateSprite(map[i][j], Point(i, j));
                cardArr[i][j] = card;
                addChild(card);
            }
        }
    }
}

bool GameLayer::checkTable()
{
    if (SqliteUtil::getInstance()->usable) {
        if(!SqliteUtil::getInstance()->tableIsExist(this->content->SQL_TABLE)){
            SqliteUtil::getInstance()->createTable(this->content->create_table.c_str());
        }
        return true;
    }
    return false;
}

int GameLayer::selectAll(ContentValue * para, int n_column, char ** column_value, char ** column_name)
{
    list<ContentValue*>* value = (list<ContentValue*>*)para;
    ContentValue* raw = new ContentValue();
    for (int i = 0; i < n_column; i++)
    {
        raw->setValue(column_name[i],column_value[i]);
    }
    value->push_back(raw);
    return 0;
}

//创建卡片
void GameLayer::initData()
{
    bool isGetTable = checkTable();
    if (isGetTable) {
        SqliteUtil::getInstance()->insert(this->content->SQL_TABLE, this->content);
        SqliteUtil::getInstance()->select(this->content->SQL_TABLE, this->content, "ID", "1");
    }
    
    CCLOG("mzh score:%d", this->content->score);
    //4*4的单元格
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            map[i][j] = 0;
        }
    }
}

void GameLayer::recordData(){
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            last_map[i][j] = map[i][j];
            if(cardArr[i][j] != nullptr){
                map[i][j] = cardArr[i][j]->getNumber();
            }else{
                map[i][j] = 0;
            }
        }
    }
    canReverse = true;
}

//求出单元格的宽度和高度
void GameLayer::autoCreateCardNumber(){
    int i = CCRANDOM_0_1()*4;
    int j = CCRANDOM_0_1()*4;
    
    //判断是否这个位置已存在卡片
    if(this->isCellOcuppiedAt(i, j))
    {
        autoCreateCardNumber();
    }
    else
    {
        int num = CCRANDOM_0_1()*100 < 9 ? 4: 2;
        Cards* card = Cards::CreateSprite(num, Point(i, j));
        cardArr[i][j] = card;
        addChild(card);
    }
}

void GameLayer::addCardAt(int x, int y, int value){
    Cards* card = Cards::CreateSprite(value, Point(x, y));
    cardArr[x][y] = card;
    addChild(card);
}


void GameLayer::initEvents(){
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

float dirx = 0;
float diry = 0;
bool isTouchReacted = false;
bool btnReact = false;
bool GameLayer::onTouchBegan(Touch *touch, Event *event){
    Point tp = touch->getLocation();
    //    touch->getLocationInView();
    firstX = tp.x;
    firstY = tp.y;
    if (firstY < (this->getChildByTag(15)->getPosition().y - this->getChildByTag(15)->getContentSize().height)) {
        isTouchReacted = true;
    }
    else{
        isTouchReacted = false;
    }
    /*
     Rect rect = btn1->boundingBox();
     if (rect.containsPoint(tp)) {
     if(doLeft2()){
     autoCreateCardNumber();
     recordData();
     }
     doCheckGameOver();
     btnReact = true;
     }
     
     rect = btn2->boundingBox();
     if (rect.containsPoint(tp)) {
     if(doRight2()){
     autoCreateCardNumber();
     recordData();
     }
     doCheckGameOver();
     btnReact = true;
     }
     
     rect = btn3->boundingBox();
     if (rect.containsPoint(tp)) {
     if(doUp2()){
     autoCreateCardNumber();
     recordData();
     }
     doCheckGameOver();
     btnReact = true;
     }
     
     rect = btn4->boundingBox();
     if (rect.containsPoint(tp)) {
     if(doDown2()){
     autoCreateCardNumber();
     recordData();
     }
     doCheckGameOver();
     btnReact = true;
     }*/
    return true;
}

void GameLayer::onTouchEnded(Touch *touch, Event *event){
    Point pt = touch->getLocation();
    endX = firstX - pt.x;
    endY = firstY - pt.y;
    if (btnReact || !isTouchReacted) {
        btnReact = false;
        return;
    }
    if(abs(endX) > abs(endY))
    {
        //手势向左右
        //判断向左还是向右
        if(endX > 25)
        {
            //向左
            if(doLeft2()){
                autoCreateCardNumber();
                recordData();
            }
            doCheckGameOver();
        }
        else if(endX < -25)
        {
            //向右
            if(doRight2()){
                autoCreateCardNumber();
                recordData();
            }
            doCheckGameOver();
        }
    }
    else
    {
        //手势向上下
        //判断手势向上还是向下
        if(endY > 25)
        {
            //向下
            if(doDown2()){
                autoCreateCardNumber();
                recordData();
            }
            doCheckGameOver();
        }
        else if(endY < -25)
        {
            //向上
            if(doUp2()){
                autoCreateCardNumber();
                recordData();
            }
            doCheckGameOver();
        }
        
    }
}

float move_time = .25;

bool GameLayer::withinBounds(Point cell){
    return cell.x >=0 && cell.x < 4 && cell.y >=0 && cell.y < 4;
}

bool GameLayer::cellAvailable(Point cell){
    int x = cell.x, y = cell.y;
    return !isCellOcuppiedAt(x, y);
}

Position GameLayer::checkfathest(Point cell, Point dir)
{
    Position p;
    Point prev = Point(0,0), next=cell;
    do {
        prev = next;
        next = Point(prev.x + dir.x, prev.y + dir.y);
    } while (withinBounds(next) && cellAvailable(next));
    p.fathest = prev;
    p.next = next;
    return p;
}



bool GameLayer::doLeft2(){
    bool isdo = false;
    Point dir = Point(-1, 0);
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (this->isCellOcuppiedAt(x, y)) {
                Cards *c = this->getTile(x, y);
                Position p = checkfathest(Point(x, y),dir);
                Cards *next = getTile(p.next.x, p.next.y); //cardArr[(int)p.next.x][(int)p.next.y];
                
                if (next && next->getNumber() == c->getNumber() && !next->isMerging) {
                    Cards *merged =Cards::CreateSprite(c->getNumber()<<1, p.next); // new Tile(positions.next, tile.value * 2)
                    Mergefrom mg;
                    mg.f = c;
                    mg.m = next;
                    merged->mg = mg;
                    merged->isMerging = true;
                    float aniDelay = c->updatePosition(p.next,true);
                    cardArr[merged->getX()][merged->getY()]->mergeRemove(aniDelay);
                    cardArr[merged->getX()][merged->getY()] = merged;
                    cardArr[x][y] = nullptr;
                    
                    addChild(merged);
                    /*
                     // Update the score
                     self.score += merged.value;*/
                    
                    // The mighty 2048 tile
                    //if (merged.value == 2048) self.won = true;
                    last_score = score;
                    score += merged->getNumber();
                    scoreLabel->setString(String::createWithFormat("%i",score)->getCString());
                    
                    isdo = true;
                } else {
                    bool b = moveTile(c, p.fathest);
                    if(!isdo) isdo = b;
                }
            }
        }
    }
    return isdo;
}


bool GameLayer::doRight2(){
    bool isdo = false;
    Point dir = Point(1, 0);
    for (int y = 0; y < 4; y++) {
        for (int x = 3; x >= 0; x--) {
            if (this->isCellOcuppiedAt(x, y)) {
                Cards *c = this->getTile(x, y);
                Position p = checkfathest(Point(x, y),dir);
                Cards *next = getTile(p.next.x, p.next.y); //cardArr[(int)p.next.x][(int)p.next.y];
                
                if (next && next->getNumber() == c->getNumber() && !next->isMerging) {
                    Cards *merged =Cards::CreateSprite(c->getNumber()<<1, p.next); // new Tile(positions.next, tile.value * 2)
                    Mergefrom mg;
                    mg.f = c;
                    mg.m = next;
                    merged->mg = mg;
                    next->isMerging = true;
                    float aniDelay = c->updatePosition(p.next,true);
                    cardArr[merged->getX()][merged->getY()]->mergeRemove(aniDelay);
                    cardArr[merged->getX()][merged->getY()] = merged;
                    cardArr[x][y] = nullptr;
                    addChild(merged);
                    /*
                     // Update the score
                     self.score += merged.value;*/
                    
                    // The mighty 2048 tile
                    //if (merged.value == 2048) self.won = true;
                    last_score = score;
                    score += merged->getNumber();
                    scoreLabel->setString(String::createWithFormat("%i",score)->getCString());
                    isdo = true;
                } else {
                    bool b = moveTile(c, p.fathest);
                    if(!isdo) isdo = b;
                }
            }
        }
    }
    return isdo;
}

bool GameLayer::doDown2(){
    bool isdo = false;
    Point dir = Point(0, -1);
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (this->isCellOcuppiedAt(x, y)) {
                Cards *c = this->getTile(x, y);
                Position p = checkfathest(Point(x, y),dir);
                Cards *next = getTile(p.next.x, p.next.y); //cardArr[(int)p.next.x][(int)p.next.y];
                
                if (next && next->getNumber() == c->getNumber() && !next->isMerging) {
                    Cards *merged =Cards::CreateSprite(c->getNumber()<<1, p.next); // new Tile(positions.next, tile.value * 2)
                    Mergefrom mg;
                    mg.f = c;
                    mg.m = next;
                    merged->mg = mg;
                    merged->isMerging = true;
                    float aniDelay = c->updatePosition(p.next,true);
                    cardArr[merged->getX()][merged->getY()]->mergeRemove(aniDelay);
                    cardArr[merged->getX()][merged->getY()] = merged;
                    cardArr[x][y] = nullptr;
                    addChild(merged);
                    /*
                     // Update the score
                     self.score += merged.value;*/
                    
                    // The mighty 2048 tile
                    //if (merged.value == 2048) self.won = true;
                    last_score = score;
                    score += merged->getNumber();
                    scoreLabel->setString(String::createWithFormat("%i",score)->getCString());
                    isdo = true;
                } else {
                    bool b = moveTile(c, p.fathest);
                    if(!isdo) isdo = b;
                }
            }
        }
    }
    return isdo;
}

bool GameLayer::doUp2(){
    bool isdo = false;
    Point dir = Point(0, 1);
    for (int x = 0; x < 4; x++) {
        for (int y = 3; y >=0; y--) {
            if (this->isCellOcuppiedAt(x, y)) {
                Cards *c = this->getTile(x, y);
                Position p = checkfathest(Point(x, y),dir);
                Cards *next = getTile(p.next.x, p.next.y); //cardArr[(int)p.next.x][(int)p.next.y];
                
                if (next && next->getNumber() == c->getNumber() && !next->isMerging) {
                    Cards *merged =Cards::CreateSprite(c->getNumber()<<1, p.next); // new Tile(positions.next, tile.value * 2)
                    Mergefrom mg;
                    mg.f = c;
                    mg.m = next;
                    merged->mg = mg;
                    merged->isMerging = true;
                    float aniDelay = c->updatePosition(p.next,true);
                    cardArr[merged->getX()][merged->getY()]->mergeRemove(aniDelay);
                    cardArr[merged->getX()][merged->getY()] = merged;
                    cardArr[x][y] = nullptr;
                    addChild(merged);
                    /*
                     // Update the score
                     self.score += merged.value;*/
                    
                    // The mighty 2048 tile
                    //if (merged.value == 2048) self.won = true;
                    last_score = score;
                    score += merged->getNumber();
                    scoreLabel->setString(String::createWithFormat("%i",score)->getCString());
                    isdo = true;
                } else {
                    bool b = moveTile(c, p.fathest);
                    if(!isdo) isdo = b;
                }
            }
        }
    }
    return isdo;
}

bool GameLayer::moveTile(Cards *tile, Point pos){
    int ox = tile->pos.x, oy = tile->pos.y, nx = pos.x, ny = pos.y;
    if (ox == nx && oy == ny) {
        return false;
    }
    cardArr[ox][oy] = nullptr;
    cardArr[nx][ny] = tile;
    tile->updatePosition(pos);
    return true;
}

bool GameLayer::isCellOcuppiedAt(int x, int y){
    return getTile(x, y) != nullptr;
}

Cards* GameLayer::getTile(int x, int y){
    if (x>3 || x <0 || y>3 || y <0) {
        return nullptr;
    }
    return cardArr[x][y];
}

//判断游戏是否还能继续
void GameLayer::doCheckGameOver(){
    bool isGameOver = true;
    
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (isCellOcuppiedAt(x, y) == 0||
                (x>0 && cardArr[x-1][y] && (cardArr[x][y]->getNumber() == cardArr[x-1][y]->getNumber()))||
                (x<3 && cardArr[x+1][y] && (cardArr[x][y]->getNumber() == cardArr[x+1][y]->getNumber()))||
                (y>0 && cardArr[x][y-1] && (cardArr[x][y]->getNumber() == cardArr[x][y-1]->getNumber()))||
                (y<3 && cardArr[x][y+1] && (cardArr[x][y]->getNumber() == cardArr[x][y+1]->getNumber()))) {
                isGameOver = false;
            }
        }
    }
    
    if (isGameOver) {
        //游戏结束，重新开始游戏
        log("游戏结束");
        initData();
        Director::getInstance()->replaceScene(TransitionFade::create(1, GameLayer::createScene()));
    }
}




void GameLayer::menuCloseCallback(Ref* pSender)
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
