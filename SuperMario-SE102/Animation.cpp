#include "Animation.h"

LPANIMATION_FRAME CAnimation::CreateFrame(DWORD time)
{
    int t = time;
    if (time == 0) t = this->defaultTime;

    LPANIMATION_FRAME frame = new CAnimationFrame(t);
    frames.push_back(frame);
    return frame;
}

void CAnimation::Add(int spriteId, DWORD time)
{
    LPANIMATION_FRAME frame = CreateFrame(time);
    LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
    frame->AddSprite(sprite, 0, 0);
}

void CAnimation::AddSprite(int spriteId, float offsetX, float offsetY)
{
    if (frames.empty()) {
        CreateFrame();
    }

    LPANIMATION_FRAME currentFrame = frames.back();
    LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
    currentFrame->AddSprite(sprite, offsetX, offsetY);
}

void CAnimation::Render(float x, float y)
{
    if (frames.empty()) return;

    ULONGLONG now = GetTickCount64();
    if (currentFrame == -1)
    {
        currentFrame = 0;
        lastFrameTime = now;
        isFinished = false;
    }

    if (isFinished)
    {
        frames[currentFrame]->Draw(x, y);
        return;
    }

    DWORD frameTime = frames[currentFrame]->GetTime();
    if (now - lastFrameTime <= frameTime)
    {
        frames[currentFrame]->Draw(x, y);
        return;
    }

    lastFrameTime = now;
    UpdateFrame();

    frames[currentFrame]->Draw(x, y);

}

void CAnimation::UpdateFrame()
{
    switch (playMode)
    {
    case AnimationPlayMode::Loop:
        currentFrame++;
        if (currentFrame >= (int)frames.size())
            currentFrame = 0;
        break;

    case AnimationPlayMode::PlayOnce:
        currentFrame++;
        if (currentFrame >= (int)frames.size()) {
            currentFrame = 0; 
            isFinished = true;
        }
        break;

    case AnimationPlayMode::PlayAndHold:
        currentFrame++;
        if (currentFrame >= (int)frames.size()) {
            currentFrame = frames.size() - 1; 
            isFinished = true;
        }
        break;

    case AnimationPlayMode::PingPong:
        if (playingForward) {
            currentFrame++;
            if (currentFrame >= (int)frames.size() - 1) {
                playingForward = false;
                currentFrame = frames.size() - 1;
            }
        }
        else {
            currentFrame--;
            if (currentFrame <= 0) {
                playingForward = true;
                currentFrame = 0;
            }
        }
        break;
    }
}
void CAnimation::SetCurrentFrame(int frameIndex) {
    if (frameIndex >= 0 && frameIndex < (int)frames.size()) {
        currentFrame = frameIndex;
        lastFrameTime = GetTickCount64();
        isFinished = false;
    }
}

