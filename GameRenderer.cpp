#include "GameRenderer.h"

    void  GameRenderer::_displayFieldClick() {
        if (_click_on_fild)
            drawSquareLines(x_index, y_index);
    }

    void  GameRenderer::_FieldClick() {
        if (x_index == x_parts + 1 && y_index == y_parts + 2) {
            rows.clear();
            GenerateField();
        }
        else if (x_index >= 1 && x_index <= rows.size()) {
            auto row_it = rows.begin();
            std::advance(row_it, x_index - 1);
            if (y_index >= 1 && y_index <= row_it->size()) {
                auto obj_it = row_it->begin();
                std::advance(obj_it, y_index - 1);
                _click_on_fild = true;
                if (obj_it->shapeType == FILL) {
                    Fill((*obj_it));
                }
                else if (!fill.empty()) {
                    Fill((*obj_it));
                    _click_on_fild = false;
                }
                else if (((abs(x_index - x_index_last) < 1 && abs(y_index - y_index_last) <= 1) ||
                    (abs(x_index - x_index_last) <= 1 && abs(y_index - y_index_last) < 1))) {
                    interface->swapObjects(x_index, y_index, x_index_last, y_index_last);
                    x_index_last = 0; y_index_last = 0;
                }
                else {
                    x_index_last = x_index;
                    y_index_last = y_index;
                }
            }
        }
        else
            _click_on_fild = false;
    }

    void  GameRenderer::_updateOldField() {
        std::vector<GameObject*> objects_to_remove;
        if (!rows.empty()) {
            for (auto& data : _move_queue) {
                if (data.y_new > 0 && data.x_new > 0 && data.x_new < rows.size()) {
                    GameObject& new_obj = getObject(data.x_new, data.y_new);
                    if (&new_obj != nullptr && &new_obj != &data.obj && new_obj.y > 0) {
                        objects_to_remove.push_back(&new_obj);
                    }
                }
            }
        }
        _remove_objects(objects_to_remove);
    }

    void GameRenderer::_remove_objects(std::vector<GameObject*>& objects_to_remove) {
        _rows_copy = rows;
        for (auto obj_ptr : objects_to_remove) {
            int x = obj_ptr->x;
            int y = obj_ptr->y;

            auto rowIter = std::next(_rows_copy.begin(), x - 1);
            auto objIter = std::find_if(rowIter->begin(), rowIter->end(), [&](const GameObject& obj_copy) {
                return obj_copy.x == x && obj_copy.y == y;
                });

            if (objIter != rowIter->end()) {
                objIter = rowIter->erase(objIter);
            }
        }
    }

    GameRenderer::GameRenderer(GLuint VBO_, GLuint VAO_, int x_parts, int y_parts, ShapeType type_, GLint vertexColorLocation_)
        : Renderer(VBO_, VAO_, x_parts, y_parts, vertexColorLocation_), GameField(x_parts, y_parts, type_) {
        interface = this;
        animate.reset(this);
    }

    void  GameRenderer::drawField() {
        for (auto& row : rows) {
            for (const auto& obj : row) {
                drawObject(obj);
            }
        }
    }

    void  GameRenderer::drawOldField() {
        for (auto& row : _rows_copy) {
            for (const auto& obj : row) {
                drawObject(obj);
            }
        }
    }

    void  GameRenderer::drawSwap() {
        for (auto& data : _swap_queue) {
            Animate(data.obj1, data.obj2, true);
        }
        if (animate_step == steps || animate_step == 0) {
            _swap_queue.clear();
            Update();
        }
    }

    void  GameRenderer::drawMove() {
        if (animate_step == steps || animate_step == 0) {
            _updateOldField();
        }
        for (auto& data : _move_queue) {
            Animate(data.obj, data.x_new, data.y_new, true);
        }
        animate_step -= _move_queue.size() - 2;
        if (animate_step >= steps - _move_queue.size() || animate_step <= -2) {
            _move_queue.clear();
            Update();
            animate_step = 0;
        }
    }

    void  GameRenderer::GEMS() {

        if (!_swap_queue.empty()) {
            drawOldField();
            drawSwap();
        }
        else if (!_move_queue.empty())
        {
            drawOldField();
            drawMove();
        }
        else {
            drawField();
        }

        drawObject(x_parts + 1, y_parts + 2, RETURN);
        drawObject(x_parts / 2, y_parts + 1, GEMStext);
        glUniform4f(vertexColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
        drawBorderLines();
        drawGrid(x_parts, y_parts);

        _displayFieldClick();
    }

    void  GameRenderer::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            x_index = (int)(xpos / (WIDTH / (x_parts + 2)));
            y_index = (int)(ypos / (HEIGHT / (y_parts + 3)));
            y_index = y_parts + 2 - y_index;

            interface->_FieldClick();

        }
    }

    void  GameRenderer::swapObject(GameObject& obj1, GameObject& obj2) {
        _swap_queue.push_back({ obj1, obj2 });
        std::vector<GameObject*> objects_to_remove{ &obj1, &obj2 };
        _remove_objects(objects_to_remove);
    }

    void  GameRenderer::animateObjectMovement(std::vector<GameObject*>& objects_to_remove) {

        if (objects_to_remove.size() == 1) {
            return;
        }

        std::sort(objects_to_remove.begin(), objects_to_remove.end(),
            [](const GameObject* a, const GameObject* b) {
                if (a->x == b->x) {
                    return a->y < b->y;
                }
                else {
                    return a->x < b->x;
                }
            });

        int deltaX = 0;
        auto row = rows.begin();
        auto remove_obj = objects_to_remove.begin();
        auto remove_end = objects_to_remove.begin();
        std::advance(remove_end, objects_to_remove.size() - 1);

        GameObject* remove = *remove_obj;
        while (true) {
            int deltaY = 0;
            for (auto& game_obj : *row) {
                if (remove_obj != remove_end && *remove == game_obj) {
                    deltaY++;
                    ++remove_obj;
                    remove = *remove_obj;
                }
                else if ((deltaY != 0 || deltaX != 0) && (game_obj.x - deltaX > 0 || game_obj.y - deltaY > 0)) {
                    _move_queue.push_back({ game_obj, game_obj.x - deltaX, game_obj.y - deltaY });
                }
                if (deltaY == std::distance(row->begin(), row->end())) {
                    deltaX++;
                }
                else if (remove_obj == remove_end) {
                    return;
                }
            }
            if (remove_obj != objects_to_remove.end()) {
                while (remove->x - 1 > std::distance(rows.begin(), row)) {
                    ++row;
                }
            }
            else
                return;
        }
    }