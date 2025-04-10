#include "ObjectManager.h"
#include "GameObject.h"

ObjectManager* ObjectManager::instance = nullptr;
ObjectManager::ObjectManager() {}

ObjectManager::~ObjectManager() {
    for (CGameObject* obj : objects) {
        delete obj;
    }
    objects.clear();
}

ObjectManager* ObjectManager::GetInstance() {
    if (instance == nullptr) {
        instance = new ObjectManager();
    }
    return instance;
}

void ObjectManager::AddObject(CGameObject* obj) {
    objects.push_back(obj);
}

void ObjectManager::RemoveObject(CGameObject* obj) {
    auto it = find(objects.begin(), objects.end(), obj);
    if (it != objects.end()) {
        objects.erase(it);
    }
}

void ObjectManager::Update(DWORD dt) {
    for (CGameObject* obj : objects) {
        if (obj->IsActive()) {
            obj->Update(dt);
        }
    }
}

void ObjectManager::Render() {
    for (CGameObject* obj : objects) {
        if (obj->IsActive()) {
            obj->Render();
        }
    }
}
