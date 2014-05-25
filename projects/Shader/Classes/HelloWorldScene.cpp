#include "HelloWorldScene.h"
#include "MapSprite.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::create();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    setTouchEnabled(true);
    m_light = CCPointMake(size.width/2, size.height/2);
    MapSprite* pSprite = MapSprite::create("ground", 0,  this);
    pSprite->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(pSprite);
    
    return true;
}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch* touch = (CCTouch*)pTouches->anyObject();
    m_light = this->convertTouchToNodeSpace(touch);
}
