#include "GameField.h"


   std::vector<GameObject*>  GameField::_getObjectsToRemove(GameObject obj1) {
        std::vector<GameObject*> objects_to_remove;

        if (obj1.shapeType != BOMB && obj1.shapeType != FILL)
            _checkNeighbors(obj1.color, obj1.x, obj1.y, objects_to_remove, POINT);

        if (objects_to_remove.size() < 2) {
            objects_to_remove.clear();
        }

        return objects_to_remove;
    }

    void  GameField::_checkNeighbors(Color color, int x, int y, std::vector<GameObject*>& objects_to_remove, Direction dir) {
        if (dir != RIGHT && x < rows.size() && getObject(x + 1, y).color == color) {
            GameObject* obj = &getObject(x + 1, y);
            if (std::find(objects_to_remove.begin(), objects_to_remove.end(), obj) == objects_to_remove.end()) {
                if (obj->shapeType != BOMB && obj->shapeType != FILL) {
                    objects_to_remove.push_back(obj);
                    _checkNeighbors(color, x + 1, y, objects_to_remove, LEFT);
                }
            }
        }

        auto it = std::next(rows.begin(), x - 1);
        if (dir != UP && y < it->size() && getObject(x, y + 1).color == color) {
            GameObject* obj = &getObject(x, y + 1);
            if (std::find(objects_to_remove.begin(), objects_to_remove.end(), obj) == objects_to_remove.end()) {
                if (obj->shapeType != BOMB && obj->shapeType != FILL) {
                    objects_to_remove.push_back(obj);
                    _checkNeighbors(color, x, y + 1, objects_to_remove, DOWN);
                }
            }
        }

        if (dir != LEFT && x > 1 && getObject(x - 1, y).color == color) {
            GameObject* obj = &getObject(x - 1, y);
            if (std::find(objects_to_remove.begin(), objects_to_remove.end(), obj) == objects_to_remove.end()) {
                if (obj->shapeType != BOMB && obj->shapeType != FILL) {
                    objects_to_remove.push_back(obj);
                    _checkNeighbors(color, x - 1, y, objects_to_remove, RIGHT);
                }
            }
        }

        if (dir != DOWN && 1 < y && getObject(x, y - 1).color == color) {
            GameObject* obj = &getObject(x, y - 1);
            if (std::find(objects_to_remove.begin(), objects_to_remove.end(), obj) == objects_to_remove.end()) {
                if (obj->shapeType != BOMB && obj->shapeType != FILL) {
                    objects_to_remove.push_back(obj);
                    _checkNeighbors(color, x, y - 1, objects_to_remove, UP);
                }
            }
        }
    }

    void GameField::_deleteGameObject(GameObject& obj) {
        auto row_it = rows.begin();
        std::advance(row_it, obj.x - 1);
        if (row_it == rows.end()) {
            return;
        }

        auto obj_it = row_it->begin();
        std::advance(obj_it, obj.y - 1);
        if (obj_it == row_it->end()) {
            return;
        }

        int x = obj.x;
        int y = obj.y;

        row_it->erase(obj_it); 

        if (row_it->empty()) {
            _updateGameObjectsX(row_it, x);

            row_it = rows.begin();
            std::advance(row_it, x - 1);
            rows.erase(row_it);
        }
        else {
            _updateGameObjectsY(*row_it, y);
        }
    }

    void  GameField::_deleteGameObject(std::vector<GameObject*>& objects_to_remove) {
        if (!_start_flag) {
            if (!objects_to_remove.empty()) {
                animate->animateObjectMovement(objects_to_remove);
            }
        }

        std::sort(objects_to_remove.begin(), objects_to_remove.end(),
            [](const GameObject* a, const GameObject* b) {
                if (a->x == b->x) {
                    return a->y > b->y;
                }
                else {
                    return a->x > b->x;
                }
            });

        objects_to_remove.erase(std::unique(objects_to_remove.begin(), objects_to_remove.end(),
            [](const GameObject* a, const GameObject* b) {
                return a->x == b->x && a->y == b->y;
            }), objects_to_remove.end());


        std::list<GameObject*> bonus_squares;
        if (!_start_flag) {
            for (const auto& obj : objects_to_remove) {
                std::list<GameObject*> non_empty_squares = _getNonEmptySquaresInRadius(obj, 3);
                _SetRandomShapeType(non_empty_squares);
                bonus_squares.insert(bonus_squares.end(), non_empty_squares.begin(), non_empty_squares.end());
            }

            bonus_squares.sort([](const GameObject* a, const GameObject* b) {
                if (a->x == b->x) {
                    return a->y < b->y;
                }
                return a->x < b->x;
                });

            bonus_squares.erase(std::unique(bonus_squares.begin(), bonus_squares.end(),
                [](const GameObject* a, const GameObject* b) {
                    return a->x == b->x && a->y == b->y;
                }), bonus_squares.end());

            bonus_squares.remove_if([&objects_to_remove](const GameObject* obj) {
                return std::find(objects_to_remove.begin(), objects_to_remove.end(), obj) != objects_to_remove.end();
                });
        }

        for (const auto& obj : objects_to_remove) {
            _deleteGameObject(*obj);
        }

    }

    void  GameField::_SetRandomShapeType(std::list<GameObject*>& non_empty_squares) {
        if (!non_empty_squares.empty()) {
            static bool initialized = false;
            if (!initialized) {
                srand(time(0));
                initialized = true;
            }

            int random_index = rand() % non_empty_squares.size();

            auto it = non_empty_squares.begin();
            std::advance(it, random_index);

            int random_choice = rand() % 2;
            if ((*it)->shapeType != BOMB && (*it)->shapeType != FILL) {
                if (random_choice == 0) {
                    (*it)->shapeType = FILL;
                }
                else {
                    (*it)->shapeType = BOMB;
                }
            }
        }
    }

    void  GameField::_updateGameObjectsY(std::list<GameObject>& row, int start_y) {
        for (auto& game_obj : row) {
            if (game_obj.y > start_y) {
                game_obj.y--;
            }
        }
    }

    void  GameField::_updateGameObjectsX(std::list<std::list<GameObject>>::iterator& row, int start_x) {
        while (row != rows.end()) {
            for (auto& game_obj : *row) {
                game_obj.x--;
            }
            ++row;
        }
    }

    std::list<GameObject*>  GameField::_getNonEmptySquaresInRadius(GameObject* obj, int radius) {
        std::list<GameObject*> non_empty_squares;
        auto row = rows.begin();
        for (int i = 0; i < obj->x - radius && i < rows.size(); i++) {
            ++row;
        }

        auto row_end = rows.begin();
        std::advance(row_end, rows.size() - 1);

        std::vector<std::pair<int, int>> potential_points;
        for (int i = -radius; i <= radius; ++i) {
            auto& rowList = *row;
            if (obj->x + i >= 1 && obj->x + i <= rows.size()) {
                for (int j = -radius; j <= radius; ++j) {
                    if ((i * i + j * j <= radius * radius) && !(j == 0 && i == 0) &&
                        (obj->y + j <= rowList.size()) && (obj->y + j >= 1)) {
                        non_empty_squares.push_back(&getObject(obj->x + i, obj->y + j));
                    }
                }
                if (row == row_end) {
                    potential_points.clear();
                    return non_empty_squares;
                }
                ++row;
            }
        }
        potential_points.clear();
        return non_empty_squares;
    }

    void  GameField::_shuffle(std::vector<std::pair<int, int>>& vec) {
        srand(time(0));
        for (std::size_t i = vec.size() - 1; i > 0; --i) {
            std::size_t j = std::rand() % (i + 1);
            std::swap(vec[i], vec[j]);
        }
    }

    bool  GameField::_hasThreeOfSameColor() {
        for (const auto& row : rows) {
            std::map<Color, int> colorCount;
            for (const auto& obj : row) {
                colorCount[obj.color]++;
                if (colorCount[obj.color] == 2) {
                    return true;
                }
            }
        }
        return false;
    }

    bool  GameField::_hasNotSameColor(GameObject& obj) {
        int count = 0;
        for (const auto& row : rows) {
            for (const auto& game_obj : row) {
                if (game_obj.shapeType == _type && game_obj.color != obj.color &&
                    !((abs(obj.x - game_obj.x) < 1 && abs(obj.y - game_obj.y) <= 1) ||
                        (abs(obj.x - game_obj.x) <= 1 && abs(obj.y - game_obj.y) < 1))) {
                    count++;
                    if (count > 1) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    std::vector<GameObject*>  GameField::_getRandomObject(int count) {
        std::vector<GameObject*> objects_to_remove;
        std::vector<std::pair<int, int>> all_objects;

        int i = 0;
        for (auto& row : rows) {
            i++;
            for (int j = 1; j <= row.size(); j++) {
                all_objects.push_back(std::make_pair(i, j));
            }
        }

        if (!all_objects.empty()) {
            _shuffle(all_objects);
        }

        for (size_t i = 0; i < count && i < all_objects.size(); ++i) {
            objects_to_remove.push_back(&getObject(all_objects[i].first, all_objects[i].second));
            if (objects_to_remove[i]->shapeType == BOMB) {
                count_bomb++;
            }
        }
        all_objects.clear();

        return objects_to_remove;
    }


    GameField::GameField(int x, int y, ShapeType type_) : _type(type_) {
        x_parts = x;
        y_parts = y;
        GenerateField();
    }

    void  GameField::GenerateField() {
        _start_flag = true;
        srand(time(0));
        int colors = std::ranges::min(x_parts, y_parts) - std::ranges::min(x_parts, y_parts) / 3;
        do {
            while (rows.size() < x_parts) {
                rows.push_back({});
            }
            for (auto it = rows.begin(); it != rows.end(); ++it) {
                int x = std::distance(rows.begin(), it) + 1;
                while (it->size() < y_parts) {
                    it->push_back({ x, (int)it->size() + 1, static_cast<Color>(rand() % colors), _type });
                }
            }
        } while (!removeSameColorObjects());
        _start_flag = false;
        count_bomb = 0;
    }

    GameObject& GameField::getObject(int x, int y) {

        static GameObject defaultObject{ 0, 0, WHITE, RHOMBUS };
        if (x < 1 || y < 1 || x>rows.size()) {
            return defaultObject;
        }
        auto rowIter = std::next(rows.begin(), x - 1);
        if (rowIter->size() < y) {
            return defaultObject;
        }
        auto objIter = std::find_if(rowIter->begin(), rowIter->end(), [&](const GameObject& obj) {
            return obj.x == x && obj.y == y;
            });
        return *objIter;
    }

    bool  GameField::swapObjects(int x_index, int y_index, int x_index_last, int y_index_last) {
        auto& obj1 = getObject(x_index, y_index);
        auto& obj2 = getObject(x_index_last, y_index_last);

        if (animate != nullptr) {
            auto obj1_copy = obj1;
            auto obj2_copy = obj2;
            animate->swapObject(obj1_copy, obj2_copy);
        }


        std::swap(obj1.color, obj2.color);
        std::swap(obj1.shapeType, obj2.shapeType);

        if (obj1.shapeType == BOMB) {
            _deleteGameObject(obj1);
            count_bomb++;
        }

        if (obj2.shapeType == BOMB) {
            _deleteGameObject(obj2);
            count_bomb++;
        }

        removeSameColorObjects();
        return true;
    }

    bool  GameField::removeSameColorObjects() {
        std::vector<GameObject*> objects_to_remove;

        for (auto& row : rows) {
            for (auto& obj : row) {
                if (std::find(objects_to_remove.begin(), objects_to_remove.end(), &obj) == objects_to_remove.end()) {
                    std::vector<GameObject*> to_remove = _getObjectsToRemove(obj);

                    if (to_remove.size() >= 2) {
                        objects_to_remove.push_back(&obj);

                        objects_to_remove.insert(objects_to_remove.end(), to_remove.begin(), to_remove.end());
                    }
                }
            }
        }

        _deleteGameObject(objects_to_remove);
        bool flag = objects_to_remove.empty();
        objects_to_remove.clear();

        return flag;
    }

    void  GameField::Fill(GameObject& obj) {
        if (fill.empty()) {
            if (_hasNotSameColor(obj)) {
                obj.shapeType = _type;
                fill.push_back(obj);
            }
            else {
                return;
            }
        }
        else {
            if (obj.shapeType != FILL && obj.shapeType != BOMB) {
                if (!((abs(obj.x - fill.begin()->x) < 1 && abs(obj.y - fill.begin()->y) <= 1) ||
                    (abs(obj.x - fill.begin()->x) <= 1 && abs(obj.y - fill.begin()->y) < 1)))
                {
                    obj.color = fill.begin()->color;
                    fill.push_back(obj);
                }
            }
            if (fill.size() >= 3) {
                fill.clear();
                Update();
            }
        }
    }

    void  GameField::Update() {
        if (count_bomb > 0) {
            count_bomb--;
            std::vector<GameObject*> objects_to_remove = _getRandomObject(4);
            _deleteGameObject(objects_to_remove);
        }
        else if (_hasThreeOfSameColor()) {
            removeSameColorObjects();
        }
    }