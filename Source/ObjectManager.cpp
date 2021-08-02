#include "..\Header\ObjectManager.h"
#include<glm/gtx/norm.hpp>
#include<iostream>

ObjectManager::ObjectManager()
{
    objMap.insert(std::pair<Shape, Objects*>(Shape::CIRCLE, new Circles()));
}

void ObjectManager::renderObjects()
{
    objMap[Shape::CIRCLE]->draw();
}

void ObjectManager::InputEventHandler()
{

}

void ObjectManager::calcPosition()
{
    void* info = objMap[Shape::CIRCLE]->getInfo();
    CircleData* particles = static_cast<CircleData*>(info);
    unsigned int count = objMap[Shape::CIRCLE]->getCount();

    for (unsigned int pos = 0; pos < count; pos++)
    {
        if (particles[pos].type == circleType::GRAVITY_STATIONARY || particles[pos].type == circleType::NORMAL_STATIONARY)
        {
            continue;
        }

        float vX{ particles[pos].data.m_velocity.x }, vY(particles[pos].data.m_velocity.y);
        glm::vec3 dist = glm::vec3(vX * TIMESTEP + 0.5f * particles[pos].data.m_accelaration.x * TIMESTEP * TIMESTEP,
            vY * TIMESTEP + 0.5f * particles[pos].data.m_accelaration.y * TIMESTEP * TIMESTEP, 0);

        //if ((particles[pos].data.m_velocity.x == 0 || particles[pos].data.m_velocity.y == 0) && particles[pos].data.collision.occur)
        //{
        //    if (particles[pos].data.m_velocity.x == 0)
        //    {
        //        dist.x = 0;
        //    }
        //    if (particles[pos].data.m_velocity.y == 0)
        //    {
        //        dist.y = 0;
        //    }
        //}

        particles[pos].data.m_position = particles[pos].data.m_position + dist;

    }
}

void ObjectManager::calcVelocity()
{
    void * info = objMap[Shape::CIRCLE]->getInfo();
    CircleData* particles = static_cast<CircleData*>(info);
    unsigned int count = objMap[Shape::CIRCLE]->getCount();

    for (unsigned int pos = 0; pos < count; pos++)
    {
        if (particles[pos].type == circleType::GRAVITY_STATIONARY || particles[pos].type == circleType::NORMAL_STATIONARY)
        {
            continue;
        }

		float aX{ particles[pos].data.m_accelaration.x }, aY(particles[pos].data.m_accelaration.y);
		glm::vec3 velocity = glm::vec3(aX * TIMESTEP, aY * TIMESTEP, 0);

		//if (!particles[pos].collision.occur)
		//{
		//	particles[pos].m_velocity = particles[pos].m_velocity + velocity;
		//}
		//else
		//{
		//	//particles[pos].m_velocity = particles[pos].collision.m_velocity + velocity; //maybe try?
		//	particles[pos].m_velocity = particles[pos].collision.m_velocity;
		//}

        particles[pos].data.m_velocity = particles[pos].data.m_velocity + velocity;
        
    }
}

void ObjectManager::calcAccelaration()
{
    void* info = objMap[Shape::CIRCLE]->getInfo();
    CircleData* particles = static_cast<CircleData*>(info);
    unsigned int count = objMap[Shape::CIRCLE]->getCount();

    std::vector<CircleData> gravityPos;
    for (unsigned int pos = 0; pos < count; pos++)
    {
        if (particles[pos].type == circleType::GRAVITY_STATIONARY || particles[pos].type == circleType::GRAVITY)
        {
            gravityPos.push_back(particles[pos]);
        }
    }

    for (unsigned int pos = 0; pos < count; pos++)
    {
		if (particles[pos].type == circleType::GRAVITY_STATIONARY || particles[pos].type == circleType::NORMAL_STATIONARY)
		{
			continue;
		}
		//for now just gravity

		if (particles[pos].data.m_position.y < -0.8f)
		{
			particles[pos].data.m_accelaration = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		else
		{
			/*if (pos % 2 == 0)
				particles[pos].data.m_accelaration = glm::vec3(0.0f, 0.5f, 0.0f);
			else
				particles[pos].data.m_accelaration = glm::vec3(0.0f, -0.5f, 0.0f);*/
            if (GRAVITY_EFFECT)
            {
                particles[pos].data.m_accelaration = glm::vec3(0.0f, -0.5f, 0.0f);
            }
            else
            {
                particles[pos].data.m_accelaration = glm::vec3(0.0f, 0.0f, 0.0f);
            }
            glm::vec3 dir;
            for (auto itr : gravityPos)
            {
                float dx = (itr.data.m_position.x - particles[pos].data.m_position.x) * SCREEN_WIDTH / 2.0;
                float dy = (itr.data.m_position.y - particles[pos].data.m_position.y) * SCREEN_HEIGHT / 2.0;
                float sqrt_ = std::sqrt(dx * dx + dy * dy);

                if (sqrt_ < itr.m_radius + particles[pos].m_radius)
                    continue;

                dir = itr.data.m_position - particles[pos].data.m_position;
                float norm = glm::l2Norm(dir);

                float magnitude = GRAVITATIONAL_CONSTANT * itr.data.mass / (norm*norm);
                dir = glm::normalize(dir);
                particles[pos].data.m_accelaration += magnitude * dir;

            }
		}
        
    }
}

void ObjectManager::calcAdjacencyMatrix()
{
    void* info = objMap[Shape::CIRCLE]->getInfo();
    CircleData* particles = static_cast<CircleData*>(info);
    unsigned int count = objMap[Shape::CIRCLE]->getCount();

    for (unsigned int i = 0; i < count; i++)
    {
        for (unsigned int j = i + 1; j < count; j++)
        {
            float dx = (particles[i].data.m_position.x - particles[j].data.m_position.x) * SCREEN_WIDTH / 2.0;
            float dy = (particles[i].data.m_position.y - particles[j].data.m_position.y) * SCREEN_HEIGHT / 2.0;
            float sqrt_ = std::sqrt(dx * dx + dy * dy);

            //checking cooldown
            if (adjacancyMatrix[i][j] == -1.0 && sqrt_ <= particles[i].m_radius + particles[j].m_radius)
            {
                continue;
            }
            else
            {
                adjacancyMatrix[i][j] = sqrt_;
            }
        }
    }
}

void ObjectManager::collisionCheck()
{
    void* info = objMap[Shape::CIRCLE]->getInfo();
    CircleData* particles = static_cast<CircleData*>(info);
    unsigned int count = objMap[Shape::CIRCLE]->getCount();

    //unsigned int pos = blockDim.x * blockIdx.x + threadIdx.x;
    calcAdjacencyMatrix();
    std::vector<std::pair<unsigned int, unsigned int>> collindingPairs;

    for (unsigned int i = 0; i < count; i++)
    {
        for (unsigned int j = i + 1; j < count; j++)
        {
            if (adjacancyMatrix[i][j] <= particles[i].m_radius + particles[j].m_radius && adjacancyMatrix[i][j] >= 0)
            {
                collindingPairs.push_back(std::make_pair(i, j));
                adjacancyMatrix[i][j] = -1.0f;
            }
        }
    }

    for (unsigned int pos = 0; pos < count; pos++)
    {

			bool WallCollisiony = (particles[pos].data.m_position.y <= -1.0f && particles[pos].data.m_velocity.y < 0) || 
                                  (particles[pos].data.m_position.y >= 1.0f && particles[pos].data.m_velocity.y > 0);

			bool WallCollisionx = (particles[pos].data.m_position.x <= -1.0f && particles[pos].data.m_velocity.x < 0) || 
                                  (particles[pos].data.m_position.x >= 1.0f && particles[pos].data.m_velocity.x > 0);

            if ((WallCollisionx || WallCollisiony) && !particles[pos].data.collide)
            {
                if (WallCollisiony)
                {
                    particles[pos].data.collide = true;
                    particles[pos].data.m_velocity.y = -particles[pos].data.m_velocity.y;
                }
                if (WallCollisionx)
                {
                    particles[pos].data.collide = true;
                    particles[pos].data.m_velocity.x = -particles[pos].data.m_velocity.x;
                }
            }
            else
            {
                particles[pos].data.collide = false;
            }

            std::vector<std::pair<unsigned int, unsigned int>>::iterator itr = collindingPairs.begin();
            while (!collindingPairs.empty() && itr->first == pos)
            {
                //std::swap(particles[itr->first].data.m_velocity, particles[itr->second].data.m_velocity);

                glm::vec3 collisionDir = particles[itr->second].data.m_position - particles[itr->first].data.m_position; 
                collisionDir = glm::normalize(collisionDir);
                glm::vec3 collisionDir_p = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0))*glm::vec4(collisionDir,0.0);
                collisionDir_p = glm::normalize(collisionDir_p);


                float V1i = glm::dot(particles[itr->first].data.m_velocity,collisionDir);
                float V2i = glm::dot(particles[itr->second].data.m_velocity, collisionDir);

                float V1p = glm::dot(particles[itr->first].data.m_velocity, collisionDir_p);
                float V2p = glm::dot(particles[itr->second].data.m_velocity, collisionDir_p);

                if ((particles[itr->first].type == circleType::GRAVITY || particles[itr->first].type == circleType::NORMAL) &&
                    (particles[itr->second].type == circleType::GRAVITY || particles[itr->second].type == circleType::NORMAL))
                {
                    float totalMass = particles[itr->first].data.mass + particles[itr->second].data.mass;
                    float difference = particles[itr->first].data.mass - particles[itr->second].data.mass;

                    float V1f = (difference / totalMass) * V1i + (2 * particles[itr->second].data.mass / totalMass) * V2i;
                    float V2f = (2 * particles[itr->first].data.mass / totalMass) * V1i - (difference / totalMass) * V2i;

                    particles[itr->first].data.m_velocity = V1f * collisionDir + V1p * collisionDir_p;
                    particles[itr->second].data.m_velocity = V2f * collisionDir + V2p * collisionDir_p;
                }
                else if (particles[itr->first].type == circleType::GRAVITY_STATIONARY || particles[itr->first].type == circleType::NORMAL_STATIONARY)
                {
                    particles[itr->second].data.m_velocity = -V2i * collisionDir + V2p * collisionDir_p;
                    //std::cout << "new velocity is : " << particles[itr->second].data.m_velocity.x << " , " << particles[itr->second].data.m_velocity.y << std::endl;
                }
                else if (particles[itr->second].type == circleType::GRAVITY_STATIONARY || particles[itr->second].type == circleType::NORMAL_STATIONARY)
                {
                    particles[itr->first].data.m_velocity = -V1i * collisionDir + V1p * collisionDir_p;
                    //std::cout << "new velocity is : " << particles[itr->first].data.m_velocity.x << " , " << particles[itr->first].data.m_velocity.y << std::endl;
                }
                else
                {
                    //do nothing
                }
                

                collindingPairs.erase(itr);

                itr = collindingPairs.begin();
            }
        
    }
}

void ObjectManager::injectParticles(float x, float y,float size,unsigned int type)
{
    objMap[Shape::CIRCLE]->injectParticles(x,y,size,type);
}

ObjectManager::~ObjectManager()
{
    delete objMap[Shape::CIRCLE];


}
