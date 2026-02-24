#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "B2Sprite.h"

#define RATIO 48.0f

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene *createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref *pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void BeginContact(b2Contact *contact);

    virtual void update(float dt);

    b2World *world;
    B2Sprite *bird;
    cocos2d::Size screenSize;
    cocos2d::Sprite *barContainer;

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
