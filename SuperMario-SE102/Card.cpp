#include "Card.h"
#include "Sprites.h"
#include "Animations.h"
#include "PlayScene.h"
#include "debug.h"

// Initialize static members
std::vector<LPCARD> Card::collectedCards;
int Card::lastCollectedType = 0;

Card::Card(float x, float y) : CGameObject(x, y)
{
    switch_card_start = GetTickCount64();

    state = CARD_STATE_STAR;
    isMushroom = false;
    isStar = true;
    isFlower = false;

    type = CARD_STATE_STAR;
}

void Card::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    // If card is in static state (displayed in HUD), no need for further updates
    if (state == CARD_STATE_STATIC) return;

    // Delete the card when it goes off the top of the screen after being collected
    if (y <= 100.0f && state == CARD_STATE_BE_COLLECTED)
    {
        this->Delete();
        return;
    }

    // Cycle through card types if not collected or static
    if (state != CARD_STATE_BE_COLLECTED && state != CARD_STATE_STATIC)
    {
        if (GetTickCount64() - switch_card_start > TIME_SWITCH_CARD)
        {
            // Cycle through card types: mushroom -> star -> flower -> mushroom
            if (isMushroom)
                SetState(CARD_STATE_STAR);
            else if (isStar)
                SetState(CARD_STATE_FLOWER);
            else if (isFlower)
                SetState(CARD_STATE_MUSHROOM);
        }
    }

    // Apply vertical movement for collected cards
    if (state == CARD_STATE_BE_COLLECTED)
    {
        y += vy * dt;
    }

}

void Card::Render()
{
    int aniID = -1;

    if (state == CARD_STATE_MUSHROOM)
        aniID = ID_ANI_CARD_MUSHROOM;
    else if (state == CARD_STATE_STAR)
        aniID = ID_ANI_CARD_STAR;
    else if (state == CARD_STATE_FLOWER)
        aniID = ID_ANI_CARD_FLOWER;
    else if (state == CARD_STATE_BE_COLLECTED)
    {
        if (isMushroom)
            aniID = ID_ANI_BLACK_CARD_MUSHROOM;
        else if (isStar)
            aniID = ID_ANI_BLACK_CARD_STAR;
        else
            aniID = ID_ANI_BLACK_CARD_FLOWER;
    }
    else if (state == CARD_STATE_STATIC)
    {
        if (isMushroom)
            aniID = ID_ANI_STATIC_CARD_MUSHROOM;
        else if (isStar)
            aniID = ID_ANI_STATIC_CARD_STAR;
        else
            aniID = ID_ANI_STATIC_CARD_FLOWER;
    }

    CAnimations* animations = CAnimations::GetInstance();
    if (aniID != -1 && animations != nullptr)
    {
        if (state != CARD_STATE_STATIC)
            animations->Get(aniID)->Render(x, y);
        else
            animations->Get(aniID)->Render(x, y); 
    }
}

void Card::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - CARD_BBOX_WIDTH / 2;
    t = y - CARD_BBOX_HEIGHT / 2;
    r = l + CARD_BBOX_WIDTH;
    b = t + CARD_BBOX_HEIGHT;
}

void Card::SetState(int state)
{
    // Handle state transitions
    switch (state)
    {
    case CARD_STATE_MUSHROOM:
        switch_card_start = GetTickCount64();
        isMushroom = true;
        isStar = false;
        isFlower = false;
        type = CARD_STATE_MUSHROOM;
        break;

    case CARD_STATE_STAR:
        switch_card_start = GetTickCount64();
        isMushroom = false;
        isStar = true;
        isFlower = false;
        type = CARD_STATE_STAR;
        break;

    case CARD_STATE_FLOWER:
        switch_card_start = GetTickCount64();
        isMushroom = false;
        isStar = false;
        isFlower = true;
        type = CARD_STATE_FLOWER;
        break;

    case CARD_STATE_BE_COLLECTED:
        // When collected, card flies upward
        vy = -CARD_FLY_SPEED;

        // Record the collected card type
        lastCollectedType = GetType();
        AddCollectedCard(this);  
        break;
    }

    CGameObject::SetState(state);
}

void Card::OnNoCollision(DWORD dt)
{
    // Update position when no collision occurs
    y += vy * dt;
}

int Card::GetType() const
{
    if (isMushroom) return CARD_STATE_MUSHROOM;
    else if (isStar) return CARD_STATE_STAR;
    else return CARD_STATE_FLOWER;
}

int Card::GetCollectedCardCount(int type)
{
    if (type < CARD_STATE_MUSHROOM || type > CARD_STATE_FLOWER)
        return 0;

    int count = 0;
    for (auto card : collectedCards)
    {
        if (card->GetType() == type)
            count++;
    }
    return count;
}

int Card::GetTotalCollectedCards()
{
    return collectedCards.size();
}

void Card::AddCollectedCard(LPCARD card)
{
    if (card && card->GetType() >= CARD_STATE_MUSHROOM && card->GetType() <= CARD_STATE_FLOWER)
        collectedCards.push_back(card);
}

bool Card::HasCollectedAllCards()
{
    bool hasMushroom = false;
    bool hasStar = false;
    bool hasFlower = false;

    for (auto card : collectedCards)
    {
        int cardType = card->GetType();
        if (cardType == CARD_STATE_MUSHROOM)
            hasMushroom = true;
        else if (cardType == CARD_STATE_STAR)
            hasStar = true;
        else if (cardType == CARD_STATE_FLOWER)
            hasFlower = true;
    }

    return (hasMushroom && hasStar && hasFlower);
}