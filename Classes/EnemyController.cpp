#include <stddef.h>  // defines NULL
#include <math.h>
#include "EnemyController.h"
#include "Enemy.h"

using namespace cocos2d;

// Global static pointer used to ensure a single instance of the class.
EnemyController* EnemyController::instance = NULL;

static bool isGameOver = false;


EnemyController* EnemyController::Instance() {
	if (!instance)
		instance = new EnemyController;

	CCLOG("%s \n", "Instance");
	return instance;
}

void EnemyController::createWave(int waveNumber, CCLayer* layer,
		CCPoint heroPosition, CCSprite* heroSprite) {
	hero = heroSprite;
	int qtd = pow(waveNumber, 2);
	CCLOG("%s \n", "createWave" + qtd);
	if (!isGameOver && waveNumber < 9) {
		createEnemy(qtd, heroPosition, layer);
	}
}

void EnemyController::createEnemy(int qtdEnemy, CCPoint heroPosition,
		CCLayer* layer) {
	layerGame = layer;

	CCLOG("%s \n", "Creating enemies...");
	CCArray *arrayEnemies = CCArray::create();
	for (int i = 0; i < qtdEnemy; i++) {
		Enemy *enemy;
		CCLOG("%s \n", "Create Enemy");
		enemy = Enemy::create("mov1.png");
		enemy->initEnemy();
		CCLOG("%s \n", "addChild");

		layer->addChild(enemy);
		CCLOG("%s \n", "addedChild");

		arrayEnemies->addObject(enemy);
	}
	arrayEnemies->retain();
	arrayEnemy = arrayEnemies;

	moveWaveEnemies(heroPosition);
}

void EnemyController::moveWaveEnemies(CCPoint heroPosition) {
	CCObject* enemy;
	CCLOG("moveWaveEnemies");
	CCARRAY_FOREACH(arrayEnemy, enemy) {
		Enemy *enemy1 = static_cast<Enemy*>(enemy);
		enemy1->moveTo(heroPosition);
	}

//	GamePlayScene::scene()->schedule(schedule_selector(update));

}

void EnemyController::update() {
	CCArray* enemiesToDelete = new CCArray;

	CCLOG("%s \n", "Update_Controller1");

	if (!isGameOver) {
		CCObject* it;

		CCRect heroRect = CCRectMake(
				hero->getPosition().x - (hero->getContentSize().width/2),
				hero->getPosition().y - (hero->getContentSize().height/2),
				hero->getContentSize().width,
				hero->getContentSize().height);

		CCLOG("arrayenemy");

		CCARRAY_FOREACH(arrayEnemy, it) {
			CCLOG("foreach arrayenemy");
			Enemy *enemy = static_cast<Enemy*>(it);
			CCLOG("%s \n", "Update_Controller3");

			CCRect enemyRect = CCRectMake(
					enemy->getPosition().x - (enemy->getContentSize().width/2),
					enemy->getPosition().y - (enemy->getContentSize().height/2),
					enemy->getContentSize().width,
					enemy->getContentSize().height);
			CCLOG("%s \n", "Update_Controller4");

			if (enemyRect.intersectsRect(heroRect)) {
				CCLOG("%s \n", "Collision!");
				enemy->attack(enemy->getPosition());
				//vanish();
                isGameOver = true;
			} else {
				//layerGame->removeChild(enemy, true);
				//enemiesToDelete->addObject(enemy);
			}
		}


/*
		CCObject* jt;
		CCARRAY_FOREACH(enemiesToDelete, jt) {
			Enemy *enemy = static_cast<Enemy*>(jt);
			enemiesToDelete->removeObject(enemy);
			layerGame->removeChild(enemy, true);
		}
		enemiesToDelete->release();
*/
	} else {
		return;
	}
}

CCArray* EnemyController::getArrayEnemies() {
	return arrayEnemy;
}

void EnemyController::vanish(float dt) {

	CCLOG("removing enemy");

	CCLOG("removing enemy - arrayenemies1");
	CCObject* jt;
	CCARRAY_FOREACH(arrayEnemy, jt) {
		Enemy *enemy = static_cast<Enemy*>(jt);
		arrayEnemy->removeObject(enemy);
		layerGame->removeChild(enemy, true);
	}
	CCLOG("removing enemy - releasing");
//	arrayEnemy->release();
	CCLOG("enemies removed");

}

void EnemyController::gameOver(float dt) {

	isGameOver = true;

	vanish(0);

	CCLabelTTF* label = CCLabelTTF::create("Game Over", "Helvetica", 200,
	                                      CCSizeMake(245, 32), kCCTextAlignmentCenter);
	layerGame->addChild(label);
	layerGame->retain();
	CCLOG("layer Game Over");

}

void EnemyController::setIsGameOver(){
	isGameOver = false;
}
