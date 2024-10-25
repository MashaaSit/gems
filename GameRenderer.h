#ifndef GAMERENDERER_H
#define GAMERENDERER_H
#include <list>
#include <vector>
#include <ranges>
#include <algorithm>
#include "Renderer.h"
#include "GameField.h"
#include "GameObjects.h"
#include "Window.h"

class GameRenderer : public Renderer, public GameField, public IRenderer {
private:
    inline static bool _click_on_fild;
    void _displayFieldClick();
    void _FieldClick();
    inline static GameRenderer* interface;
    void _updateOldField();
    struct SwapObjectData {
        GameObject obj1;
        GameObject obj2;
    };
    struct AnimateObjectMovementData {
        GameObject obj;
        int x_new;
        int y_new;
    };
    using AnimeteSwap = std::vector<SwapObjectData>;
    using AnimeteMovemen = std::vector<AnimateObjectMovementData>;
    std::list<std::list<GameObject>> _rows_copy;
    AnimeteSwap _swap_queue;
    AnimeteMovemen _move_queue;
    void _remove_objects(std::vector<GameObject*>& objects_to_remove);
public:
    GameRenderer(GLuint VBO_, GLuint VAO_, int x_parts, int y_parts, ShapeType type_, GLint vertexColorLocation_);
    void drawField();
    void drawOldField();
    void drawSwap();
    void drawMove();
    void GEMS();
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void swapObject(GameObject& obj1, GameObject& obj2);
    void animateObjectMovement(std::vector<GameObject*>& objects_to_remove);
};

#endif