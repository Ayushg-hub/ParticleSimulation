#pragma once
#include<map>
#include<vector>
#include"Circle.h"

class ObjectManager
{
	std::map<Shape, Objects*> objMap;
	float adjacancyMatrix[MAX_PARTICLES][MAX_PARTICLES]{};
	 
public :
	ObjectManager();
	void renderObjects();

	void InputEventHandler();

	void calcPosition();
	void calcVelocity();
	void calcAccelaration();

	void calcAdjacencyMatrix();
	void collisionCheck();
	void injectParticles(float x, float y);
	~ObjectManager();
};

