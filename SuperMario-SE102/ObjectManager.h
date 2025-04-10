#include <vector>
#include "GameObject.h"

class ObjectManager {
public:
    static ObjectManager* GetInstance();

    void AddObject(CGameObject* obj);

    void RemoveObject(CGameObject* obj);

    void Update(DWORD dt);

    void Render();

    ~ObjectManager();

private:
    ObjectManager();
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;

    static ObjectManager* instance;

    std::vector<CGameObject*> objects;
};
