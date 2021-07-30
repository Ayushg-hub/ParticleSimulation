#pragma once
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"parameters.h"
#include"glew/include/GL/glew.h"

struct ObjData
{
	bool collide{false};
	glm::vec3 m_position = glm::vec3(2.0f, 2.0f, 1.0f);
	glm::vec3 m_velocity = glm::vec3(0.1f, -0.5f, 0.0f);
	glm::vec3 m_accelaration = glm::vec3(0.0f, -0.5f, 0.0f);
	float mass{ 1 };
};

enum class circleType
{
	NORMAL = 0,
	NORMAL_STATIONARY = 1,
	GRAVITY = 2,
	GRAVITY_STATIONARY = 3
};

struct CircleData
{
    circleType type{ circleType::NORMAL };
	unsigned int m_radius{ CIRCLESIZE };
	ObjData data;
};

enum class Shape
{
	CIRCLE=0,
    NONE
};

class Objects
{
public:
	virtual void draw() = 0;
	virtual Shape getShape() = 0;
	virtual void* getInfo() = 0;
	virtual unsigned int getCount() = 0;
	virtual void  injectParticles(float x,float y) = 0;
};

