#include "ObjectManager.h"
#include "debug.h"
#include <algorithm>

ObjectManager::ObjectManager() : player(nullptr)
{
}

ObjectManager::~ObjectManager()
{
    Clear();
}

void ObjectManager::Add(LPGAMEOBJECT object)
{
    if (object == nullptr)
        return;

    objects.push_back(object);
}

void ObjectManager::AddPlayer(LPGAMEOBJECT playerObj)
{
    player = playerObj;
    Add(playerObj);
}

void ObjectManager::Remove(LPGAMEOBJECT object)
{
    if (object == nullptr)
        return;

    auto it = std::find(objects.begin(), objects.end(), object);
    if (it != objects.end())
    {
        if (*it == player)
            player = nullptr;

        delete* it;
        objects.erase(it);
    }
}

void ObjectManager::Clear()
{
    for (auto obj : objects)
    {
        if (obj)
            delete obj;
    }
    objects.clear();
    player = nullptr;
}

void ObjectManager::Update(DWORD dt)
{

    for (auto obj : objects)
    {
        obj->Update(dt);
    }

    ProcessCollisions(dt);
}

void ObjectManager::Render()
{
    for (auto obj : objects)
    {
        obj->Render();
    }
}

bool ObjectManager::IsGameObjectDeleted(const LPGAMEOBJECT& o) const
{
    return o == nullptr;
}

void ObjectManager::ProcessCollisions(DWORD dt)
{
    if (objects.empty()) return;

    CCollision* collision = CCollision::GetInstance();

    for (auto obj : objects)
    {
        if (!obj || !obj->IsActive()) continue;

        std::vector<LPGAMEOBJECT> coObjects;
        for (auto other : objects)
        {
            if (other != obj && other->IsActive())
                coObjects.push_back(other);
        }

        if (!coObjects.empty())
            collision->Process(obj, dt, &coObjects);
    }
}

void ObjectManager::PurgeDeletedObjects()
{
    for (auto& obj : objects)
    {
        if (obj && !obj->IsActive())
        {
            delete obj;
            obj = nullptr;
        }
    }

    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [this](const LPGAMEOBJECT& o) { return IsGameObjectDeleted(o); }),
        objects.end());
}

std::vector<LPGAMEOBJECT> ObjectManager::GetCollidableObjects() const
{
    std::vector<LPGAMEOBJECT> coObjects;
    for (auto obj : objects)
    {
        if (obj != player)
            coObjects.push_back(obj);
    }
    return coObjects;
}
