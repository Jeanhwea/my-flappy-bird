#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "B2Sprite.h"

#define RATIO 48.0f

class HelloWorld : public cocos2d::Scene, public b2ContactListener
{
public:
    static cocos2d::Scene *createScene();

    virtual bool init();

    void menuCloseCallback(cocos2d::Ref *pSender);

    CREATE_FUNC(HelloWorld);

    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
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

#endif
