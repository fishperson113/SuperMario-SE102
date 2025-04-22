#include "Koopas.h"

void Koopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPAS_STATE_SHELL)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT_SHELL / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT_SHELL;
	}
	else 
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}
}

void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	if (state == KOOPAS_STATE_SHELL_MOVING)
	{
		// Apply deceleration to the shell on left or right movement
		if (vx > 0)
		{
			vx -= KOOPAS_SHELL_DECELERATION * dt;
			if (vx < KOOPAS_MIN_SPEED)
			{
				vx = 0;
				SetState(KOOPAS_STATE_SHELL); 
			}
		}
		else if (vx < 0)
		{
			vx += KOOPAS_SHELL_DECELERATION * dt;
			if (vx > -KOOPAS_MIN_SPEED)
			{
				vx = 0;
				SetState(KOOPAS_STATE_SHELL); 
			}
		}
	}
	else if (state == KOOPAS_STATE_SHELL && GetTickCount64() - shell_start > KOOPAS_SHELL_TIMEOUT)
	{
		SetState(KOOPAS_STATE_WALKING);
	}

	// Add this line to continuously update sensor positions
	ResetSensors();

}

void Koopas::Render()
{
	int aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
	if (state == KOOPAS_STATE_WALKING)
	{
		if (vx > 0)
		{
			aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
		}
		else if (vx < 0)
		{
			aniId = ID_ANI_KOOPAS_WALKING_LEFT;
		}
	}
	else if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_SHELL_MOVING)
	{
		aniId = ID_ANI_KOOPAS_SHELL;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void Koopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Koopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<Koopas*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
		ResetSensors();
	}
}

Koopas::Koopas(float x, float y)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	die_start = -1;
	shell_start = 0;
	frontSensor = new FallSensor(x, y, this, true);
	backSensor = new FallSensor(x, y, this, false);

	SetState(KOOPAS_STATE_SHELL);
	ResetSensors();
}

void Koopas::SetState(int state)
{
	int previousState = this->state;
	this->state = state;
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
		ay = KOOPAS_GRAVITY;
		if (previousState == KOOPAS_STATE_SHELL)
		{
			y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2;
		}
		break;

	case KOOPAS_STATE_SHELL_MOVING:
		ay = KOOPAS_GRAVITY; 
		shell_start = 0;
		break;
	case KOOPAS_STATE_SHELL:
		vx = 0;
		ay = KOOPAS_GRAVITY;

		if (previousState == KOOPAS_STATE_WALKING)
		{
			y += (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2;
		}

		shell_start = GetTickCount64();
		break;
	}
}

void Koopas::ResetSensors()
{
	if (!frontSensor || !backSensor) return;

	float koopas_left, koopas_top, koopas_right, koopas_bottom;
	GetBoundingBox(koopas_left, koopas_top, koopas_right, koopas_bottom);

	float sensorY = y;

	if (vx > 0) 
	{
		frontSensor->SetPosition(koopas_right + 4, sensorY);

		backSensor->SetPosition(koopas_left - 4, sensorY);
	}
	else 
	{
		frontSensor->SetPosition(koopas_left - 4, sensorY);

		backSensor->SetPosition(koopas_right + 4, sensorY);
	}
}
