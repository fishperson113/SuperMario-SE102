#include "HitBox.h"
#include "Mario.h"
#include "ParaGoomba.h"
#include "Goomba.h"
#include "Koopas.h"
#include "KoopaParatroopa.h"
#include "Bullet.h"
#include "Brick.h"
#include "PiranhaPlant.h"
#include "Utils.h"
#include "CEffect.h"
#include "CEffectScore.h"
#include "SwitchBrick.h"
#include "CoinBrick.h"
#include "MushroomBrick.h"
#include "SuperLeafBrick.h"
#include"EffectHit.h"

HitBox::HitBox(CGameObject* owner):CGameObject(x,y)
{
    this->owner = owner;
    this->isActivate = false;
    this->activate_start = 0;
}

void HitBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isActivate) return;

    if (GetTickCount64() - activate_start > HITBOX_LIFESPAN)
    {
        isActivate = false;
    }
    if (isActivate)
    {

        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
        if (scene)
        {
            const std::vector<LPGAMEOBJECT>& objects = scene->GetObjectManager()->GetObjects();
            CheckOverlaps(const_cast<std::vector<LPGAMEOBJECT>*>(&objects));
        }
    }
}
void HitBox::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
    CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
    if (goomba->GetState() != GOOMBA_STATE_DIE)
        goomba->SetState(GOOMBA_STATE_DIE);
    mario->SpawnHitEffect(e, this, EFF_COL_TYPE_NORMAL);
    DebugOut(L">>> Hitbox hit Goomba! >>> \n");
}

void HitBox::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
    CParaGoomba* paraGoomba = dynamic_cast<CParaGoomba*>(e->obj);
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
    if (paraGoomba->GetState() != PARAGOOMBA_STATE_DIE)
        paraGoomba->SetState(PARAGOOMBA_STATE_DIE);
    mario->SpawnHitEffect(e, this, EFF_COL_TYPE_NORMAL);
}

void HitBox::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
    Koopas* koopas = dynamic_cast<Koopas*>(e->obj);
    if (!koopas) return;

    // Get Mario to spawn effect and get direction
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
    float kickDirection = mario ? mario->GetDirection() : 1.0f;

    // Handle based on Koopas type
    if (koopas->GetType() == KOOPAS_RED)
    {
        // Turn Koopas upside down (use the proper state constant that exists in your code)
        koopas->SetState(KOOPAS_STATE_UPSIDE_DOWN);

        // Kick it away
        koopas->SetSpeed(kickDirection * KOOPAS_SHELL_SPEED, 0);

        // Spawn hit effect
        mario->SpawnHitEffect(e, this, EFF_COL_TYPE_NORMAL);

        DebugOut(L">>> Hitbox turned Red Koopa upside down and kicked it! >>> \n");
    }
    // Handle Green Koopas
    else if (koopas->GetType() == KOOPAS_GREEN || koopas->GetType() == KOOPAS_GREEN_NO_WINGS)
    {
        if (koopas->GetType() == KOOPAS_GREEN)
            koopas->SetType(KOOPAS_GREEN_NO_WINGS);

        koopas->SetState(KOOPAS_STATE_UPSIDE_DOWN);

        // Kick it away
        koopas->SetSpeed(kickDirection * KOOPAS_SHELL_SPEED, 0);

        // Spawn hit effect
        mario->SpawnHitEffect(e, this, EFF_COL_TYPE_NORMAL);
    }
}

void HitBox::OnCollisionWithKoopaParatroopa(LPCOLLISIONEVENT e)
{
    CKoopaParatroopa* koopaParatroopa = dynamic_cast<CKoopaParatroopa*>(e->obj);
    if (!koopaParatroopa) return;

    // Get Mario's direction to determine shell direction
    CMario* mario = dynamic_cast<CMario*>(owner);
    float shellDirection = mario ? mario->GetDirection() : 1.0f;

    if (koopaParatroopa->GetState() == KOOPA_PARATROOPA_STATE_WALKING_WINGS ||
        koopaParatroopa->GetState() == KOOPA_PARATROOPA_STATE_JUMPING_WINGS)
    {
        // Remove wings
        koopaParatroopa->SetState(KOOPA_PARATROOPA_STATE_WALKING);
        DebugOut(L">>> Hitbox removed Koopa Paratroopa wings! >>> \n");
    }
    else if (koopaParatroopa->GetState() == KOOPA_PARATROOPA_STATE_WALKING)
    {
        // Turn to shell
        koopaParatroopa->SetState(KOOPA_PARATROOPA_STATE_SHELL);
        DebugOut(L">>> Hitbox turned Koopa Paratroopa into shell! >>> \n");
    }
    else if (koopaParatroopa->GetState() == KOOPA_PARATROOPA_STATE_SHELL ||
        koopaParatroopa->GetState() == KOOPA_PARATROOPA_STATE_SHELL_FEET)
    {
        // Kick shell
        koopaParatroopa->SetState(KOOPA_PARATROOPA_STATE_MOVING_SHELL);
        koopaParatroopa->SetSpeed(shellDirection * KOOPAS_SHELL_SPEED, 0);
        DebugOut(L">>> Hitbox kicked Koopa Paratroopa shell! >>> \n");
    }
    else if (koopaParatroopa->GetState() == KOOPA_PARATROOPA_STATE_MOVING_SHELL)
    {
        // Reverse direction
        float vx, vy;
        koopaParatroopa->GetSpeed(vx, vy);
        koopaParatroopa->SetSpeed(-vx, vy);
        DebugOut(L">>> Hitbox reversed Koopa Paratroopa shell direction! >>> \n");
    }
}

void HitBox::OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e)
{
    CPiranhaPlant* plant = dynamic_cast<CPiranhaPlant*>(e->obj);
    plant->Delete();
}

void HitBox::OnCollisionWithBullet(LPCOLLISIONEVENT e)
{
    CBullet* bullet = dynamic_cast<CBullet*>(e->obj);
    bullet->Delete();
}

void HitBox::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
    // Only break bricks that are breakable
    CBrick* brick = dynamic_cast<CBrick*>(e->obj);
    if (brick->IsBreakable())
        brick->Break();
}

void HitBox::OnCollisionWithSwitchBrick(LPCOLLISIONEVENT e)
{
	CSwitchBrick* switchBrick = dynamic_cast<CSwitchBrick*>(e->obj);
    if (switchBrick && switchBrick->GetState() != SWITCH_BRICK_STATE_HIT)
    {
        switchBrick->SetState(SWITCH_BRICK_STATE_HIT);
        switchBrick->SpawnSwitch();
        DebugOut(L">>> Mario hit a switch brick! >>> \n");
    }
    else
    {
        DebugOut(L">>> Mario tried to hit a switch brick that is already activated! >>> \n");
    }
}

void HitBox::OnCollisionWithCoinBrick(LPCOLLISIONEVENT e)
{
	CCoinBrick* coinBrick = dynamic_cast<CCoinBrick*>(e->obj);
	if (coinBrick && coinBrick->GetState() != BRICK_STATE_HIT)
	{
		coinBrick->SetState(BRICK_STATE_HIT);
		coinBrick->SpawnCoin();
		DebugOut(L">>> Mario hit a coin brick! >>> \n");
	}
	else
	{
		DebugOut(L">>> Mario tried to hit a coin brick that is already activated! >>> \n");
	}
}

void HitBox::OnCollisionWithMushroomBrick(LPCOLLISIONEVENT e)
{
	CMushroomBrick* mushroomBrick = dynamic_cast<CMushroomBrick*>(e->obj);
	if (mushroomBrick && mushroomBrick->GetState() != BRICK_STATE_HIT)
	{
        mushroomBrick->SpawnMushroom();
		mushroomBrick->SetState(BRICK_STATE_HIT);
		DebugOut(L">>> Mario hit a mushroom brick! >>> \n");
	}
	else
	{
		DebugOut(L">>> Mario tried to hit a mushroom brick that is already activated! >>> \n");
	}
}
void HitBox::Render()
{
    //if (!isActivate) return;
    RenderBoundingBox();
}

void HitBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - HITBOX_WIDTH / 2;
    top = y - HITBOX_HEIGHT / 2;
    right = left + HITBOX_WIDTH;
    bottom = top + HITBOX_HEIGHT;
}

void HitBox::Activate()
{
    isActivate = true;
    activate_start = GetTickCount64();
}
void HitBox::OnNoCollision(DWORD dt)
{
    float mario_x, mario_y;
    CMario* mario = dynamic_cast<CMario*>(owner);    
    mario->GetPosition(mario_x, mario_y);

    float offsetX = mario->GetDirection() > 0 ? HITBOX_WIDTH / 2 : -HITBOX_WIDTH / 2;
    float offsetY = (mario->GetLevel() == MARIO_LEVEL_SMALL) ? 0 : 5;
    x = mario_x + offsetX;
    y = mario_y+offsetY;
}
void HitBox::CheckOverlaps(vector<LPGAMEOBJECT>* coObjects)
{

    if (!isActivate || !coObjects) return;

    float hl, ht, hr, hb;
    GetBoundingBox(hl, ht, hr, hb);

    for (size_t i = 0; i < coObjects->size(); i++)
    {
        LPGAMEOBJECT obj = coObjects->at(i);

        // Skip self, owner, and deleted objects
        if (obj == this || obj == owner || obj->IsDeleted() || !obj->IsActive())
            continue;

        // Get object bounds
        float ol, ot, or_, ob;
        obj->GetBoundingBox(ol, ot, or_, ob);

        // Check if there's an overlap
        if (IsOverlap(hl, ht, hr, hb, ol, ot, or_, ob))
        {
            // Create a collision event
            CCollisionEvent* e = new CCollisionEvent(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, obj, this);

            // Process the collision
            OnCollisionWith(e);

            // Clean up
            delete e;
        }
    }
}
void HitBox::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!isActivate) return;
    // Handle collision with enemies
    if (dynamic_cast<CGoomba*>(e->obj))
        OnCollisionWithGoomba(e);
    else if (dynamic_cast<CParaGoomba*>(e->obj))
        OnCollisionWithParaGoomba(e);
    else if (dynamic_cast<Koopas*>(e->obj))
        OnCollisionWithKoopas(e);
    else if (dynamic_cast<CKoopaParatroopa*>(e->obj))
        OnCollisionWithKoopaParatroopa(e);
    else if (dynamic_cast<CPiranhaPlant*>(e->obj))
        OnCollisionWithPiranhaPlant(e);
    else if (dynamic_cast<CBullet*>(e->obj))
        OnCollisionWithBullet(e);
    else if (dynamic_cast<CBrick*>(e->obj))
        OnCollisionWithBrick(e);
	else if (dynamic_cast<CSwitchBrick*>(e->obj))
		OnCollisionWithSwitchBrick(e);
    else if (dynamic_cast<CCoinBrick*>(e->obj))
        OnCollisionWithCoinBrick(e);
    else if (dynamic_cast<CMushroomBrick*>(e->obj))
        OnCollisionWithMushroomBrick(e);
}