#include "DoorController.h"

void DoorController::Update(float dt)
{

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
