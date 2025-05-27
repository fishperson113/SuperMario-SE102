#pragma once
#include "GameObject.h"
#include <vector>

#define CARD_BBOX_WIDTH		16
#define CARD_BBOX_HEIGHT	16

#define CARD_STATE_MUSHROOM	1
#define CARD_STATE_STAR	    2
#define CARD_STATE_FLOWER	3
#define CARD_STATE_BE_COLLECTED	4
#define CARD_STATE_STATIC	5  

#define ID_ANI_CARD_MUSHROOM	91000
#define ID_ANI_CARD_STAR	    91001
#define ID_ANI_CARD_FLOWER	    91002

#define ID_ANI_BLACK_CARD_MUSHROOM	91100
#define ID_ANI_BLACK_CARD_STAR	    91101
#define ID_ANI_BLACK_CARD_FLOWER	91102

#define ID_ANI_STATIC_CARD_MUSHROOM	91200
#define ID_ANI_STATIC_CARD_STAR	    91201
#define ID_ANI_STATIC_CARD_FLOWER	91202

#define TIME_SWITCH_CARD	150     
#define CARD_FLY_SPEED		0.06f   

class Card;
typedef Card* LPCARD;

class Card : public CGameObject
{
    int type;
    ULONGLONG switch_card_start;
    BOOLEAN isMushroom, isStar, isFlower;
    static std::vector<LPCARD> collectedCards;
    static int lastCollectedType;

public:
    Card(float x, float y);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
    void SetState(int state);
    void GetBoundingBox(float& l, float& t, float& r, float& b);

    int IsBlocking() { return 0; }
    int IsCollidable() { return state != CARD_STATE_BE_COLLECTED; }

    int GetType() const;

    static int GetLastCollectedType() { return lastCollectedType; }
    static int GetCollectedCardCount(int type);
    static int GetTotalCollectedCards();
    static void AddCollectedCard(LPCARD card);
    static bool HasCollectedAllCards();
    static const std::vector<LPCARD>& GetCollectedCards() { return collectedCards; }
    void OnNoCollision(DWORD dt);
};