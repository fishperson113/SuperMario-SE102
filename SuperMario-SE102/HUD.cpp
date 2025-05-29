#include "HUD.h"
#include "Sprites.h"
#include "Game.h"
#include "Mario.h"
#include "PlayScene.h"
#include "debug.h"
#include "Card.h"
HUD* HUD::__HudInstance = NULL;
bool HUD::isStarting = false;
bool HUD::initStart = false;
ULONGLONG HUD::Hud_Start_Draw_Time = 0;
bool HUD::isAllowToRenderHudStart = false;

HUD* HUD::GetInstance()
{
    if (__HudInstance == NULL)
    {
        CGame* game = CGame::GetInstance();
        float cam_x, cam_y;
        game->GetCamPos(cam_x, cam_y);

        __HudInstance = new HUD(cam_x, cam_y);
    }
    return __HudInstance;
}

HUD::HUD(float x, float y)
{
    this->x = x;
    this->y = y;
    isUndrawInitialized = false;
    isAffect = false;
    initCard = false;
}

void HUD::Update()
{
    // Update HUD position to follow camera
    CGame* game = CGame::GetInstance();
    float cam_x, cam_y;
    game->GetCamPos(cam_x, cam_y);

    // Position HUD at the top of the screen
    this->x = cam_x + game->GetBackBufferWidth() / 2;
    this->y = cam_y + +game->GetBackBufferHeight() - 16.0f;

    // Update card effects if any
    UpdateCard();
}
void HUD::Render()
{
    CSprites* sprites = CSprites::GetInstance();

    // Render the HUD background
    sprites->Get(ID_HUD)->Draw(x, y,1.2,1.2);

    // Render all HUD elements
    RenderHP();
    RenderCoin();
    RenderTimer();
    RenderPoints();
    RenderSpeedBar();
    RenderCard();

    // Get the current scene
    LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
    if (scene && scene->IsPaused())
    {
        RenderPauseText();
    }
    if (scene && scene->IsGameOver())
    {
        RenderHudEnd();
	}

}
void HUD::UpdateCard()
{
    LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
    if (!scene) return;

    CMario* mario = (CMario*)scene->GetPlayer();
    if (!mario) return;

    if (!initCard)
    {
        initCard = true;
    }
}

int HUD::Get3Card()
{
    if (Card::HasCollectedAllCards())
    {
        return 7;
    }

    return Card::GetLastCollectedType();
}

void HUD::RenderHP()
{
    CSprites* sprites = CSprites::GetInstance();
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    if (!mario) return;

    sprites->Get(ID_LETTER_M)->Draw(x - 128.0f, y + 5.0f, 1.2, 1.2);

    int lives = 3;

    sprites->Get(ID_NUMBER_0 + lives)->Draw(x - 96.0f, y + 5.0f, 1.2, 1.2);

}

void HUD::RenderCoin()
{
    // Render coin count
    CSprites* sprites = CSprites::GetInstance();
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    if (!mario) return;

    // Get Mario's coin count
    int coins = mario->GetCoin(); 

    // Draw coin count
    if (coins <= 9)
    {
        // Single digit
        sprites->Get(ID_NUMBER_0 + coins)->Draw(x + 25.0f, y - 5.0f, 1.2, 1.2);
    }
    else
    {
        // Double digits
        int ones = coins % 10;
        int tens = coins / 10;

        sprites->Get(ID_NUMBER_0 + ones)->Draw(x + 31.0f, y - 5.0f, 1.2, 1.2);
        sprites->Get(ID_NUMBER_0 + tens)->Draw(x + 21.0f, y - 5.0f, 1.2, 1.2);
    }
}

void HUD::RenderTimer()
{
    // Render countdown timer
    CSprites* sprites = CSprites::GetInstance();
    LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();

    int timeRemaining = scene->GetTimeRemaining();

    // Extract digits
    int hundreds = timeRemaining / 100;
    int tens = (timeRemaining / 10) % 10;
    int ones = timeRemaining % 10;

    // Draw timer digits
    sprites->Get(ID_NUMBER_0 + hundreds)->Draw(x + 14.0f, y + 5.0f, 1.2, 1.2);
    sprites->Get(ID_NUMBER_0 + tens)->Draw(x + 22.0f, y + 5.0f, 1.2, 1.2);
    sprites->Get(ID_NUMBER_0 + ones)->Draw(x + 30.0f, y + 5.0f, 1.2, 1.2);

}

void HUD::RenderPoints()
{
    // Render score points
    CSprites* sprites = CSprites::GetInstance();
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    if (!mario) return;

    // Get points (assuming points is accessible)
    int points = mario->GetPoints(); // You might need a getter method

    // Extract digits (for a 7-digit score)
    int digits[7];
    for (int i = 0; i < 7; i++)
    {
        digits[6 - i] = points % 10;
        points /= 10;
    }

    // Draw each digit
    for (int i = 0; i < 7; i++)
    {
        sprites->Get(ID_NUMBER_0 + digits[i])->Draw(x - 70.0f + i * 9.0f, y + 5.0f, 1.2, 1.2);
    }

}

void HUD::RenderSpeedBar()
{
    // Render speed/power meter for running
    CSprites* sprites = CSprites::GetInstance();
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    if (!mario) return;

    float powerMeter = mario->GetPowerMeter();
    int barSegments = (powerMeter +1);

    // Draw speed bar segments
    for (int i = 0; i < barSegments; i++)
    {
        if (i == 6) // P-meter full
        {
            sprites->Get(ID_P_BUTTON)->Draw(x - 10.0f, y - 5.0f, 1.2, 1.2);
        }
        else
        {
            // Draw regular speed bar segments
            sprites->Get(ID_SPEED_BAR)->Draw(x - (73.0f - 9.0f * i), y - 6.0f, 1.2, 1.2);
        }
    }
}

void HUD::RenderCard()
{
    CSprites* sprites = CSprites::GetInstance();
    if (!sprites) return;

    // Position for displaying cards in the HUD
    float cardX = x + 64.0f;
    float cardY = y ;
    float cardSpacing = 30.0f; // Space between cards

    // Get the collected cards
    auto listCards = Card::GetCollectedCards();

    int cardsToDisplay = 3;
    if (listCards.size() < cardsToDisplay)
    {
        cardsToDisplay = listCards.size(); // Adjust if fewer cards are collected
	}
    for (int i = 0; i < cardsToDisplay; i++)
    {
        //int cardIndex = listCards.size() - 1 - i;
        auto card = listCards[i];
        // Determine sprite ID based on card type
        int spriteID = 0;
        if (card->GetType() == CARD_STATE_MUSHROOM)
        {
            spriteID = 90200; // Mushroom card sprite ID
        }
        else if (card->GetType() == CARD_STATE_STAR)
        {
            spriteID = 90201; // Star card sprite ID
        }
        else if (card->GetType() == CARD_STATE_FLOWER)
        {
            spriteID = 90202; // Flower card sprite ID
        }

        float posX = cardX + i * cardSpacing;

        // Draw the card
        if (spriteID != 0)
        {
            sprites->Get(spriteID)->Draw(posX, cardY, 1.2, 1.2);
        }
    }
}

void HUD::RenderHudStart()
{
}

void HUD::RenderHudEnd()
{
    CSprites* sprites = CSprites::GetInstance();
    CGame* game = CGame::GetInstance();
    float cam_x, cam_y;
    game->GetCamPos(cam_x, cam_y);
    float screenCenterX = cam_x + game->GetBackBufferWidth() / 2.0f;
    float screenCenterY = cam_y + game->GetBackBufferHeight() / 2.0f;


    sprites->Get(ID_HUD_END)->Draw(screenCenterX, screenCenterY, 1.2, 1.2);
}

void HUD::RenderPauseText()
{
    CSprites* sprites = CSprites::GetInstance();
    CGame* game = CGame::GetInstance();

    float cam_x, cam_y;
    game->GetCamPos(cam_x, cam_y);

    float screenCenterX = cam_x + game->GetBackBufferWidth() / 2.0f;
    float screenCenterY = cam_y + game->GetBackBufferHeight() / 2.0f;


    sprites->Get(ID_LETTER_P)->Draw(screenCenterX, screenCenterY);
    sprites->Get(ID_LETTER_A)->Draw(screenCenterX + 10.0f, screenCenterY);
    sprites->Get(ID_LETTER_U)->Draw(screenCenterX + 20.0f, screenCenterY);
    sprites->Get(ID_LETTER_S)->Draw(screenCenterX + 30.0f, screenCenterY);
    sprites->Get(ID_LETTER_E)->Draw(screenCenterX + 40.0f, screenCenterY);
}
