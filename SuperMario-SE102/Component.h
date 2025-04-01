#pragma once

#include"GameObject.h"
#include"Game.h"

class VECTOR2
{
public:
	VECTOR2() : x(0), y(0) {}
	VECTOR2(float x, float y) : x(x), y(y) {}
	float x, y;
};
class Component
{
public:
    virtual ~Component() = default;
    void SetParentObject(CGameObject* parent) { parentObject = parent; }
    CGameObject* GetParentObject() const { return parentObject; }

	virtual void Update(float dt) = 0;
    virtual void Render() = 0;
protected:
    CGameObject* parentObject = nullptr;
};

class TransformComponent : public Component
{
public:
    TransformComponent():mRotation(0),posX(0),posY(0) {};
    TransformComponent(const TransformComponent&) = default;
	void SetPosition(float x, float y) { posX = x; posY = y; }
	void SetRotation(float rotation) { mRotation = rotation; }
	float GetRotation() const { return mRotation; }
	float GetPositionX() const { return posX; }
	float GetPositionY() const { return posY; }

	void Update(float dt) override {}
	void Render() override {}
private:
    float mRotation;
    float posX, posY;
};

class VelocityComponent : public Component
{
public:
    VelocityComponent() : velX(0), velY(0), speed(1.0f) {}
    VelocityComponent(float vx, float vy, float spd) : velX(vx), velY(vy), speed(spd) {}

    void SetVelocity(float vx, float vy) { velX = vx; velY = vy; }
    VECTOR2 GetVelocity() const { return VECTOR2(velX, velY); }
    void SetSpeed(float spd) { speed = spd; }
    float GetSpeed() const { return speed; }
    void Update(float dt) override;
	void Render() override {}
private:
    float velX, velY;
    float speed;
};
//Monobehaviour
class ScriptComponent : public Component
{
public:
    virtual void Update(float dt) = 0;
    virtual void Awake()=0;
    virtual void Start()=0;
	virtual void Render() = 0;
};

class AnimationComponent : public Component
{
public:
	AnimationComponent() : currentAnimation(nullptr) {}
	void SetCurrentAnimation(LPANIMATION ani) { currentAnimation = ani; }
	LPANIMATION GetCurrentAnimation() const { return currentAnimation; }
    void Update(float dt) override {};
	void Render() override;
private:
	LPANIMATION currentAnimation;
};

class SpriteComponent : public Component
{
public:
    SpriteComponent() : sprite(nullptr) {}

    void SetSprite(LPSPRITE newSprite) {
        sprite = newSprite;
    }

    LPSPRITE GetSprite() const {
        return sprite;
    }

    void Update(float dt) override {}

    void Render() override{
        auto transform = GetParentObject()->GetComponent<TransformComponent>();
        if (!transform || !sprite) return;

        sprite->Draw(transform->GetPositionX(), transform->GetPositionY());
    }

private:
    LPSPRITE sprite;
};
