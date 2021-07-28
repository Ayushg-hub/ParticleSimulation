#pragma once
#include<glew/include/GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

enum class IfaceType
{
	SIMULATION,
	HOME,
	OTHER
};

class UserIface
{
protected:
	struct IDs
	{
		GLuint VBO;
		GLuint instance_VBO;
		GLuint IBO;
		GLuint VAO;
		GLuint Texture;
		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint shaderProgram;
	}ID;

public:
	
	virtual void InputEventHandler() = 0;
	virtual void UpdateVertexBuffers() = 0;
	virtual void render() = 0;
	virtual IfaceType getIfaceType() = 0;
	virtual std::string getType() = 0;

	//virtual float* GetVertexBuffersPtr() = 0;
	//virtual GLuint* GetIndexBufferPtr() = 0;
};

