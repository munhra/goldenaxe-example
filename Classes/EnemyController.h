#ifndef ENEMYCONTROLLER_H_
#define ENEMYCONTROLLER_H_

#include "cocos2d.h"
#include "Enemy.h"

using namespace cocos2d;

class EnemyController : public cocos2d::CCObject
{
public:
	static EnemyController* Instance();
//	static bool isGameOver;


	void createWave(int waveNumber, CCLayer* layer, CCPoint heroPosition, CCSprite* heroSprite);
	void update();
	CCArray* getArrayEnemies();

	void vanish(float dt);
	void gameOver(float dt);
	void setIsGameOver();
private:

	CCArray* arrayEnemy;
	CCSprite *hero;
	CCLayer* layerGame;

	EnemyController(){}; // Private so that it can  not be called
	EnemyController(EnemyController const&) {}; // copy constructor is private
	EnemyController& operator=(EnemyController const&){}; // assignment operator is private
	static EnemyController* instance;

	void createEnemy(int qtdEnemy, CCPoint heroPosition, CCLayer* layer);
	void moveWaveEnemies(CCPoint heroPosition);

};

#endif
