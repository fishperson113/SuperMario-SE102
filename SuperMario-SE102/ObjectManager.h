#pragma once

#include <vector>
#include <algorithm>
#include "GameObject.h"
#include "Collision.h"
#include"Checkpoint.h"
class ObjectManager
{
private:
    std::vector<LPGAMEOBJECT> objects;
    LPGAMEOBJECT player;
public:
    ObjectManager();
    ~ObjectManager();

    void Add(LPGAMEOBJECT object);
    void AddPlayer(LPGAMEOBJECT player);
    void Remove(LPGAMEOBJECT object);
    void Clear();
    void Update(DWORD dt);
    void Render();
    void PurgeDeletedObjects();

    LPGAMEOBJECT GetPlayer() const { return player; }
    const std::vector<LPGAMEOBJECT>& GetObjects() const { return objects; }
    std::vector<LPGAMEOBJECT> GetCollidableObjects() const;
	bool IsValidPointer(const LPGAMEOBJECT& o) const;
    bool IsGameObjectDeleted(const LPGAMEOBJECT& o) const;
    size_t Size() const { return objects.size(); }
private:
    void ProcessCollisions(DWORD dt);
};
