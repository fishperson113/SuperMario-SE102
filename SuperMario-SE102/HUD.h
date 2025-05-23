#pragma once
#include"Game.h"
#define ID_HUD          100000
#define ID_NUMBER_0     0
#define ID_NUMBER_1     1
#define ID_NUMBER_2     2
#define ID_NUMBER_3     3
#define ID_NUMBER_4     4
#define ID_NUMBER_5     5
#define ID_NUMBER_6     6
#define ID_NUMBER_7     7
#define ID_NUMBER_8     8
#define ID_NUMBER_9     9
#define ID_LETTER_M     13
#define ID_LETTER_P     16
#define ID_LETTER_A     17
#define ID_LETTER_U     18
#define ID_LETTER_S     19
#define ID_LETTER_E     22
#define ID_SPEED_BAR    30
#define ID_P_BUTTON     31
#define ID_HUD_START    100001
#define ID_HUD_END      100002

class HUD
{
    static HUD* __HudInstance;  // Ensure only one HUD instance exists
    float x, y;
    bool isUndrawInitialized;
    bool isAffect;  
    bool initCard; 

public:
    static bool isStarting;
    static bool initStart;
    static ULONGLONG Hud_Start_Draw_Time;
    static bool isAllowToRenderHudStart;

    HUD(float x, float y);
    static HUD* GetInstance();
    void Update();
    void UpdateCard();
    int Get3Card();

    void Render();
    void RenderHP();
    void RenderCoin();
    void RenderTimer();
    void RenderPoints();
    void RenderSpeedBar();
    void RenderCard();
    void RenderHudStart();
    void RenderHudEnd();
    void RenderPauseText();

    void SetInitCard(bool para) { this->initCard = para; }
};
