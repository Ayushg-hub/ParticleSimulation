#include "..\Header\ObjectManager.h"

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

    for (unsigned int pos = 0; pos < count; pos++)
    {
            //for now just gravity
            if (particles[pos].data.m_position.y < -0.8f)
            {
                particles[pos].data.m_accelaration = glm::vec3(0.0f, 0.0f, 0.0f);
            }
            else
            {
                if (pos % 2 == 0)
                    particles[pos].data.m_accelaration = glm::vec3(0.0f, 0.5f, 0.0f);
                else
                    particles[pos].data.m_accelaration = glm::vec3(0.0f, -0.5f, 0.0f);
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
            if (adjacancyMatrix[i][j] == -1.0 && sqrt_ < particles[i].m_radius + particles[j].m_radius)
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
                    particles[pos].data.m_velocity.x = particles[pos].data.m_velocity.x;
                }
                if (WallCollisionx)
                {
                    particles[pos].data.collide = true;
                    particles[pos].data.m_velocity.x = -particles[pos].data.m_velocity.x;
                    particles[pos].data.m_velocity.y = particles[pos].data.m_velocity.y;
                }
            }
            else
            {
                particles[pos].data.collide = false;
            }

            std::vector<std::pair<unsigned int, unsigned int>>::iterator itr = collindingPairs.begin();
            while (!collindingPairs.empty() && itr->first == pos)
            {
                std::swap(particles[itr->first].data.m_velocity, particles[itr->second].data.m_velocity);

                //particles[itr->first].collision.occur = true;
                //particles[itr->second].collision.occur = true;

                collindingPairs.erase(itr);

                itr = collindingPairs.begin();
            }
        
    }
}

void ObjectManager::injectParticles(float x, float y,float size)
{
    objMap[Shape::CIRCLE]->injectParticles(x,y,size);
}

ObjectManager::~ObjectManager()
{
    delete objMap[Shape::CIRCLE];


}
