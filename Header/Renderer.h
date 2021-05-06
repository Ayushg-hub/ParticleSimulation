#pragma once
#include"HomeIface.h"

class Renderer
{
	UserIface* UI;
	GLFWwindow* window;

public:

	Renderer();

	void Start();

	~Renderer();

};

