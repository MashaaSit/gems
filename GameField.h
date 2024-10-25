#ifndef GAMEFIELD_H
#define GAMEFIELD_H
#include <iostream>
#define GLEW_STATIC
#include <list>
#include <vector>
#include <ranges>
#include <algorithm>
#include <memory>

#include "Renderer.h"
#include "Shader.h"
#include "GameObjects.h"


enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    POINT
};

class GameField {
private:
    bool _start_flag;
    ShapeType _type;

    std::vector<GameObject*> _getObjectsToRemove(GameObject obj1);
    void _checkNeighbors(Color color, int x, int y, std::vector<GameObject*>& objects_to_remove, Direction dir);
    void _deleteGameObject(GameObject& obj);
    void _deleteGameObject(std::vector<GameObject*>& objects_to_remove);
    void _SetRandomShapeType(std::list<GameObject*>& non_empty_squares);
    void _updateGameObjectsY(std::list<GameObject>& row, int start_y);
    void _updateGameObjectsX(std::list<std::list<GameObject>>::iterator& row, int start_x);
    std::list<GameObject*> _getNonEmptySquaresInRadius(GameObject* obj, int radius);
    void _shuffle(std::vector<std::pair<int, int>>& vec);
    bool _hasThreeOfSameColor();
    bool _hasNotSameColor(GameObject& obj);
    std::vector<GameObject*> _getRandomObject(int count);

public:
    GameField(int x, int y, ShapeType type_);
    void GenerateField();
    GameObject& getObject(int x, int y);
    bool swapObjects(int x_index, int y_index, int x_index_last, int y_index_last);
    std::list<std::list<GameObject>> rows;
    inline static int x_parts;
    inline static int y_parts;
    inline static int x_index, y_index;
    inline static int x_index_last, y_index_last;
    std::unique_ptr<IRenderer> animate;
    std::list<GameObject> fill;
    int count_bomb;
    bool removeSameColorObjects();
    void Fill(GameObject& obj);
    void Update();
};

#endif