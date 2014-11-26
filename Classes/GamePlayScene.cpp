/*
 * GamePlayScene.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: vntraal
 */

#include "GamePlayScene.h"
#include "SimpleAudioEngine.h"
#include "math.h"

using namespace cocos2d;
using namespace CocosDenshion;

int onda = 1;
CCAction *heroAnimAction;
CCAction *inimigoAndando;
CCAction *inimigoMorrendo;
CCAction *inimigoBatendo;
CCAction *heroRunAnimation;
CCAction *heroMoveAction;
CCAction *heroStopped;
CCSprite *axeBattle;
CCLabelTTF* labelVidas;
CCLabelTTF* logLabel;
bool run, proximo;

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

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(
			"Inimigo01.plist");

	axeBattle = CCSprite::create("walk1.png");
	axeBattle->retain();
	axeBattle->setPosition(ccp(200, 200));
	proximo = true;
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

	labelVidas = CCLabelTTF::create("x", "Thonburi", 34);
	labelVidas->setPosition(ccp(size.width / 2, size.height - 50));

	sprintf(stopBuf, "Onda %d", onda);
	logLabel->setString(stopBuf);

	sprintf(stopBuf, "Inimigos %d", 2);
	labelVidas->setString(stopBuf);

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(
			this, 0, true);

	this->addChild(axeBattle);
	this->addChild(logLabel);
	this->addChild(labelVidas);
	GamePlayScene::inimigosVivos = CCArray::create();

	GamePlayScene::inimigosVivos->retain();

	this->setTouchEnabled(true);

	schedule(schedule_selector(GamePlayScene::calculaColisao));

	schedule(schedule_selector(GamePlayScene::criaInimigo),
			onda + 2 + pow(2, onda));

	return true;
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
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GamePlayScene::criaInimigo() {

	static unsigned int seed = time(NULL);
	char stopBuf[32] = { 0 };
	CCLOG("==================================================== onda %d", onda);

	if (onda < 9 && GamePlayScene::inimigosVivos->count() == 0) {
		sprintf(stopBuf, "Onda %d", onda);
		logLabel->setString(stopBuf);

		for (int aux = 0; aux < pow(2, onda); ++aux) {
			CCLOG(
					"==================================================== onda %d",
					onda);
			CCLOG("==================================================== Aux %d",
					aux);
			CCSize size = CCDirector::sharedDirector()->getWinSize();

			CCPoint in, fi;

			CCSprite *inimigo = CCSprite::createWithSpriteFrameName(
					"InimigoAndando01.png");

			addChild(inimigo);

			srand(seed);

			seed = rand();

			if (seed % 4 == 0) {

				srand(seed);

				seed = rand();

				in.x = (((float) seed) / RAND_MAX) * size.width;

				in.y = size.height + inimigo->boundingBox().size.height / 2;

			} else if (seed % 4 == 1) {

				srand(seed);

				seed = rand();

				in.x = size.width + inimigo->boundingBox().size.width / 2;

				in.y = (((float) seed) / RAND_MAX) * size.height;

			} else if (seed % 4 == 2) {

				srand(seed);

				seed = rand();

				in.x = (((float) seed) / RAND_MAX) * size.width;

				in.y = -inimigo->boundingBox().size.height / 2;

			} else {

				srand(seed);

				seed = rand();

				in.x = -inimigo->boundingBox().size.width / 2;

				in.y = (((float) seed) / RAND_MAX) * size.height;

			}

			fi = axeBattle->getPosition();

			CCArray *inimigoMorrendoArray = CCArray::create();

			inimigoMorrendoArray->addObject(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							"InimigoMorrendo01.png"));
			inimigoMorrendoArray->addObject(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							"InimigoMorrendo02.png"));
			inimigoMorrendoArray->addObject(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							"InimigoMorrendo03.png"));

			CCAnimation *inimigoMorrendoAnimacao =
					CCAnimation::createWithSpriteFrames(inimigoMorrendoArray,
							0.15f);

			inimigoMorrendo = CCRepeatForever::create(
					CCAnimate::create(inimigoMorrendoAnimacao));

			inimigoMorrendo->retain();

			CCArray *inimigoBatendoArray = CCArray::create();

			inimigoBatendoArray->addObject(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							"InimigoBatendo01.png"));
			inimigoBatendoArray->addObject(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							"InimigoBatendo02.png"));
			inimigoBatendoArray->addObject(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							"InimigoBatendo03.png"));

			CCAnimation *inimigoBatendoAnimacao =
					CCAnimation::createWithSpriteFrames(inimigoBatendoArray,
							0.15f);

			inimigoBatendo = CCRepeatForever::create(
					CCAnimate::create(inimigoBatendoAnimacao));

			inimigoBatendo->retain();

			CCArray *inimigoAndandoArray = CCArray::create();

			inimigoAndandoArray->addObject(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							"InimigoAndando01.png"));
			inimigoAndandoArray->addObject(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							"InimigoAndando02.png"));
			inimigoAndandoArray->addObject(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							"InimigoAndando03.png"));

			CCAnimation *inimigoAndandoAnimacao =
					CCAnimation::createWithSpriteFrames(inimigoAndandoArray,
							0.15f);

			inimigoAndando = CCRepeatForever::create(
					CCAnimate::create(inimigoAndandoAnimacao));

			inimigoAndando->retain();

			inimigo->setPosition(in);
			GamePlayScene::inimigosVivos->addObject(inimigo);

			seed = rand();
			int potencia = pow(2, onda);

			CCMoveTo* movimento = CCMoveTo::create(
					(onda + int(seed % potencia)), fi);

			CCCallFuncN* desalocacao = CCCallFuncN::create(this,
					callfuncN_selector(GamePlayScene::mataInimigo));

			if (in.x > fi.x) {
				inimigo->setFlipX(true);
			} else if (inimigo->isFlipX() == true) {
				inimigo->setFlipX(false);
			}

			inimigo->runAction(inimigoAndando);
			inimigo->runAction(
					CCSequence::create(movimento, desalocacao, NULL));

		}
		++onda;
	}
}

void GamePlayScene::mataInimigo(CCNode* s) {

	s->stopAllActions();
	s->runAction(inimigoMorrendo);
	CCMoveTo* movimento = CCMoveTo::create(0.45, s->getPosition());
	CCCallFuncN* desalocacao = CCCallFuncN::create(this,
			callfuncN_selector(GamePlayScene::destroiInimigo));
	s->runAction(CCSequence::create(movimento, desalocacao, NULL));
	GamePlayScene::inimigosVivos->removeObject(s, true);

}

void GamePlayScene::fimDeJogo() {
	cleanup();

	setTouchEnabled(false);

	GamePlayScene::inimigosVivos->removeAllObjects();

	GamePlayScene::inimigosVivos->release();

}

void GamePlayScene::destroiInimigo(CCNode* s) {

	removeChild(s);

}

void GamePlayScene::calculaColisao() {

	static CCSize size = CCDirector::sharedDirector()->getWinSize();

	static int raioHeroi = 20;
	static int raioInimigo = 20;

	static int distanciaMaxima = (raioHeroi + raioInimigo)
			* (raioHeroi + raioInimigo);

	char stopBuf[32] = { 0 };

	sprintf(stopBuf, "Inimigos %d", GamePlayScene::inimigosVivos->count());
	labelVidas->setString(stopBuf);

	for (int i = 0; i < GamePlayScene::inimigosVivos->count(); i++) {

		int dist =
				(GamePlayScene::axeBattle->getPosition().x
						- static_cast<CCNode*>(GamePlayScene::inimigosVivos->objectAtIndex(
								i))->getPosition().x)
						* (GamePlayScene::axeBattle->getPosition().x
								- static_cast<CCNode*>(GamePlayScene::inimigosVivos->objectAtIndex(
										i))->getPosition().x)
						+

						(GamePlayScene::axeBattle->getPosition().y
								- static_cast<CCNode*>(GamePlayScene::inimigosVivos->objectAtIndex(
										i))->getPosition().y)
								* (GamePlayScene::axeBattle->getPosition().y
										- static_cast<CCNode*>(GamePlayScene::inimigosVivos->objectAtIndex(
												i))->getPosition().y);

		if (dist <= distanciaMaxima) {

			static_cast<CCNode*>(GamePlayScene::inimigosVivos->objectAtIndex(i))->stopAllActions();
			static_cast<CCNode*>(GamePlayScene::inimigosVivos->objectAtIndex(i))->runAction(
					inimigoBatendo);

			CCMoveTo* movimento = CCMoveTo::create(0.9,
					axeBattle->getPosition());
			CCCallFuncN* desalocacao = CCCallFuncN::create(this,
					callfuncN_selector(GamePlayScene::destroiInimigo));
			static_cast<CCNode*>(GamePlayScene::inimigosVivos->objectAtIndex(i))->runAction(
					CCSequence::create(movimento, desalocacao, NULL));
			GamePlayScene::inimigosVivos->removeObject(
					static_cast<CCNode*>(GamePlayScene::inimigosVivos->objectAtIndex(
							i)), true);

		}

	}

}

