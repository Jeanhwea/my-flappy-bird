#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

// Game State
typedef enum {
    GAME_STATE_READY = 0,  // Ready
    GAME_STATE_PLAYING,    // Playing
    GAME_STATE_OVER        // Game Over
} GameState;

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameScene);

    void update(float dt);

    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    bool onContactBegin(cocos2d::PhysicsContact& contact);

private:
    void spawnPipe();
    void resetGame();
    void gameOver();
    void updateScore();

    GameState _gameState;
    cocos2d::Sprite* _bird;
    cocos2d::Sprite* _ground;
    cocos2d::Node* _pipeContainer;
    cocos2d::Label* _scoreLabel;
    cocos2d::Label* _readyLabel;
    cocos2d::Label* _gameOverLabel;

    float _birdVelocity;
    float _gravity;
    float _jumpForce;
    float _pipeSpeed;
    float _pipeGap;
    float _pipeSpawnInterval;
    float _lastPipeSpawnTime;

    int _score;
    float _groundHeight;
    cocos2d::Size _visibleSize;
};

#endif
