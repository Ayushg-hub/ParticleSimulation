#include"../Header/mouse.h"

Mouse& Mouse::getInstance()
{
    static Mouse instance;

    return instance;
}

void Mouse::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Mouse& mouse = Mouse::getInstance();

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        std::cout << "MOUSE BUTTON PRESSED !!!" << std::endl;
        mouse.mouseMode = mouseModes::PRESSED;
        mouse.pressCount++;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        std::cout << "MOUSE BUTTON RELEASED" << std::endl;
        mouse.mouseMode = mouseModes::RELEASED;
        mouse.pressCount = 0;
    }
    else
    {
        std::cout << "WRONG BUTTON" << std::endl;
    }
}

void Mouse::update(GLFWwindow* window)
{
    glfwGetCursorPos(window, &pos.x, &pos.y);

    if (mouseMode == mouseModes::PRESSED)
    {
        pressCount++;
    }
}