#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "GamePlayScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* MenuScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    MenuScene *layer = MenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCLayerColor *blueSky = CCLayerColor::create(ccc4(100, 100, 250, 255));
    this->addChild(blueSky);


    CCSize size = CCDirector::sharedDirector()->getWinSize();

    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "press_start.png",
                                        "press_start.png",
                                        this,
                                        menu_selector(MenuScene::menuPlayCallback) );

    pCloseItem->setPosition(ccp((size.width/2) - 120 , (size.height/2) - 120));

    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);

    CCLabelTTF* pLabel = CCLabelTTF::create("Menu", "Thonburi", 34);

    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    this->addChild(pMenu, 1);

    return true;
}

void MenuScene::menuPlayCallback(CCObject* pSender)
{
	CCLOG("Game Play !");
	this->removeFromParentAndCleanup(true);
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionZoomFlipX::create(0.5, GamePlayScene::scene()));
}
