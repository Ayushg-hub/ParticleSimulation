#pragma once
#include"UserIface.h"

class Renderer
{
	UserIface* UI;
	GLFWwindow* window;

public:

	Renderer();

	void Start();

	~Renderer();

};

