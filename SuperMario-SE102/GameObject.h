#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Collision.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define BBOX_ALPHA 0.25f		// Bounding box transparency
class HitBox;
class CGameObject
{
protected:

	float x; 
	float y;

	float vx;
	float vy;

	int nx;	 

	int state;
	HitBox* spinHitbox;
	bool isDeleted; 
	bool isActive;
	vector<LPGAMEOBJECT>* coObjects;

public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	int GetDirection() { return nx; }
	int GetState() { return this->state; }
	virtual void Delete() { isDeleted = true;  }
	bool IsDeleted() { return isDeleted; }

	virtual void RenderBoundingBox();

	CGameObject();
	CGameObject(float x, float y) :CGameObject() { this->x = x; this->y = y; isActive = true;}


	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	bool IsActive() const { return isActive; }
	void SetActive(bool active) { isActive = active; }
	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) { if (!isActive) return; };
	virtual void OnCollisionExit(LPGAMEOBJECT obj) {};
	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};
	virtual void OnCollisionStay(LPCOLLISIONEVENT e) {};
	virtual void OnOverlapWith(LPGAMEOBJECT obj) {}

	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }

	//Is this object moving ?
	virtual int IsDynamic() { return 0; }
	void SetCoObjects(vector<LPGAMEOBJECT>* coObjects) { this->coObjects = coObjects; };
	// Does this object collide with other object at certain direction ( like ColorBox )
	virtual int IsDirectionColliable(float nx, float ny) { return 1; }
	void SetHitbox(HitBox* hitBox) { spinHitbox = hitBox; };
	virtual ~CGameObject();

	static bool IsDeleted(const LPGAMEOBJECT &o) { return o->isDeleted; }
};
