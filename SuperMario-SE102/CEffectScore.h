#pragma once
#include "CEffect.h"
// Speeds and durations
#define EFFECT_SCORE_RISE_SPEED 0.05f
#define EFFECT_SCORE_LIFETIME 1000  // 1 second

// Score values
#define SCORE_100  100
#define SCORE_200  200
#define SCORE_400  400
#define SCORE_800  800
#define SCORE_1000 1000
#define SCORE_LEVEL_UP 1100  

// Animation IDs for different score values
#define ID_ANI_EFFECT_SCORE_100  10
#define ID_ANI_EFFECT_SCORE_200  20
#define ID_ANI_EFFECT_SCORE_400  40
#define ID_ANI_EFFECT_SCORE_800  80
#define ID_ANI_EFFECT_SCORE_1000 100
#define ID_ANI_EFFECT_LEVEL_UP 110 

class CEffectScore : public CEffect
{
private:
    int score;  // The score value to display
    int animationId;  // Animation ID for this score

public:
    CEffectScore(float x, float y, int score);
    virtual void Render();
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

    // Helper function to convert score value to animation ID
    static int ScoreToAnimationId(int score);
};

