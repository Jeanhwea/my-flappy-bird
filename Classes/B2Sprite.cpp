#include "B2Sprite.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

B2Sprite::B2Sprite()
    : m_bIgnoreBodyRotation(false)
    , m_pB2Body(nullptr)
    , m_fPTMRatio(0.0f)
    , m_bPositionDirty(true)
{}

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

bool B2Sprite::isIgnoreBodyRotation() const
{
    return m_bIgnoreBodyRotation;
}

void B2Sprite::setIgnoreBodyRotation(bool bIgnoreBodyRotation)
{
    m_bIgnoreBodyRotation = bIgnoreBodyRotation;
}

void B2Sprite::updatePosFromPhysics() const
{
    if (m_pB2Body && m_fPTMRatio > 0) {
        b2Vec2 pos = m_pB2Body->GetPosition();
        float x = pos.x * m_fPTMRatio;
        float y = pos.y * m_fPTMRatio;
        const_cast<B2Sprite*>(this)->Sprite::setPosition(Vec2(x, y));
    }
}

const Vec2& B2Sprite::getPosition() const
{
    if (m_pB2Body) {
        updatePosFromPhysics();
    }
    return Sprite::getPosition();
}

void B2Sprite::setPosition(const Vec2& pos)
{
    if (m_pB2Body) {
        float angle = m_pB2Body->GetAngle();
        m_pB2Body->SetTransform(b2Vec2(pos.x / m_fPTMRatio, pos.y / m_fPTMRatio), angle);
    }
    Sprite::setPosition(pos);
}

float B2Sprite::getRotation() const
{
    if (m_pB2Body && !m_bIgnoreBodyRotation) {
        return CC_RADIANS_TO_DEGREES(m_pB2Body->GetAngle());
    }
    return Sprite::getRotation();
}

void B2Sprite::setRotation(float fRotation)
{
    if (m_pB2Body && !m_bIgnoreBodyRotation) {
        b2Vec2 p = m_pB2Body->GetPosition();
        float radians = CC_DEGREES_TO_RADIANS(fRotation);
        m_pB2Body->SetTransform(p, radians);
    } else {
        Sprite::setRotation(fRotation);
    }
}

AffineTransform B2Sprite::nodeToParentTransform() const
{
    if (!m_pB2Body) {
        return Sprite::nodeToParentTransform();
    }

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

b2Body* B2Sprite::getB2Body() const
{
    return m_pB2Body;
}

void B2Sprite::setB2Body(b2Body* pBody)
{
    m_pB2Body = pBody;
    if (pBody) {
        pBody->SetUserData(this);
    }
}

float B2Sprite::getPTMRatio() const
{
    return m_fPTMRatio;
}

void B2Sprite::setPTMRatio(float fRatio)
{
    m_fPTMRatio = fRatio;
}
