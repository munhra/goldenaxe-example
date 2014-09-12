/*
 * GamePlayScene.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: vntraal
 */

#include "GamePlayScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"


using namespace cocos2d;
using namespace CocosDenshion;

CCAction *heroAnimAction;
CCAction *heroRunAnimation;
CCAction *heroMoveAction;
CCAction *heroStopped;
CCSprite *axeBattle;
CCLabelTTF* logLabel;
bool run;

CCScene* GamePlayScene::scene() {
	CCScene *scene = CCScene::create();
	GamePlayScene *layer = GamePlayScene::create();
	scene->addChild(layer);
	return scene;
}

bool GamePlayScene::init() {

	if (!CCLayer::init()) {
		return false;
	}

	const char *name = "walk";
	char frameNameBuf[32] = { 0 };

	const char *runname = "run";
	char frameRunBuf[32] = { 0 };

	char stopBuf[32] = { 0 };

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(
			"gAxeWalk.plist");

	axeBattle = CCSprite::create("walk1.png");
	axeBattle->retain();
	axeBattle->setPosition(ccp(200, 200));

	run = false;

	CCArray *walkAnimFrames = CCArray::create();

	for (int i = 1; i <= 4; ++i) {
		sprintf(frameNameBuf, "%s%d.png", name, i);
		walkAnimFrames->addObject(
				CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
						frameNameBuf));
	}

	CCAnimation *walkAnim = CCAnimation::createWithSpriteFrames(walkAnimFrames,
			0.15f);

	heroAnimAction = CCRepeatForever::create(CCAnimate::create(walkAnim));

	heroAnimAction->retain();

	CCArray *runAnimFrames = CCArray::create();

	for (int i = 1; i <= 3; ++i) {
		sprintf(frameRunBuf, "%s%d.png", runname, i);
		runAnimFrames->addObject(
				CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
						frameRunBuf));
	}

	CCAnimation *runAnim = CCAnimation::createWithSpriteFrames(runAnimFrames,
			0.15f);

	heroRunAnimation = CCRepeatForever::create(CCAnimate::create(runAnim));

	heroRunAnimation->retain();

	CCArray *stopAnimFrames = CCArray::create();

	for (int i = 1; i <= 1; ++i) {
		sprintf(stopBuf, "%s%d.png", name, i);
		stopAnimFrames->addObject(
				CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
						stopBuf));
	}

	CCAnimation *stopAnim = CCAnimation::createWithSpriteFrames(stopAnimFrames,
			0.15f);

	heroStopped = CCRepeatForever::create(CCAnimate::create(stopAnim));

	heroStopped->retain();

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	logLabel = CCLabelTTF::create("Hello World XXXX", "Thonburi", 34);
	logLabel->setPosition(ccp(size.width / 2, size.height - 20));
	logLabel->setString("Golden Axe !");
	logLabel->retain();

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(
			this, 0, true);

	this->addChild(axeBattle);
	this->addChild(logLabel);
	this->setTouchEnabled(true);
	this->schedule(schedule_selector(GamePlayScene::delayTest), 1.0);

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png",
			"CloseSelected.png", this,
			menu_selector(GamePlayScene::menuCloseCallback));
	pCloseItem->setPosition(
			ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	this->schedule(schedule_selector(GamePlayScene::delayTest),12,-1,0);

	return true;
}

void GamePlayScene::delayTest(float dt) {
	logLabel->setString("Timer delayTest");
	CCLOG("%s \n", "schedule with delay test");
}

void GamePlayScene::endMoveCallBack(CCNode* sender) {
	CCLOG("%s \n", "endMoveCallBack");
	axeBattle->stopAllActions();
	axeBattle->runAction(heroStopped);
}

bool GamePlayScene::ccTouchBegan(cocos2d::CCTouch* touch,
		cocos2d::CCEvent* event) {
	CCLOG("%s \n", "Touch Began");
	return true;
}

void GamePlayScene::ccTouchMoved(cocos2d::CCTouch* touch,
		cocos2d::CCEvent* event) {
	CCLOG("%s \n", "Touch Moved");
	run = true;
}

void GamePlayScene::ccTouchEnded(cocos2d::CCTouch* touch,
		cocos2d::CCEvent* event) {
	CCLOG("%s \n", "Touch Ended");

	CCPoint location = this->convertTouchToNodeSpace(touch);
	axeBattle->stopAllActions();

	CCFiniteTimeAction *actionMove = CCMoveTo::create(1, location);
	CCFiniteTimeAction *endMoveCallBack = CCCallFuncN::create(this,
			callfuncN_selector(GamePlayScene::endMoveCallBack));

	CCSequence *actionSequence = CCSequence::create(actionMove, endMoveCallBack,
			NULL);

	actionSequence->retain();

	axeBattle->runAction(actionSequence);

	if (location.x > axeBattle->getPosition().x) {
		CCLOG("%s \n", "Flip X");
		axeBattle->setFlipX(true);
	} else if (axeBattle->isFlipX() == true) {
		CCLOG("%s \n", "Not Flip X");
		axeBattle->setFlipX(false);
	}

	if (run == true) {
		axeBattle->runAction(heroRunAnimation);
	} else {
		axeBattle->runAction(heroAnimAction);
	}

	run = false;
}

void GamePlayScene::menuCloseCallback(CCObject* pSender) {

	CCLOG("Game Menu !");
	this->removeFromParentAndCleanup(true);
	CCDirector::sharedDirector()->replaceScene(
	CCTransitionZoomFlipX::create(0.5, MenuScene::scene()));

}
