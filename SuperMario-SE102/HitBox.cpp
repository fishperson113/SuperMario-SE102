#include "HitBox.h"
#include "Mario.h"
#include"ParaGoomba.h"
#include"Goomba.h"
#include"Koopas.h"
#include"KoopaParatroopa.h"
#include"Bullet.h"
#include"Brick.h"
#include"PiranhaPlant.h"
HitBox::HitBox(CGameObject* owner):CGameObject(x,y)
{
    this->owner = owner;
    this->isActive = false;
    this->activate_start = 0;
}

void HitBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CMario* mario = dynamic_cast<CMario*>(owner);
    if (mario)
    {
        // Place hitbox in front of Mario based on his direction
        float mario_x, mario_y;
        mario->GetPosition(mario_x, mario_y);

        float offsetX = mario->GetDirection() > 0 ? HITBOX_WIDTH / 2 : -HITBOX_WIDTH / 2;
        x = mario_x + offsetX;
        y = mario_y;
    }

    // Deactivate hitbox after its lifespan
    if (GetTickCount64() - activate_start > HITBOX_LIFESPAN)
    {
        isActive = false;
    }
}
void HitBox::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
    CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
    if (goomba->GetState() != GOOMBA_STATE_DIE)
        goomba->SetState(GOOMBA_STATE_DIE);
}

void HitBox::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
    CParaGoomba* paraGoomba = dynamic_cast<CParaGoomba*>(e->obj);
    if (paraGoomba->GetState() != PARAGOOMBA_STATE_DIE)
        paraGoomba->SetState(PARAGOOMBA_STATE_DIE);
}

void HitBox::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
    Koopas* koopas = dynamic_cast<Koopas*>(e->obj);
    if (koopas->GetState() != KOOPAS_STATE_SHELL)
    {
        koopas->SetState(KOOPAS_STATE_SHELL);
    }
    else if (koopas->GetState() == KOOPAS_STATE_SHELL)
    {
        // Direction is based on Mario's direction
        //float shellDirection = owner->GetDirection() > 0 ? 1.0f : -1.0f;
        /*koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
        koopas->SetSpeed(shellDirection * KOOPAS_SHELL_SPEED, 0);*/
        DebugOut(L">>> Mario's hitbox kicked Koopa shell! >>> \n");
    }
    else if (koopas->GetState() == KOOPAS_STATE_SHELL_MOVING)
    {
        // Reverse the shell's direction
        float vx, vy;
        koopas->GetSpeed(vx, vy);
        koopas->SetSpeed(-vx, vy);
        DebugOut(L">>> Mario's hitbox reversed Koopa shell direction! >>> \n");
    }
}

void HitBox::OnCollisionWithKoopaParatroopa(LPCOLLISIONEVENT e)
{
    CKoopaParatroopa* koopaParatroopa = dynamic_cast<CKoopaParatroopa*>(e->obj);
    if (koopaParatroopa->GetState() == KOOPA_PARATROOPA_STATE_WALKING_WINGS ||
        koopaParatroopa->GetState() == KOOPA_PARATROOPA_STATE_JUMPING_WINGS)
        koopaParatroopa->SetState(KOOPA_PARATROOPA_STATE_WALKING);
    else if (koopaParatroopa->GetState() == KOOPA_PARATROOPA_STATE_WALKING)
        koopaParatroopa->SetState(KOOPA_PARATROOPA_STATE_SHELL);
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
    /*if (brick->IsBreakable())
        brick->Break();*/
}
void HitBox::Render()
{
    if (!isActive) return;
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
    isActive = true;
    activate_start = GetTickCount64();
}

void HitBox::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!isActive) return;
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
}