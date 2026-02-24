#include "GameScene.h"

USING_NS_CC;

const float PIPE_WIDTH = 52.0f;
const float BIRD_RADIUS = 20.0f;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::initWithPhysics()) {
        return false;
    }

    getPhysicsWorld()->setGravity(Vec2(0, 0));
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);

    _visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _gravity = -1500.0f;
    _jumpForce = 400.0f;
    _pipeSpeed = 120.0f;
    _pipeGap = 120.0f;
    _pipeSpawnInterval = 2.5f;
    _groundHeight = 60.0f;
    _birdVelocity = 0.0f;
    _score = 0;
    _gameState = GAME_STATE_READY;

    auto background = LayerColor::create(Color4B(100, 200, 255, 255));
    this->addChild(background, -10);

    _pipeContainer = Node::create();
    this->addChild(_pipeContainer, 0);

    _ground = Sprite::create("ground.png");
    if (_ground) {
        float scaleX = _visibleSize.width / _ground->getContentSize().width + 0.1f;
        _ground->setScaleX(scaleX);
        _ground->setAnchorPoint(Vec2(0, 0));
        _ground->setPosition(origin.x, origin.y);

        auto groundBody = PhysicsBody::createBox(Size(_visibleSize.width, _groundHeight));
        groundBody->setDynamic(false);
        groundBody->setCategoryBitmask(0x02);
        groundBody->setCollisionBitmask(0x01);
        groundBody->setContactTestBitmask(0x01);
        _ground->setPhysicsBody(groundBody);
        this->addChild(_ground, 9);
    }

    _bird = Sprite::create("bird.png");
    if (_bird) {
        float scale = 0.5f;
        _bird->setScale(scale);
        auto birdBody = PhysicsBody::createCircle(BIRD_RADIUS);
        birdBody->setDynamic(true);
        birdBody->setCategoryBitmask(0x01);
        birdBody->setCollisionBitmask(0x02);
        birdBody->setContactTestBitmask(0x02);
        birdBody->setGravityEnable(false);
        _bird->setPhysicsBody(birdBody);
    }
    _bird->setPosition(origin.x + _visibleSize.width / 4, origin.y + _visibleSize.height / 2);
    this->addChild(_bird, 5);

    _scoreLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 48);
    _scoreLabel->setPosition(origin.x + _visibleSize.width / 2, origin.y + _visibleSize.height - 60);
    _scoreLabel->setColor(Color3B::WHITE);
    _scoreLabel->enableOutline(Color4B::BLACK, 2);
    this->addChild(_scoreLabel, 20);

    _readyLabel = Label::createWithTTF("Click to Start", "fonts/Marker Felt.ttf", 36);
    _readyLabel->setPosition(origin.x + _visibleSize.width / 2, origin.y + _visibleSize.height / 2 + 50);
    _readyLabel->setColor(Color3B::WHITE);
    _readyLabel->enableOutline(Color4B::BLACK, 2);
    this->addChild(_readyLabel, 20);

    _gameOverLabel = Label::createWithTTF("Game Over! Click to restart", "fonts/Marker Felt.ttf", 28);
    _gameOverLabel->setPosition(origin.x + _visibleSize.width / 2, origin.y + _visibleSize.height / 2 - 50);
    _gameOverLabel->setColor(Color3B::WHITE);
    _gameOverLabel->enableOutline(Color4B::BLACK, 2);
    _gameOverLabel->setVisible(false);
    this->addChild(_gameOverLabel, 20);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    this->scheduleUpdate();

    return true;
}

void GameScene::update(float dt)
{
    if (_gameState != GAME_STATE_PLAYING) {
        return;
    }

    _birdVelocity += _gravity * dt;
    Vec2 newPos = _bird->getPosition();
    newPos.y += _birdVelocity * dt;

    float maxY = _visibleSize.height - 20;
    float minY = _groundHeight + 20;

    if (newPos.y > maxY) {
        newPos.y = maxY;
        _birdVelocity = 0;
    }
    if (newPos.y < minY) {
        newPos.y = minY;
        gameOver();
        return;
    }

    _bird->setPosition(newPos);

    float rotation = -_birdVelocity * 0.05f;
    rotation = clampf(rotation, -45.0f, 45.0f);
    _bird->setRotation(rotation);

    for (auto& pipe : _pipeContainer->getChildren()) {
        Vec2 pipePos = pipe->getPosition();
        pipePos.x -= _pipeSpeed * dt;
        pipe->setPosition(pipePos);

        if (pipe->getName() == "top" && pipePos.x + PIPE_WIDTH < _bird->getPosition().x &&
            !pipe->getUserData()) {
            pipe->setUserData((void*)1);
            _score++;
            updateScore();
        }

        if (pipePos.x < -PIPE_WIDTH) {
            pipe->removeFromParent();
        }
    }

    _lastPipeSpawnTime += dt;
    if (_lastPipeSpawnTime >= _pipeSpawnInterval) {
        spawnPipe();
        _lastPipeSpawnTime = 0;
    }
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void GameScene::onTouchMoved(Touch* touch, Event* event) {}

void GameScene::onTouchEnded(Touch* touch, Event* event)
{
    if (_gameState == GAME_STATE_READY) {
        _gameState = GAME_STATE_PLAYING;
        _readyLabel->setVisible(false);
        _birdVelocity = _jumpForce;
        _lastPipeSpawnTime = _pipeSpawnInterval;
    } else if (_gameState == GAME_STATE_PLAYING) {
        _birdVelocity = _jumpForce;
    } else if (_gameState == GAME_STATE_OVER) {
        resetGame();
    }
}

bool GameScene::onContactBegin(PhysicsContact& contact)
{
    if (_gameState != GAME_STATE_PLAYING) {
        return false;
    }

    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    if ((bodyA->getCategoryBitmask() == 0x01 && bodyB->getCategoryBitmask() == 0x02) ||
        (bodyA->getCategoryBitmask() == 0x02 && bodyB->getCategoryBitmask() == 0x01)) {
        gameOver();
    }

    return true;
}

void GameScene::spawnPipe()
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float minHeight = 20.0f;
    float maxHeight = _visibleSize.height - _groundHeight - _pipeGap - minHeight;
    float bottomHeight = minHeight + (rand() % (int)(maxHeight - minHeight));

    auto bottomPipe = Sprite::create("down_bar.png");
    if (bottomPipe) {
        bottomPipe->setAnchorPoint(Vec2(0, 0));
        float scaleY = bottomHeight / bottomPipe->getContentSize().height;
        bottomPipe->setScale(1.0f, scaleY);
        bottomPipe->setPosition(origin.x + _visibleSize.width + 50, origin.y + _groundHeight);
        auto pipeBody = PhysicsBody::createBox(Size(PIPE_WIDTH, bottomHeight));
        pipeBody->setDynamic(false);
        pipeBody->setCategoryBitmask(0x02);
        pipeBody->setCollisionBitmask(0x01);
        pipeBody->setContactTestBitmask(0x01);
        bottomPipe->setPhysicsBody(pipeBody);
        bottomPipe->setName("bottom");
        _pipeContainer->addChild(bottomPipe);
    }

    auto topPipe = Sprite::create("up_bar.png");
    if (topPipe) {
        float topHeight = _visibleSize.height - _groundHeight - bottomHeight - _pipeGap;
        topPipe->setAnchorPoint(Vec2(0, 1));
        float scaleY = topHeight / topPipe->getContentSize().height;
        topPipe->setScale(1.0f, scaleY);
        topPipe->setPosition(origin.x + _visibleSize.width + 50, origin.y + _visibleSize.height);
        auto pipeBody = PhysicsBody::createBox(Size(PIPE_WIDTH, topHeight));
        pipeBody->setDynamic(false);
        pipeBody->setCategoryBitmask(0x02);
        pipeBody->setCollisionBitmask(0x01);
        pipeBody->setContactTestBitmask(0x01);
        topPipe->setPhysicsBody(pipeBody);
        topPipe->setName("top");
        topPipe->setUserData(nullptr);
        _pipeContainer->addChild(topPipe);
    }
}

void GameScene::resetGame()
{
    _gameState = GAME_STATE_READY;
    _score = 0;
    _birdVelocity = 0;
    _lastPipeSpawnTime = 0;

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    _bird->setPosition(origin.x + _visibleSize.width / 4, origin.y + _visibleSize.height / 2);
    _bird->setRotation(0);

    _pipeContainer->removeAllChildren();

    updateScore();
    _readyLabel->setVisible(true);
    _gameOverLabel->setVisible(false);
}

void GameScene::gameOver()
{
    _gameState = GAME_STATE_OVER;
    _gameOverLabel->setVisible(true);
}

void GameScene::updateScore()
{
    _scoreLabel->setString(StringUtils::format("%d", _score));
}
