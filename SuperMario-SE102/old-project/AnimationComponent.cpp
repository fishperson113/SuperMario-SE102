#include"Component.h"

void AnimationComponent :: Render() {
    auto transform = GetParentObject()->GetComponent<TransformComponent>();
    if (!transform || !currentAnimation) return;

    currentAnimation->Render(transform->GetPositionX(), transform->GetPositionY());
}