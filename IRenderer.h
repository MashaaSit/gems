#ifndef IRENDERER_H
#define IRENDERER_H

#include "GameObjects.h"
#include <vector>

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void swapObject(GameObject& obj1, GameObject& obj2) = 0;
    virtual void animateObjectMovement(std::vector<GameObject*>& objects_to_remove) = 0;
};

#endif