#include <stdlib.h>
#include "Enemy.h"
#include "EnemyController.h"

using namespace cocos2d;
//using namespace CocosDenshion;

//CCSprite *heningerBattle;
CCAction *heningerRunAnimation;
CCAction *heningerStopAnimation;
CCAction *heningerAttackAnimation;

Enemy* Enemy::create(const char *filename) {
	Enemy *e = new Enemy();
	if (e && e->initWithFile(filename)) {
// Set to autorelease
		e->autorelease();
		return e;
	}
	CC_SAFE_DELETE(e);
	return NULL;
}

void Enemy::initEnemy() {
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(
			"heninger.plist");
	defineAnimations();
	CCLog("initEnemy");
	int x = rand() % 2000;
	int y = rand() % 1000;
//	this->setPosition(ccp(1000, 200));
	this->setPosition(ccp(x, y));
}

void Enemy::defineAnimations() {
	const char *movname = "mov";
	char frameMovBuf[32] = { 0 };
	char frameStopBuf[32] = { 0 };
	char frameAttackBuf[32] = { 0 };

	CCArray *movAnimFrames = CCArray::create();
	for (int i = 1; i <= 4; ++i) {
		sprintf(frameMovBuf, "%s%d.png", movname, i);
		movAnimFrames->addObject(
				CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
						frameMovBuf));
	}
	CCAnimation *movAnim = CCAnimation::createWithSpriteFrames(movAnimFrames,
			0.15f);
	heningerRunAnimation = CCRepeatForever::create(CCAnimate::create(movAnim));
	heningerRunAnimation->retain();

	//
	CCArray *stopAnimFrames = CCArray::create();
	for (int i = 11; i <= 13; ++i) {
		sprintf(frameStopBuf, "%s%d.png", movname, i);
		stopAnimFrames->addObject(
				CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
						frameMovBuf));
	}
	CCAnimation *stopAnim = CCAnimation::createWithSpriteFrames(stopAnimFrames,
			0.15f);
	heningerStopAnimation = CCRepeatForever::create(
			CCAnimate::create(stopAnim));
	heningerStopAnimation->retain();

	//
	CCArray *attackAnimFrames = CCArray::create();
	for (int i = 6; i <= 10; ++i) {
		sprintf(frameAttackBuf, "%s%d.png", movname, i);
		attackAnimFrames->addObject(
				CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
						frameAttackBuf));
	}
	CCAnimation *attackAnim = CCAnimation::createWithSpriteFrames(
			attackAnimFrames, 0.15f);
	heningerAttackAnimation = CCRepeatForever::create(
			CCAnimate::create(attackAnim));
	heningerAttackAnimation->retain();

}

void Enemy::moveTo(CCPoint location) {
	CCLOG("%s \n", "moveTo1");

	CCFiniteTimeAction *actionMove = CCMoveTo::create(1, location);
	CCFiniteTimeAction *endMoveCallBack = CCCallFuncN::create(this,
			callfuncN_selector(Enemy::endMoveEnemyCallBack));

	CCSequence *actionSequence = CCSequence::create(actionMove, endMoveCallBack,
			NULL);
	CCLOG("%s \n", "moveTo2");

	actionSequence->retain();

	this->runAction(actionSequence);
	CCLOG("%s \n", "moveTo3");

	if (location.x > this->getPosition().x) {
		CCLOG("%s \n", "Flip X");
		this->setFlipX(true);
	} else if (this->isFlipX() == true) {
		CCLOG("%s \n", "Not Flip X");
		this->setFlipX(false);
	}

	this->runAction(heningerRunAnimation);

}

void Enemy::endMoveEnemyCallBack(CCNode* sender) {
	CCLOG("%s \n", "endMoveCallBack");

//	this->schedule(schedule_selector(Enemy::delayEnemy), -1, -1, 3);
//	this->scheduleOnce(schedule_selector(Enemy::delayEnemy), 3);

	EnemyController::Instance()->vanish(0);

	//	CCLOG("%s \n", "endMoveCallBack-vanish");
//	this->stopAllActions();


//	this->runAction(heningerRunAnimation);
	//heningerBattle->runAction(heroStopped);
}

void Enemy::delayEnemy(float dt) {

	CCLOG("%s \n", "delay enemy");
	EnemyController::Instance()->vanish(0);
}
void Enemy::attack(CCPoint location) {
	CCLOG("Attack");

	CCFiniteTimeAction *actionMove = CCMoveTo::create(5, location);
	CCFiniteTimeAction *endAttackCallBack = CCCallFuncN::create(this,
			callfuncN_selector(Enemy::endAttackEnemyCallBack));

	CCSequence *actionSequence = CCSequence::create(actionMove, endAttackCallBack,
			NULL);
	CCLOG("%s \n", "Attack2");

	actionSequence->retain();

	this->runAction(actionSequence);
	CCLOG("%s \n", "Attack3");

	if (location.x > this->getPosition().x) {
		CCLOG("%s \n", "Flip X");
		this->setFlipX(true);
	} else if (this->isFlipX() == true) {
		CCLOG("%s \n", "Not Flip X");
		this->setFlipX(false);
	}

	this->runAction(heningerAttackAnimation);
//	this->stopAllActions();


}


void Enemy::endVanishEnemyCallBack(CCNode* sender){
	CCLOG("endVanishEnemyCallBack");

}
void Enemy::endAttackEnemyCallBack(CCNode* sender) {
	EnemyController::Instance()->gameOver(0);
//	this->schedule(schedule_selector(EnemyController::gameOver), 20);

}

