/*
 * MenuScene.h
 *
 *  Created on: Jul 10, 2013
 *      Author: munhra
 */

#ifndef MENUSCENE_H_
#define MENUSCENE_H_

#include "cocos2d.h"

class MenuScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    // a selector callback
    void menuPlayCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(MenuScene);
};

#endif /* MENUSCENE_H_ */
