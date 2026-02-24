#ifndef __PHYSICSNODES_CCPHYSICSSPRITE_H__
#define __PHYSICSNODES_CCPHYSICSSPRITE_H__

#include "cocos2d.h"

class b2Body;

class B2Sprite : public cocos2d::Sprite
{
protected:
    bool m_bIgnoreBodyRotation;
    b2Body* m_pB2Body;
    float m_fPTMRatio;
    mutable bool m_bPositionDirty;

public:
    B2Sprite();

    static B2Sprite* create();
    static B2Sprite* createWithTexture(cocos2d::Texture2D* pTexture);
    static B2Sprite* createWithTexture(cocos2d::Texture2D* pTexture, const cocos2d::Rect& rect);
    static B2Sprite* createWithSpriteFrame(cocos2d::SpriteFrame* pSpriteFrame);
    static B2Sprite* createWithSpriteFrameName(const char* pszSpriteFrameName);
    static B2Sprite* create(const char* pszFileName);
    static B2Sprite* create(const char* pszFileName, const cocos2d::Rect& rect);

    bool isIgnoreBodyRotation() const;
    void setIgnoreBodyRotation(bool bIgnoreBodyRotation);

    virtual const cocos2d::Vec2& getPosition() const;
    virtual void setPosition(const cocos2d::Vec2& position);
    virtual float getRotation() const;
    virtual void setRotation(float fRotation);
    virtual cocos2d::AffineTransform nodeToParentTransform() const;

    b2Body* getB2Body() const;
    void setB2Body(b2Body* pBody);

    float getPTMRatio() const;
    void setPTMRatio(float fRatio);

protected:
    void updatePosFromPhysics() const;
};

#endif
