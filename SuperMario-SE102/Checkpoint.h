#pragma once
#include "GameObject.h"
#include <vector>
#define CHECKPOINT_BBOX_WIDTH 16
#define CHECKPOINT_BBOX_HEIGHT 32
#define CHECKPOINT_STATE_INACTIVE 0
#define CHECKPOINT_STATE_ACTIVE 1

class ObjectManager;
class Checkpoint : public CGameObject
{
private:
    std::vector<LPGAMEOBJECT> objectsToSpawn;
    float width;
    float height;
    bool isActivated;
	ObjectManager* objectManager;
public:
    Checkpoint(float x, float y, float width = CHECKPOINT_BBOX_WIDTH, float height = CHECKPOINT_BBOX_HEIGHT,ObjectManager* objectManager = nullptr);

    void AddObjectToSpawn(LPGAMEOBJECT obj);

    void AddObjectsToSpawn(const std::vector<LPGAMEOBJECT>& objects);

    void SpawnObjects();

    virtual void Render();
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects=NULL);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual int IsBlocking() { return 0; }
    virtual void SetState(int state);
    bool IsActivated() const { return isActivated; }
};

