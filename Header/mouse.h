#pragma once
#include<glew/include/GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include"parameters.h"
#include"ParticleSimulation.h"

enum class mouseModes
{
    NONE = 0,
    PRESSED = 1,
    RELEASED = 2
}mouseMode;

unsigned int count = 0;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        std::cout << "MOUSE BUTTON PRESSED !!!" << std::endl;
        mouseMode = mouseModes::PRESSED;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        std::cout << "MOUSE BUTTON RELEASED" << std::endl;
        mouseMode = mouseModes::RELEASED;
        count = 0;
    }
    else
    {
        std::cout << "WRONG BUTTON" << std::endl;
    }
}

void handleMouseInputEvent(GLFWwindow* window,UserIface* UI)
{
    double xPos{ 0 }, yPos{ 0 };
    glfwGetCursorPos(window, &xPos, &yPos);
    //std::cout << "(" << xPos << "," << yPos << ")" << std::endl;
    switch (UI->getIfaceType())
    {
    case IfaceType::SIMULATION:
        {
            if (mouseMode == mouseModes::PRESSED && count % 5 == 0)
            {
                //setting the right coordinate system
                xPos = xPos - SCREEN_WIDTH / 2;
                yPos = -(yPos - SCREEN_HEIGHT / 2);

                //normalizing
                xPos = xPos / (float)(SCREEN_WIDTH / 2);
                yPos = yPos / (float)(SCREEN_HEIGHT / 2);

                ParticleSimulation* P = static_cast<ParticleSimulation*>(UI);
                P->injectParticles(xPos, yPos);
                std::cout << "CURRENTLY PRESSED" << std::endl;
            }
            count++;
        }
        break;
    }

}

//just a test function
void handleMouseInputEvent(GLFWwindow* window)
{
    double xPos{ 0 }, yPos{ 0 };
    glfwGetCursorPos(window, &xPos, &yPos);
    if (mouseMode == mouseModes::PRESSED)
    {

        std::cout << "PRESSED" << std::endl;
    }
}