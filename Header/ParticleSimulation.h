#pragma once
#include<glew/include/GL/glew.h>
#include<GLFW/glfw3.h>
#include"SimulationIface.h"
#include"parameters.h"
#include"particle.h"
#include<iostream>
#include"ObjectManager.h"

class ParticleSimulation:public SimulationIface
{
	const char* vertexshader =
		"#version 330 core\n"
		"layout(location = 0) in vec2 position;\n"
		"layout(location = 1) in vec2 offset;\n"
		"void main(){\n"
		"gl_Position = vec4(position + offset,1,1);\n"
		"}\0\n";

	const char* fragmentshader =
		"#version 330 core\n"
		"out vec4 color;\n"
		"void main(){\n"
		"color = vec4(1.0f , 0.5f , 0.2f , 1.0f);\n"
		"}\0\n";

	const std::string Type = "PARTICLE";

	particle particles[MAX_PARTICLES];
	float positions[2 * MAX_PARTICLES]{};
	/*float vertexBuffer[MAX_PARTICLES * ((NOOFTRIANGLES + 1) * 2)];
	GLuint indexBuffer[MAX_PARTICLES * (NOOFTRIANGLES * 3)];*/

	float vertexBuffer[(NOOFTRIANGLES + 1) * 2];
	GLuint indexBuffer[(NOOFTRIANGLES * 3)];

	float adjacanyMatrix[MAX_PARTICLES][MAX_PARTICLES]{};

	ObjectManager objMgr;

	//char* vertexshader;
	//char* fragmentshader;

	unsigned int count{ 0 };

public:
	ParticleSimulation();
	ParticleSimulation(const ParticleSimulation&) = delete;
	ParticleSimulation operator=(const ParticleSimulation&) = delete;

	void InputEventHandler() override;
	void UpdateVertexBuffers() override;
	void render() override;
	std::string getType() override { return Type; }
	//float* GetVertexBuffersPtr() override;
	//GLuint* GetIndexBufferPtr() override;

	void calcPosition() override;
	void calcVelocity() override;
	void calcAccelaration() override;

	void calcAdjacencyMatrix();
	void collisionCheck();
	void updateVBPositions(glm::vec3 dist, unsigned int pos);
	void injectParticles(float x, float y);


	~ParticleSimulation();
};

