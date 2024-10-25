#include "Window.h"

void initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void initGLEW() {
    glewExperimental = GL_TRUE;
    glewInit();
}

void initOpenGL(GLFWwindow*& window) {
    window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    initGLEW();

    glViewport(0, 0, WIDTH, HEIGHT);
}

void cleanup(GLuint& VAO, GLuint& VBO) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
}