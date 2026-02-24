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

#include "B2Sprite.h"

#include "Box2D/Box2D.h"

USING_NS_CC;

B2Sprite::B2Sprite() : m_bIgnoreBodyRotation(false), m_pB2Body(nullptr), m_fPTMRatio(0.0f) {}

B2Sprite* B2Sprite::create()
{
    B2Sprite* pRet = new B2Sprite();
    if (pRet && pRet->init()) {
        pRet->autorelease();
    } else {
        delete pRet;
        pRet = nullptr;
    }

    return pRet;
}

B2Sprite* B2Sprite::createWithTexture(Texture2D* pTexture)
{
    B2Sprite* pRet = new B2Sprite();
    if (pRet && pRet->initWithTexture(pTexture)) {
        pRet->autorelease();
    } else {
        delete pRet;
        pRet = nullptr;
    }

    return pRet;
}

B2Sprite* B2Sprite::createWithTexture(Texture2D* pTexture, const Rect& rect)
{
    B2Sprite* pRet = new B2Sprite();
    if (pRet && pRet->initWithTexture(pTexture, rect)) {
        pRet->autorelease();
    } else {
        delete pRet;
        pRet = nullptr;
    }

    return pRet;
}

B2Sprite* B2Sprite::createWithSpriteFrame(SpriteFrame* pSpriteFrame)
{
    B2Sprite* pRet = new B2Sprite();
    if (pRet && pRet->initWithSpriteFrame(pSpriteFrame)) {
        pRet->autorelease();
    } else {
        delete pRet;
        pRet = nullptr;
    }

    return pRet;
}

B2Sprite* B2Sprite::createWithSpriteFrameName(const char* pszSpriteFrameName)
{
    B2Sprite* pRet = new B2Sprite();
    if (pRet && pRet->initWithSpriteFrameName(pszSpriteFrameName)) {
        pRet->autorelease();
    } else {
        delete pRet;
        pRet = nullptr;
    }

    return pRet;
}

B2Sprite* B2Sprite::create(const char* pszFileName)
{
    B2Sprite* pRet = new B2Sprite();
    if (pRet && pRet->initWithFile(pszFileName)) {
        pRet->autorelease();
    } else {
        delete pRet;
        pRet = nullptr;
    }

    return pRet;
}

B2Sprite* B2Sprite::create(const char* pszFileName, const Rect& rect)
{
    B2Sprite* pRet = new B2Sprite();
    if (pRet && pRet->initWithFile(pszFileName, rect)) {
        pRet->autorelease();
    } else {
        delete pRet;
        pRet = nullptr;
    }

    return pRet;
}

bool B2Sprite::isDirty()
{
    return true;
}

bool B2Sprite::isIgnoreBodyRotation() const
{
    return m_bIgnoreBodyRotation;
}

void B2Sprite::setIgnoreBodyRotation(bool bIgnoreBodyRotation)
{
    m_bIgnoreBodyRotation = bIgnoreBodyRotation;
}

const Vec2& B2Sprite::getPosition()
{
    updatePosFromPhysics();
    return Node::getPosition();
}

void B2Sprite::getPosition(float* x, float* y)
{
    updatePosFromPhysics();
    Node::getPosition(x, y);
}

float B2Sprite::getPositionX()
{
    updatePosFromPhysics();
    return _position.x;
}

float B2Sprite::getPositionY()
{
    updatePosFromPhysics();
    return _position.y;
}

b2Body* B2Sprite::getB2Body() const
{
    return m_pB2Body;
}

void B2Sprite::setB2Body(b2Body* pBody)
{
    m_pB2Body = pBody;
    pBody->SetUserData(this);
}

float B2Sprite::getPTMRatio() const
{
    return m_fPTMRatio;
}

void B2Sprite::setPTMRatio(float fRatio)
{
    m_fPTMRatio = fRatio;
}

void B2Sprite::updatePosFromPhysics()
{
    b2Vec2 pos = m_pB2Body->GetPosition();
    float x = pos.x * m_fPTMRatio;
    float y = pos.y * m_fPTMRatio;
    _position = Vec2(x, y);
}

void B2Sprite::setPosition(const Vec2& pos)
{
    float angle = m_pB2Body->GetAngle();
    m_pB2Body->SetTransform(b2Vec2(pos.x / m_fPTMRatio, pos.y / m_fPTMRatio), angle);
}

float B2Sprite::getRotation()
{
    return (m_bIgnoreBodyRotation ? Sprite::getRotation() : CC_RADIANS_TO_DEGREES(m_pB2Body->GetAngle()));
}

void B2Sprite::setRotation(float fRotation)
{
    if (m_bIgnoreBodyRotation) {
        Sprite::setRotation(fRotation);
    } else {
        b2Vec2 p = m_pB2Body->GetPosition();
        float radians = CC_DEGREES_TO_RADIANS(fRotation);
        m_pB2Body->SetTransform(p, radians);
    }
}

AffineTransform B2Sprite::nodeToParentTransform() const
{
    b2Vec2 pos = m_pB2Body->GetPosition();

    float x = pos.x * m_fPTMRatio;
    float y = pos.y * m_fPTMRatio;

    if (_ignoreAnchorPointForPosition) {
        x += _anchorPointInPoints.x;
        y += _anchorPointInPoints.y;
    }

    float radians = m_pB2Body->GetAngle();
    float c = cosf(radians);
    float s = sinf(radians);

    if (!_anchorPointInPoints.equals(Vec2::ZERO)) {
        x += ((c * -_anchorPointInPoints.x * _scaleX) + (-s * -_anchorPointInPoints.y * _scaleY));
        y += ((s * -_anchorPointInPoints.x * _scaleX) + (c * -_anchorPointInPoints.y * _scaleY));
    }

    return AffineTransformMake(c * _scaleX, s * _scaleX, -s * _scaleY, c * _scaleY, x, y);
}
