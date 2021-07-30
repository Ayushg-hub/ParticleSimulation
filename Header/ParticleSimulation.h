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
	const std::string Type = "PARTICLE";
	ObjectManager objMgr;

public:
	ParticleSimulation();
	ParticleSimulation(const ParticleSimulation&) = delete;
	ParticleSimulation operator=(const ParticleSimulation&) = delete;

	void InputEventHandler() override;
	void UpdateVertexBuffers() override;
	void render() override;
	std::string getType() override { return Type; }

	void renderUI() override;
	void renderSIM() override;

	void injectParticles(float x, float y);


	~ParticleSimulation();
};

