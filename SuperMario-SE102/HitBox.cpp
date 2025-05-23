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

void HitBox::Render()
{
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
    {
        CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
        if (goomba->GetState() != GOOMBA_STATE_DIE)
            goomba->SetState(GOOMBA_STATE_DIE);
    }
    else if (dynamic_cast<CParaGoomba*>(e->obj))
    {
        CParaGoomba* paraGoomba = dynamic_cast<CParaGoomba*>(e->obj);
        if (paraGoomba->GetState() != PARAGOOMBA_STATE_DIE)
            paraGoomba->SetState(PARAGOOMBA_STATE_DIE);
    }
    else if (dynamic_cast<Koopas*>(e->obj))
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
    else if (dynamic_cast<CKoopaParatroopa*>(e->obj))
    {
        CKoopaParatroopa* koopaParatroopa = dynamic_cast<CKoopaParatroopa*>(e->obj);
        if (koopaParatroopa->GetState() == KOOPA_PARATROOPA_STATE_WALKING_WINGS ||
            koopaParatroopa->GetState() == KOOPA_PARATROOPA_STATE_JUMPING_WINGS)
            koopaParatroopa->SetState(KOOPA_PARATROOPA_STATE_WALKING);
        else if (koopaParatroopa->GetState() == KOOPA_PARATROOPA_STATE_WALKING)
            koopaParatroopa->SetState(KOOPA_PARATROOPA_STATE_SHELL);
    }
    else if (dynamic_cast<CPiranhaPlant*>(e->obj))
    {
        CPiranhaPlant* plant = dynamic_cast<CPiranhaPlant*>(e->obj);
        plant->Delete();
    }
    else if (dynamic_cast<CBullet*>(e->obj))
    {
        CBullet* bullet = dynamic_cast<CBullet*>(e->obj);
        bullet->Delete();
    }
    else if (dynamic_cast<CBrick*>(e->obj))
    {
        // Only break bricks that are breakable
        CBrick* brick = dynamic_cast<CBrick*>(e->obj);
        /*if (brick->IsBreakable())
            brick->Break();*/
    }
}