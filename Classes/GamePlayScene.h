/*
 * GamePlayScene.h
 *
 *  Created on: Jun 19, 2013
 *      Author: vntraal
 */

#ifndef GAMEPLAYSCENE_H_
#define GAMEPLAYSCENE_H_

#include "cocos2d.h"

using namespace cocos2d;

class GamePlayScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // touch events
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void endMoveCallBack(CCNode* sender);

    // implement the "static node()" method manually
    CREATE_FUNC(GamePlayScene);

private:
    void delayTest(float dt);
};

#endif /* GAMEPLAYSCENE_H_ */

