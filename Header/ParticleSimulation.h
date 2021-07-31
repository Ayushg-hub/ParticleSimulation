#pragma once
#include<glew/include/GL/glew.h>
#include<GLFW/glfw3.h>
#include"SimulationIface.h"
#include"parameters.h"
#include<iostream>
#include"ObjectManager.h"

class ParticleSimulation:public SimulationIface
{
	const std::string Type = "PARTICLE";
	ObjectManager objMgr;
	IDs CircleID;

	unsigned int circleSizes[4];
	float barPositions[4];
	
	glm::mat4 transform;
	unsigned int transformLoc;//location of the transform matrix in the shader

public:
	ParticleSimulation();
	ParticleSimulation(const ParticleSimulation&) = delete;
	ParticleSimulation operator=(const ParticleSimulation&) = delete;

	UserIface* InputEventHandler() override;
	void UpdateVertexBuffers() override;
	void render() override;
	std::string getType() override { return Type; }

	void renderUI() override;
	void renderSIM() override;

	void injectParticles(float x, float y);


	~ParticleSimulation();
};

