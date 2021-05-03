#pragma once
#include<glew/include/GL/glew.h>
#include<GLFW/glfw3.h>
#include"SimulationIface.h"
#include"parameters.h"
#include"particle.h"
#include<iostream>

class ParticleSimulation:public SimulationIface
{
	const std::string Type = "PARTICLE";

	particle particles[MAX_PARTICLES];
	float vertexBuffer[MAX_PARTICLES * ((NOOFTRIANGLES + 1) * 2)];
	GLuint indexBuffer[MAX_PARTICLES * (NOOFTRIANGLES * 3)];

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

	void collisionCheck();
	void updateVBPositions(glm::vec3 dist, unsigned int pos);
	void injectParticles(float x, float y);


	~ParticleSimulation();
};

