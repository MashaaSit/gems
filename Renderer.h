#ifndef RENDERER_H
#define RENDERER_H


#include "Shader.h"
#include <GL/glew.h>
#include <functional>
#include "IRenderer.h"
#include "GameObjects.h"
#include <vector>
#include <map>


using DrawFunction = std::function<void(GLfloat, GLfloat)>;
using ShapeMap = std::map<ShapeType, DrawFunction>;
using ColorMap = std::map<Color, std::function<void()>>;

class Renderer {
public:
    GLfloat steps = 10;
    GLfloat animate_step;
    GLint vertexColorLocation;
    Renderer(GLuint VBO, GLuint VAO, int x, int y);
    Renderer(GLuint VBO, GLuint VAO, int x, int y, GLint vertexColorLocation);
    void drawBorderLines();
    void drawGrid(int x, int y);
    void drawSquareLines(int x, int y);
    void drawObject(GameObject obj);
    void drawObject(int x, int y, ShapeType shapeType);
    void drawObject(int x, int y, ShapeType shapeType, GLint vertexColorLocation);
    void drawObject(int x, int y, ShapeType shapeType, Color color);
    void Animate(GameObject obj, int x_new, int y_new, bool flag);
    void Animate(GameObject obj1, GameObject obj2, bool flag);
    void Animate(int x, int y, int x_new, int y_new, bool flag, ShapeType shapeType);
    void Animate(int x, int y, int x_new, int y_new, bool flag, ShapeType shapeType1, ShapeType shapeType2);
    void Animate(int x, int y, int x_new, int y_new, bool flag, ShapeType shapeType, Color color);
    void Animate(int x, int y, int x_new, int y_new, bool flag, ShapeType shapeType1, Color color1, ShapeType shapeType2, Color color2);
private:
    GLuint VBO, VAO;
    GLfloat dx, dy;
    void init3(GLfloat* vertices, GLfloat size);
    void draw_line(int size);
    void _drawReule(GLfloat x, GLfloat y);
    void _drawSquareLines(GLfloat x, GLfloat y);
    void _drawEllipse(GLfloat x, GLfloat y);
    void _drawTriangle(GLfloat x, GLfloat y);
    void _drawRhombus(GLfloat x, GLfloat y);
    void _drawSquare(GLfloat x, GLfloat y);
    void _drawBomb(GLfloat x, GLfloat y);
    void _drawFill(GLfloat x, GLfloat y);
    void _drawReturn(GLfloat x, GLfloat y);
    void _drawGemsText(GLfloat x, GLfloat y);
    ShapeMap createShapeMap();
    ColorMap createColorMap();
    void _AnimateDrawObject(int x, int y, int x_new, int y_new, bool flag, std::function<void(GLfloat, GLfloat)> draw_object);
};

#endif