#pragma once
#include<glew/include/GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

enum class mouseModes
{
    NONE = 0,
    PRESSED = 1,
    RELEASED = 2
};

struct mousePos
{
    double x;
    double y;
};

class Mouse
{
private:
    mouseModes mouseMode;
    mousePos pos;
    

public:
    unsigned int pressCount;

    Mouse(Mouse&) = delete;
    void operator=(Mouse&) = delete;

    static Mouse& getInstance();
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    mousePos getMousePosition() { return pos; }
    mouseModes getMouseMode() { return mouseMode; }
    unsigned int getPressCount() { return pressCount; }
    void update(GLFWwindow* window);

private:
    Mouse() :mouseMode(mouseModes::RELEASED), pressCount(0)
    {
        pos = { 0.0,0.0 };
    }


};

//void handleMouseInputEvent(GLFWwindow* window,UserIface** UI)
//{
//    double xPos{ 0 }, yPos{ 0 };
//    glfwGetCursorPos(window, &xPos, &yPos);
//    //std::cout << "(" << xPos << "," << yPos << ")" << std::endl;
//    switch ((*UI)->getIfaceType())
//    {
//    case IfaceType::SIMULATION:
//        {
//            if (mouseMode == mouseModes::PRESSED && count % 5 == 0)
//            {
//                //setting the right coordinate system
//                xPos = xPos - SCREEN_WIDTH / 2;
//                yPos = -(yPos - SCREEN_HEIGHT / 2);
//
//                //normalizing
//                xPos = xPos / (float)(SCREEN_WIDTH / 2);
//                yPos = yPos / (float)(SCREEN_HEIGHT / 2);
//
//                ParticleSimulation* P = static_cast<ParticleSimulation*>(*UI);
//                P->injectParticles(xPos, yPos);
//                std::cout << "CURRENTLY PRESSED" << std::endl;
//            }
//            count++;
//        }
//        break;
//
//    case IfaceType::HOME:
//        {
//            if (mouseMode == mouseModes::PRESSED)
//            {
//                //setting the right coordinate system
//                xPos = xPos - SCREEN_WIDTH / 2;
//                yPos = -(yPos - SCREEN_HEIGHT / 2);
//
//                //normalizing
//                xPos = xPos / (float)(SCREEN_WIDTH / 2);
//                yPos = yPos / (float)(SCREEN_HEIGHT / 2);
//
//                if (xPos >= -0.8f && xPos <= -0.4f && yPos >= -0.2f && yPos <= 0.2f)
//                {
//                    delete (*UI);
//                    (*UI) = new ParticleSimulation();
//                    //HomeIface* P = static_cast<HomeIface*>(UI);
//                }
//            }
//        }
//    }
//
//}

//just a test function
//void handleMouseInputEvent(GLFWwindow* window)
//{
//    double xPos{ 0 }, yPos{ 0 };
//    glfwGetCursorPos(window, &xPos, &yPos);
//    if (mouseMode == mouseModes::PRESSED)
//    {
//
//        std::cout << "PRESSED" << std::endl;
//    }
//}