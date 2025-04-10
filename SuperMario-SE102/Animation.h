#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"


using namespace std;

enum class AnimationPlayMode {
    Loop,           // Loop continuously
    PlayOnce,       // Play once and stop on last frame
    PlayAndHold,    // Play once and hold last frame
    PingPong        // Play forward then backward
};
class CAnimation
{
    ULONGLONG lastFrameTime;
    int defaultTime;
    int currentFrame;
    vector<LPANIMATION_FRAME> frames;

    AnimationPlayMode playMode;
    bool playingForward; 
    bool isFinished;     


public:
    CAnimation(int defaultTime=100) {
        this->defaultTime = defaultTime;
        lastFrameTime = -1;
        currentFrame = -1;
        playMode = AnimationPlayMode::Loop;
        playingForward = true;
        isFinished = false;
    }
    LPANIMATION_FRAME CreateFrame(DWORD time = 0);
    void Add(int spriteId, DWORD time = 0);
    void AddSprite(int spriteId, float offsetX = 0, float offsetY = 0);
    void Render(float x, float y);

    void SetPlayMode(AnimationPlayMode mode) {
        playMode = mode;
        isFinished = false; 
    }

    AnimationPlayMode GetPlayMode() const { return playMode; }
    bool IsFinished() const { return isFinished; }

    void SetCurrentFrame(int frameIndex);
    int GetCurrentFrame() const { return currentFrame; }
    int GetFrameCount() const { return (int)frames.size(); }

	void UpdateFrame();

    void Reset() {
        currentFrame = 0;
        lastFrameTime = GetTickCount64();
        playingForward = true;
        isFinished = false;
    }

    void JumpToLastFrame() {
        if (!frames.empty()) {
            currentFrame = frames.size() - 1;
            lastFrameTime = GetTickCount64();
        }
    }
};

typedef CAnimation* LPANIMATION;