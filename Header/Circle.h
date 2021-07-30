#pragma once
#include"Objects.h"
#include"common.h"

class Circles: public Objects
{
	IDs ID;
	unsigned int count;
	CircleData circles[MAX_CIRCLES];
	float positions[2 * MAX_CIRCLES]{};//instance buffer
	float vertexBuffer[(NOOFTRIANGLES + 1) * 2];
	GLuint indexBuffer[(NOOFTRIANGLES * 3)];

public:
	Circles();

	void draw() override;
	Shape getShape() override;
	void* getInfo() override;
	unsigned int getCount() override;
	void injectParticles(float x, float y) override;

	void updateInstanceBuffer();
	~Circles();
}; 

