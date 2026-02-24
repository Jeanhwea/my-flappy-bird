#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene *createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref *pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void BeginContact(b2Contact *contact);

    virtual void update(float dt);

    b2World *world;
    B2Sprite *bird;
    CCSize screenSize;
    CCSprite *barContainer;

private:
    void addBird();
    void addGround();
    void initWorld();
    void addBar(float dt);
    void addBarContainer();
    void startGame(float dt);
    void stopGame();
};

#endif  // __HELLOWORLD_SCENE_H__
