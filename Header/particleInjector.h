//#pragma once
//#include<iostream>
//#include<glew/include/GL/glew.h>
//#include<GLFW/glfw3.h>
//#include"particle.h"
//#include"parameters.h"
//#include"error_check.h"
//
//class particleInjector
//{
//	particle m_particles[MAX_PARTICLES];
//	float vertexBuffer[MAX_PARTICLES * ((NOOFTRIANGLES + 1)*2)];
//	GLuint indexBuffer[MAX_PARTICLES * (NOOFTRIANGLES * 3)];
//	struct IDs
//	{
//		GLuint VBO;
//		GLuint IBO;
//		GLuint VAO;
//		GLuint vertexShader;
//		GLuint fragmentShader;
//		GLuint shaderProgram;
//	}ID;
//	GLFWwindow* m_window;
//	unsigned int m_count{0};
//
//public:
//
//	void init();
//	~particleInjector();
//	particle* getParticlePointer() { return m_particles; }
//	unsigned int getParticleCount() { return m_count; }
//	float* getVertexBufferData() { return vertexBuffer; }
//	void injectParticles(float x, float y);
//	void renderParticles();
//
//	//later abstraction
//	//void createVertexBuffer();
//	//void createIndexBuffer();
//	//void createVertexArrayobject();
//};

