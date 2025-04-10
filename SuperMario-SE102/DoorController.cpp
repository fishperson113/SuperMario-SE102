#include "DoorController.h"
#include "Mario.h"

void DoorController::Update(float dt)
{
    switch (state) {
    case DoorState::Closed:
        if (IsMarioInRange()) {
            SetState(DoorState::Opening);
        }
        break;

    case DoorState::Opening:
        if (animComponent->GetCurrentAnimation()->IsFinished()) {
            SetState(DoorState::Open);
        }
        break;

    case DoorState::Open:
        if (!IsMarioInRange()) {
            SetState(DoorState::Closing);
        }
        break;

    case DoorState::Closing:
        if (animComponent->GetCurrentAnimation()->IsFinished()) {
            SetState(DoorState::Closed);
        }
        break;
    }
}

void DoorController::Awake()
{
    animComponent = parentObject->GetComponent<AnimationComponent>();
    SetState(DoorState::Closed);
    auto collider = parentObject->AddComponent<ColliderComponent>();
    collider->SetBoundingBox(0, 0, 10, 60);
}

void DoorController::Start()
{
}

bool DoorController::IsMarioInRange() const
{
	//extern CGameObject* mario;
    extern CMario* mario;
    if (!mario || !parentObject || !mario->IsActive()) return false;

    auto doorTransform = parentObject->GetComponent<TransformComponent>();
    auto marioTransform = mario->GetComponent<TransformComponent>();
    if (!doorTransform || !marioTransform) return false;

    float dx = doorTransform->GetPositionX() - marioTransform->GetPositionX();
    float dy = doorTransform->GetPositionY() - marioTransform->GetPositionY();

    float distanceSquared = dx * dx + dy * dy;
    return distanceSquared <= collisionRange * collisionRange;
}

void DoorController::SetState(DoorState newState)
{
    state = newState;
    LPANIMATION animation = CAnimations::GetInstance()->Get(doorAnimationId);

    switch (state) {
    case DoorState::Closed:
        animation->SetPlayMode(AnimationPlayMode::PlayAndHold);
        animation->JumpToLastFrame();
        break;

    case DoorState::Opening:
        animation->SetPlayMode(AnimationPlayMode::PlayAndHold);
        animation->Reset();
        break;

    case DoorState::Open:
        animation->SetPlayMode(AnimationPlayMode::PlayAndHold);
        animation->SetCurrentFrame(0);
        break;

    case DoorState::Closing:
        animation->SetPlayMode(AnimationPlayMode::PlayAndHold);
        animation->Reset();
        break;
    }
    animComponent->SetCurrentAnimation(animation);
}
