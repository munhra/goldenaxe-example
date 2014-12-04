#ifndef ENEMY_H_
#define ENEMY_H_

#include "cocos2d.h"

using namespace cocos2d;

class Enemy : public cocos2d::CCSprite
{
public:
	static Enemy* create(const char *filename);
	void initEnemy();
	void move(float x, float y);
	void moveTo(CCPoint location);
    virtual void endMoveEnemyCallBack(CCNode* sender);
    virtual void endVanishEnemyCallBack(CCNode* sender);
    virtual void endAttackEnemyCallBack(CCNode* sender);
	void attack(CCPoint location);
//	void vanish();

private:
	//CCPoint *position;
	void defineAnimations();
	void detectHeroCollision();
	void delayEnemy(float dt);
};

#endif
