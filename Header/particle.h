#pragma once
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"parameters.h"

struct Collision
{
	bool occur = false;
	glm::vec3 m_velocity = glm::vec3(0.0f, 0.0f, 1.0f);
};

struct particle
{
	bool injected{ false };
	unsigned int m_radius{CIRCLESIZE};
	glm::vec3 m_position = glm::vec3(2.0f, 2.0f, 1.0f);
	glm::vec3 m_velocity = glm::vec3(0.1f, -0.5f, 0.0f);
	glm::vec3 m_accelaration = glm::vec3(0.0f, -0.5f, 0.0f);
	float timestep{0.025f};
	float mass{ 1 };
	Collision collision;
};



