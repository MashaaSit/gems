#include <iostream>
#include "GameRenderer.h"


void gameLoop(GLFWwindow* window, GameRenderer& ren, Shader& ourShader) {
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.Use();
        ren.GEMS();

        glfwSwapBuffers(window);
    }
}

int main()
{
    initGLFW();

    GLFWwindow* window;
    initOpenGL(window);

    Shader ourShader("/home/masha/CLionProjects/допса/gems/shaders/default.vs", "/home/masha/CLionProjects/допса/gems/shaders/default.frag");
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    GLint vertexColorLocation = glGetUniformLocation(ourShader.Program, "ourColor");

    GameRenderer ren = GameRenderer(VBO, VAO, (GLfloat)x_parts, (GLfloat)y_parts, SQUARE, vertexColorLocation);

    glfwSetMouseButtonCallback(window, ren.mouse_button_callback);

    gameLoop(window, ren, ourShader);

    cleanup(VAO, VBO);

    return 0;
}