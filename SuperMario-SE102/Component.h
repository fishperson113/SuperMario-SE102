#pragma once

#include"GameObject.h"
#include"Game.h"
#include"Collision.h"
#include"Animation.h"
#include"Animations.h"
#include"AssetIDs.h"
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

    void SetEnabled(bool enabled) { isEnabled = enabled; }
    bool IsEnabled() const { return isEnabled; }
protected:
    CGameObject* parentObject = nullptr;
    bool isEnabled = true;
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
    VelocityComponent() : velX(0), velY(0), speed(0.1f),
        useGravity(true), gravityAccel(0.002f),
        maxFallingSpeed(0.5f), isOnGround(false) {
    }
    VelocityComponent(float vx, float vy, float spd) : velX(vx), velY(vy), speed(spd),
        useGravity(false), gravityAccel(0.002f),
        maxFallingSpeed(0.5f), isOnGround(false) {
    }

    void SetVelocity(float vx, float vy) { velX = vx; velY = vy; }
    VECTOR2 GetVelocity() const { return VECTOR2(velX, velY); }
    void SetSpeed(float spd) { speed = spd; }
    float GetSpeed() const { return speed; }
    void Update(float dt) override;
	void Render() override {}

    void UpdatePosition(float dt);
    void MoveToPosition(float x, float y);

    void EnableGravity(bool enable);
    void SetGravityAcceleration(float accel);
    void SetMaxFallingSpeed(float maxSpeed);
    void SetGrounded(bool grounded);
    bool IsGrounded() const;
private:
    float velX, velY;
    float speed;

    bool useGravity;
    float gravityAccel;
    float maxFallingSpeed;
    bool isOnGround;
};
//Monobehaviour
class ScriptComponent : public Component
{
public:
    virtual void Update(float dt) = 0;
    virtual void Awake()=0;
    virtual void Start()=0;
	virtual void Render() = 0;

    // When no collision has been detected (triggered by CCollision::Process)
    virtual void OnNoCollision(DWORD dt) { 
        auto velocity = parentObject->GetComponent<VelocityComponent>();
		if (!velocity) return;
        velocity->UpdatePosition(dt);
    };

    // When collision with an object has been detected (triggered by CCollision::Process)
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
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
class ColliderComponent : public Component
{
public:
    ColliderComponent()
        : left(0), top(0), width(0), height(0), isBlocking(true),
        isCollidable(true),
        collidableFromTop(true), collidableFromBottom(true),
        collidableFromLeft(true), collidableFromRight(true),
        isActivatedRenderBoundingBox(true) {}
    ~ColliderComponent() override = default;

    void Update(float dt) override {};
    void Render() override;

public:
    void SetBoundingBox(float left, float top, float width, float height);
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) const;

    bool IsBlocking() const { return isBlocking; }
    void SetBlocking(bool blocking) { isBlocking = blocking; }

    bool IsCollidable() const { return isCollidable; }
    void SetCollidable(bool collidable) { isCollidable = collidable; }

    void SetCollidableDirections(bool top, bool bottom, bool left, bool right) {
        collidableFromTop = top;
        collidableFromBottom = bottom;
        collidableFromLeft = left;
        collidableFromRight = right;
    }

    void SetAsPlatform() {
        SetCollidableDirections(true, false, false, false);
    }

    void SetAsFullCollider() {
        SetCollidableDirections(true, true, true, true);
    }


    int IsDirectionColliable(float nx, float ny) const;

private:
    float left, top, width, height;
    bool isBlocking;
    bool isCollidable;

	bool isActivatedRenderBoundingBox;

    bool collidableFromTop;     // ny = -1
    bool collidableFromBottom;  // ny = 1
    bool collidableFromLeft;    // nx = -1
    bool collidableFromRight;   // nx = 1
};
