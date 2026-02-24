/* Copyright (c) 2012 Scott Lembcke and Howling Moon Software
 * Copyright (c) 2012 cocos2d-x.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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

public:
    B2Sprite();

    static B2Sprite* create();
    static B2Sprite* createWithTexture(cocos2d::Texture2D* pTexture);
    static B2Sprite* createWithTexture(cocos2d::Texture2D* pTexture, const cocos2d::Rect& rect);
    static B2Sprite* createWithSpriteFrame(cocos2d::SpriteFrame* pSpriteFrame);
    static B2Sprite* createWithSpriteFrameName(const char* pszSpriteFrameName);
    static B2Sprite* create(const char* pszFileName);
    static B2Sprite* create(const char* pszFileName, const cocos2d::Rect& rect);

    virtual bool isDirty();

    bool isIgnoreBodyRotation() const;
    void setIgnoreBodyRotation(bool bIgnoreBodyRotation);

    virtual const cocos2d::Vec2& getPosition();
    virtual void getPosition(float* x, float* y);
    virtual float getPositionX();
    virtual float getPositionY();
    virtual void setPosition(const cocos2d::Vec2& position);
    virtual float getRotation();
    virtual void setRotation(float fRotation);
    virtual cocos2d::AffineTransform nodeToParentTransform() const;

    b2Body* getB2Body() const;
    void setB2Body(b2Body* pBody);

    float getPTMRatio() const;
    void setPTMRatio(float fRatio);

protected:
    void updatePosFromPhysics();
};

#endif
