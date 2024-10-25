#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

enum ShapeType
{
    REULE,
    SQUARE,
    TRIANGLE,
    RHOMBUS,
    BOMB,
    FILL,
    ELLIPSE,
    RETURN,
    GEMStext
};

enum Color
{
    RED,
    GREEN,
    BLUE,
    YELLOW,
    MAGENTA,
    CYAN,
    BROWN,
    GRAY,
    PINK,
    ORANGE,
    BLACK,
    WHITE
};

struct GameObject
{
    int x;
    int y;
    Color color;
    ShapeType shapeType;

    bool operator==(const GameObject& other) const
    {
        return (x == other.x && y == other.y);
    }
};

#endif