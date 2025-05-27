#include "FireTrap.h"

CFireTrap::CFireTrap(float x, float y, int type) : CGameObject(x, y)
{
	piranhaPlant = new CPiranhaPlant(x, y + 10);

	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	currentScene->GetObjectManager()->Add(piranhaPlant);

	pipe = new CPipe(x, y, 31, 15, 3, 52001, 51001, 51001);

	currentScene->GetObjectManager()->Add(pipe);


	this->spawnTime = GetTickCount64();

	this->state = FIRETRAP_STATE_APPEARING;

    this->type = type;
}

void CFireTrap::Render()
{
}

void CFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    LPGAMEOBJECT player = currentScene->GetPlayer();

    float playerX = 0.0f;
    float playerY = 0.0f;

    if (player != nullptr)
	{
		player->GetPosition(playerX, playerY);
    }

    bool isHiding = false;

    if (playerX < 340 || playerX > 400)
        isHiding = false;
    else
        isHiding = true;

    bool isLeft = false, isRight = false;
    if (playerX < 320)
        isLeft = true;
    if (playerX > 420)
		isRight = true;

    //DebugOut(L"[INFO] Fire Trap object is hiding = %d!\n", isHiding);
    //DebugOut(L"[INFO] Fire Trap object state = %d!\n", this->state);

	ULONGLONG currentTime = GetTickCount64();
    if (currentTime - this->spawnTime > SHOOTING_TIME)
    {
        switch (state)
        {
        case FIRETRAP_STATE_APPEARING:
        if (isHiding == true) break;
        if (isHiding == false)
        {
            if (this->type == TYPE_RED_FIRE_TRAP)
            {
                this->piranhaPlant->SetAnimationId(ID_ANI_PIRANHA_LONG_LEFT);
                if (this->offset > OFFSET)
                {
                    this->offset -= 0.5f;
                    this->piranhaPlant->SetPosition(this->x, this->y + this->offset);
                }
                else
                {
                    state = FIRETRAP_STATE_SHOOTING;
                    this->hasFired = false;
                }
            }
            else if (this->type == TYPE_GREEN_FIRE_TRAP)
            {
                this->piranhaPlant->SetAnimationId(ID_ANI_PIRANHA_GREEN_LONG_LEFT);
                if (this->offset > OFFSET + 4)
                {
                    this->offset -= 0.5f;
                    this->piranhaPlant->SetPosition(this->x, this->y + this->offset);
                }
                else
                {
                    state = FIRETRAP_STATE_SHOOTING;
                    this->hasFired = false;
                }
            }
			else if (this->type == TYPE_GREEN_PIRANHA_PLANT)
			{
				this->piranhaPlant->SetAnimationId(ID_ANI_PIRANHA_GREEN_ABOVE);
				if (this->offset > OFFSET + 4)
				{
					this->offset -= 0.5f;
					this->piranhaPlant->SetPosition(this->x, this->y + this->offset);
				}
				else
				{
					state = FIRETRAP_STATE_SHOOTING;
					this->hasFired = false;
				}
			}
            break;
        }

        case FIRETRAP_STATE_SHOOTING:
            if (!this->hasFired && this->type != TYPE_GREEN_PIRANHA_PLANT)
            {
                this->piranhaPlant->ShootBullet();
                this->hasFired = true;
            }

            if (isRight)
            {
                if (this->type == TYPE_RED_FIRE_TRAP)
                    this->piranhaPlant->SetAnimationId(ID_ANI_PIRANHA_CHANGE_DIRECTION);
                else if (this->type == TYPE_GREEN_FIRE_TRAP)
                    this->piranhaPlant->SetAnimationId(ID_ANI_PIRANHA_GREEN_LONG_RIGHT);
            }

			if (this->type == TYPE_RED_FIRE_TRAP || this->type == TYPE_GREEN_FIRE_TRAP)
            {
                if (currentTime - spawnTime > HIDING_TIME)
                {
                    state = FIRETRAP_STATE_HIDING;
                }
            }
            else
            {
                if (currentTime - spawnTime > HIDING_TIME)
                {
                    state = FIRETRAP_STATE_HIDING;
                }
            }
            break;

        case FIRETRAP_STATE_HIDING:
        {
            if (this->type == TYPE_RED_FIRE_TRAP)
            {
                if (this->offset2 < -OFFSET - 1)
                {
                    this->offset2 += 0.5f;
                    this->piranhaPlant->SetPosition(this->x, this->y + this->offset2);
                }
                else
                {
                    state = FIRETRAP_STATE_HIDDEN;
                    this->piranhaPlant->SetActive(false);
                }
            }
            else
            {
                if (this->offset2 < -OFFSET - 1 - 8)
                {
                    this->offset2 += 0.5f;
                    this->piranhaPlant->SetPosition(this->x, this->y + this->offset2);
                }
                else
                {
                    state = FIRETRAP_STATE_HIDDEN;
                    this->piranhaPlant->SetActive(false);
                }
            }
            break;
        }
        case FIRETRAP_STATE_HIDDEN:
        {
            if (isHiding == true) break;
            if (this->type == TYPE_RED_FIRE_TRAP || this->type == TYPE_GREEN_FIRE_TRAP)
            {
                if (currentTime - spawnTime > HIDING_TIME + 2000)
                {
                    state = FIRETRAP_STATE_APPEARING;
                    this->offset = 0.0f;
                    this->offset2 = 0.0f;
                    this->piranhaPlant->SetActive(true);
                    this->spawnTime = currentTime;
                }
            }
            else
            {
                if (currentTime - spawnTime > HIDING_TIME - 5000)
                {
                    state = FIRETRAP_STATE_APPEARING;
                    this->offset = 0.0f;
                    this->offset2 = 0.0f;
                    this->piranhaPlant->SetActive(true);
                    this->spawnTime = currentTime;
                }
            }
            break;
        }
        default:
            break;
        }
    }
}

void CFireTrap::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

int CFireTrap::IsDirectionColliable(float nx, float ny)
{
	return 0;
}

