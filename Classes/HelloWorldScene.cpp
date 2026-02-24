#include "HelloWorldScene.h"

#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene *HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Resources/' in front of filenames in "
        "HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    initWorld();
    addBird();
    addBarContainer();
    addGround();

    setTouchEnabled(true);

    scheduleOnce(schedule_selector(HelloWorld::startGame), 3);
}


void HelloWorld::initWorld(){
    world = new b2World(b2Vec2(0, -10));
    world->SetContactListener(this);
}

void HelloWorld::startGame(float dt){
    scheduleUpdate();
    schedule(schedule_selector(HelloWorld::addBar), 1);
}

void HelloWorld::stopGame(){
    unscheduleUpdate();
    unschedule(schedule_selector(HelloWorld::addBar));
}


void HelloWorld::addBird(){

    bird = B2Sprite::create("bird.png");
    CCSize size = bird->getContentSize();

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(screenSize.width/2/RATIO, screenSize.height/2/RATIO);
    b2Body * birdBody = world->CreateBody(&bodyDef);

    b2PolygonShape birdShape;
    birdShape.SetAsBox(size.width/2/RATIO, size.height/2/RATIO);

    b2FixtureDef birdFixtureDef;
    birdFixtureDef.shape= &birdShape;
    birdBody->CreateFixture(&birdFixtureDef);

    bird->setPTMRatio(RATIO);
    bird->setB2Body(birdBody);
    addChild(bird);
}

void HelloWorld::addBar(float dt){

    float offset = -rand()%5;

    //down bar
    B2Sprite * down_bar = B2Sprite::create("down_bar.png");
    CCSize down_bar_size = down_bar->getContentSize();

    b2BodyDef down_bar_body_def;
    down_bar_body_def.type = b2_kinematicBody;
    down_bar_body_def.position = b2Vec2(screenSize.width/RATIO+2, down_bar_size.height/RATIO/2+offset);
    down_bar_body_def.linearVelocity = b2Vec2(-5, 0);
    b2Body * down_bar_body = world->CreateBody(&down_bar_body_def);

    b2PolygonShape down_bar_shape;
    down_bar_shape.SetAsBox(down_bar_size.width/2/RATIO, down_bar_size.height/2/RATIO);
    b2FixtureDef down_bar_fixture_def;
    down_bar_fixture_def.shape = &down_bar_shape;
    down_bar_body->CreateFixture(&down_bar_fixture_def);

    down_bar->setB2Body(down_bar_body);
    down_bar->setPTMRatio(RATIO);
    barContainer-> addChild(down_bar);

    //up bar
    B2Sprite * up_bar = B2Sprite::create("up_bar.png");
    CCSize up_bar_size = up_bar->getContentSize();

    b2BodyDef up_bar_body_def;
    up_bar_body_def.type = b2_kinematicBody;
    up_bar_body_def.position = b2Vec2(screenSize.width/RATIO+2, down_bar_size.height/RATIO+offset+2+up_bar_size.height/2/RATIO);
    up_bar_body_def.linearVelocity = b2Vec2(-5, 0);
    b2Body * up_bar_body = world->CreateBody(&up_bar_body_def);

    b2PolygonShape up_bar_shape;
    up_bar_shape.SetAsBox(up_bar_size.width/2/RATIO, up_bar_size.height/2/RATIO);
    b2FixtureDef up_bar_fixture_def;
    up_bar_fixture_def.shape = &up_bar_shape;
    up_bar_body->CreateFixture(&up_bar_fixture_def);

    up_bar->setB2Body(up_bar_body);
    up_bar->setPTMRatio(RATIO);
    barContainer-> addChild(up_bar);

}

void HelloWorld::addBarContainer(){
    barContainer = CCSprite::create();
    addChild(barContainer);
}


void HelloWorld::addGround(){

    B2Sprite *ground = B2Sprite::create("ground.png");
    CCSize size = ground->getContentSize();

    b2BodyDef bDef;
    bDef.type = b2_staticBody;
    bDef.position = b2Vec2(size.width/2/RATIO, size.height/2/RATIO);
    b2Body *groundBody = world->CreateBody(&bDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(size.width/2/RATIO, size.height/2/RATIO);
    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundShape;
    groundBody->CreateFixture(&groundFixtureDef);

    ground->setB2Body(groundBody);
    ground->setPTMRatio(RATIO);
    addChild(ground);
}

void HelloWorld::update(float dt){
    world->Step(dt, 8, 3);
    CCSprite *s;

    for (b2Body *b = world->GetBodyList(); b!=NULL; b=b->GetNext()) {
        if (b->GetPosition().x<-3) {
            s = (CCSprite*)b->GetUserData();
            if (s!=NULL) {
                s->removeFromParent();
            }

            world->DestroyBody(b);
        }
    }
}

void HelloWorld::BeginContact(b2Contact *contact){
    if (contact->GetFixtureA()->GetBody()->GetUserData()==bird||
        contact->GetFixtureB()->GetBody()->GetUserData()==bird) {

        stopGame();

        CCMessageBox("游戏失败", "游戏失败");
    }
}


void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){

    bird->getB2Body()->SetLinearVelocity(b2Vec2(0, 5));

}

void HelloWorld::menuCloseCallback(Ref *pSender)
{
    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /* To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * Director::getInstance()->end() as given above,instead trigger a custom event created in
     * RootViewController.mm as below */

    // EventCustom customEndEvent("game_scene_close_event");
    // _eventDispatcher->dispatchEvent(&customEndEvent);
}
