#include "Collision.h"
#include "GameObject.h"
#include "Component.h"
#include "debug.h"
#include "GameObject.h"

int CCollisionEvent::WasCollided() {
	auto collider = obj->GetComponent<ColliderComponent>();
	if (!collider) return 0;

	return t >= 0.0f && t <= 1.0f && collider->IsDirectionColliable(nx, ny) == 1;
}

#define BLOCK_PUSH_FACTOR 0.01f

CCollision* CCollision::__instance = NULL;

CCollision* CCollision::GetInstance()
{
	if (__instance == NULL) __instance = new CCollision();
	return __instance;
}

/*
	SweptAABB 
*/
void CCollision::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -9999999.0f;
		tx_exit = 99999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999999999.0f;
		ty_exit = 99999999999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}

}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CCollision::SweptAABB(LPGAMEOBJECT objSrc, DWORD dt, LPGAMEOBJECT objDest)
{
	if (objSrc == objDest) return new CCollisionEvent(-1.0f, 0, 0, 0, 0, objDest, objSrc);

	float sl=0, st=0, sr=0, sb=0;		// static object bbox
	float ml=0, mt=0, mr=0, mb=0;		// moving object bbox
	float t = 1.0f, nx = 0, ny = 0;

	float mvx=0, mvy=0;
	auto velocitySrc = objSrc->GetComponent<VelocityComponent>();
	if (!velocitySrc) return new CCollisionEvent(-1.0f, 0, 0, 0, 0, objDest, objSrc);
	mvx = velocitySrc->GetVelocity().x * velocitySrc->GetSpeed();
	mvy = velocitySrc->GetVelocity().y * velocitySrc->GetSpeed();

	float dtSeconds = dt / 1000.0f;
	float mdx = mvx * dtSeconds;
	float mdy = mvy * dtSeconds;

	float svx = 0, svy = 0;
	auto velocityDest = objDest->GetComponent<VelocityComponent>();
	if (velocityDest) {
		svx = velocityDest->GetVelocity().x * velocityDest->GetSpeed();
		svy = velocityDest->GetVelocity().y * velocityDest->GetSpeed();
	}
	float sdx = svx * dtSeconds;
	float sdy = svy * dtSeconds;

	//
	// NOTE: new m speed = original m speed - collide object speed
	// 

	float dx = mdx - sdx;
	float dy = mdy - sdy;

	if (dx == 0 && dy == 0)
		return new CCollisionEvent(-1.0f, 0, 0, 0, 0, objDest, objSrc);

	auto srcCollider = objSrc->GetComponent<ColliderComponent>();
	auto destCollider = objDest->GetComponent<ColliderComponent>();

	if (!srcCollider || !destCollider)
		return new CCollisionEvent(-1.0f, 0, 0, 0, 0, objDest, objSrc);

	srcCollider->GetBoundingBox(ml, mt, mr, mb);
	destCollider->GetBoundingBox(sl, st, sr, sb);

	SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);
	if (t >= 0.0f && t <= 1.0f) {
		DebugOut(L"Collision detected! t=%.4f, nx=%.1f, ny=%.1f\n", t, nx, ny);
	}
	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, dx, dy, objDest, objSrc);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CCollision::Scan(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* objDests, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < objDests->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABB(objSrc, dt, objDests->at(i));

		if (e->WasCollided()==1)
			coEvents.push_back(e);
		else
			delete e;
	}

	//std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CCollision::Filter( LPGAMEOBJECT objSrc,
	vector<LPCOLLISIONEVENT>& coEvents,
	LPCOLLISIONEVENT &colX,
	LPCOLLISIONEVENT &colY,
	int filterBlock = 1,		// 1 = only filter block collisions, 0 = filter all collisions 
	int filterX = 1,			// 1 = process events on X-axis, 0 = skip events on X 
	int filterY = 1)			// 1 = process events on Y-axis, 0 = skip events on Y
{
	float min_tx, min_ty;

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];
		if (c->isDeleted) continue;
		if (!c->obj->IsActive()) continue;

		auto collider = c->obj->GetComponent<ColliderComponent>();
		// ignore collision event with object having IsBlocking = 0 (like coin, mushroom, etc)
		if (filterBlock == 1 && !collider->IsBlocking())
		{
			continue;
		}

		if (c->t < min_tx && c->nx != 0 && filterX == 1) {
			min_tx = c->t; min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0 && filterY == 1) {
			min_ty = c->t; min_iy = i;
		}
	}

	if (min_ix >= 0) colX = coEvents[min_ix];
	if (min_iy >= 0) colY = coEvents[min_iy];
}

/*
*  Simple/Sample collision framework 
*  NOTE: Student might need to improve this based on game logic 
*/

//TODO: Object collision doesnt follow the correct logic of velocity speed
void CCollision::Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	LPCOLLISIONEVENT colX = NULL; 
	LPCOLLISIONEVENT colY = NULL;

	coEvents.clear();

	auto srcCollider = objSrc->GetComponent<ColliderComponent>();
	auto srcTransform = objSrc->GetComponent<TransformComponent>();
	auto srcVelocity = objSrc->GetComponent<VelocityComponent>();
	auto scriptComp = objSrc->GetComponent<ScriptComponent>();

	if (!srcTransform || !srcVelocity) return;

	float x = srcTransform->GetPositionX();
	float y = srcTransform->GetPositionY();

	float vx = srcVelocity->GetVelocity().x;
	float vy = srcVelocity->GetVelocity().y;
	float speed = srcVelocity->GetSpeed();
	float dtSeconds = dt / 1000.0f;
	float dx = vx * speed * dtSeconds;
	float dy = vy * speed * dtSeconds;

	float initialX = x;
	float initialY = y;

	if (srcCollider && srcCollider->IsCollidable())
	{
		Scan(objSrc, dt, coObjects, coEvents);
	}

	// No collision detected
	if (coEvents.size() == 0)
	{

		if (scriptComp) {
			scriptComp->OnNoCollision(dt);
		}
		// By default, velocity will handle the movement
		return;
	}
	else
	{
		Filter(objSrc, coEvents, colX, colY);


		if (colX != NULL && colY != NULL) 
		{
			if (colY->t < colX->t)	// was collision on Y first ?
			{
				y = initialY + colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;

				if (scriptComp) {
					scriptComp->OnCollisionWith(colY);
				}

				//
				// see if after correction on Y, is there still a collision on X ? 
				//
				LPCOLLISIONEVENT colX_other = NULL;

				//
				// check again if there is true collision on X 
				//
				colX->isDeleted = true;		// remove current collision event on X

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABB(objSrc, dt, colX->obj));

				// re-filter on X only
				Filter(objSrc, coEvents, colX_other, colY, /*filterBlock = */ 1, 1, /*filterY=*/0);

				if (colX_other != NULL)
				{
					x = initialX + colX_other->t * dx + colX_other->nx * BLOCK_PUSH_FACTOR;
					if (scriptComp) {
						scriptComp->OnCollisionWith(colX_other);
					}
				}
				else
				{
					x = initialX + dx;
				}
			}
			else // collision on X first
			{
				x = initialX + colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;

				if (scriptComp) {
					scriptComp->OnCollisionWith(colX);
				}

				//
				// see if after correction on X, is there still a collision on Y ? 
				//
				LPCOLLISIONEVENT colY_other = NULL;

				//
				// check again if there is true collision on Y
				//
				colY->isDeleted = true;		// remove current collision event on Y

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABB(objSrc, dt, colY->obj));

				// re-filter on Y only
				Filter(objSrc, coEvents, colX, colY_other, /*filterBlock = */ 1, /*filterX=*/0, /*filterY=*/1);

				if (colY_other != NULL)
				{
					y = initialY + colY_other->t * dy + colY_other->ny * BLOCK_PUSH_FACTOR;

					if (scriptComp) {
						scriptComp->OnCollisionWith(colY_other);
					}
				}
				else
				{
					y = initialY + dy;
				}
			}
		}
		else
		if (colX != NULL)
		{
			x = initialX + colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
			y = initialY + dy;

			if (scriptComp) {
				scriptComp->OnCollisionWith(colX);
			}
		}
		else 
			if (colY != NULL)
			{
				x = initialX + dx;
				y = initialY + colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;

				if (scriptComp) {
					scriptComp->OnCollisionWith(colY);
				};
			}
			else // both colX & colY are NULL 
			{
				x = initialX + dx;
				y = initialY + dy;
			}

			srcVelocity->MoveToPosition(x, y);
	}

	//
	// Scan all non-blocking collisions for further collision logic
	//
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents[i];
		if (e->isDeleted) continue;
		auto collider = e->obj->GetComponent<ColliderComponent>();
		if (collider && collider->IsBlocking()) continue;  // blocking collisions were handled already, skip them

		scriptComp->OnCollisionWith(e);
	}


	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
