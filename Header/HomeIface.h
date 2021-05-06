#pragma once
#include"UserIface.h"
class HomeIface:public UserIface
{
	float vertexBuffer[12*2] = {
		-0.8f, -0.2f , -0.8f, 0.2f , -0.4f , 0.2f , -0.4f , -0.2f,
		-0.2f, -0.2f , -0.2f, 0.2f ,  0.2f , 0.2f ,  0.2f , -0.2f,
		 0.4f, -0.2f ,  0.4f, 0.2f ,  0.8f , 0.2f ,  0.8f , -0.2f
	};

	GLuint indexBuffer[18] = {
		0 ,1 ,2 , 0, 2 ,3 ,
		4 ,5 ,6 , 4, 6 ,7 ,
		8 ,9 ,10, 8, 10,11
	};

	const char* vertexshader =
		"#version 330 core\n"
		"layout(location = 0) in vec4 position;\n"
		"void main(){\n"
		"gl_Position = position;\n"
		"}\0\n";

	const char* fragmentshader =
		"#version 330 core\n"
		"out vec4 color;\n"
		"void main(){\n"
		"color = vec4(1.0f , 0.5f , 0.2f , 1.0f);\n"
		"}\0\n";

	const IfaceType IFType = IfaceType::HOME;

public:

	HomeIface();
    void InputEventHandler() override;
	void UpdateVertexBuffers() override;
	void render() override;
	IfaceType getIfaceType() override;
	std::string getType() override;
	~HomeIface();
};

