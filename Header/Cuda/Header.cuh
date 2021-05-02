#pragma once
#include<cuda_runtime.h>
#include "device_launch_parameters.h"
#include"../parameters.h"
#include"../particle.h"

 

namespace cuda
{
	__device__ void updateVBPositions(glm::vec3 dist, unsigned int pos, float* vertexbuffer)
	{
		unsigned int vb_pos = (NOOFTRIANGLES + 1) * 2 * pos;

		for (; vb_pos < (NOOFTRIANGLES + 1) * 2 * (pos + 1); vb_pos += 2)
		{
			vertexbuffer[vb_pos] = vertexbuffer[vb_pos] + dist.x;
			vertexbuffer[vb_pos + 1] = vertexbuffer[vb_pos + 1] + dist.y;
			//printf("position: %i and position in vertex buffer: %i\n", pos, vb_pos);
		}
	}

	__global__ void calcPosition(particle* particles, float* vertexbuffer)
	{
		unsigned int pos = blockDim.x * blockIdx.x + threadIdx.x;

		if (particles[pos].injected)
		{
			float vX{ particles[pos].m_velocity.x }, vY(particles[pos].m_velocity.y);
			glm::vec3 dist = glm::vec3(vX * particles[pos].timestep + 0.5f * particles[pos].m_accelaration.x * particles[pos].timestep * particles[pos].timestep,
				vY * particles[pos].timestep + 0.5f * particles[pos].m_accelaration.y * particles[pos].timestep * particles[pos].timestep, 0);
			particles[pos].m_position = particles[pos].m_position + dist;

			updateVBPositions(dist, pos, vertexbuffer);
		}
	}

	__global__ void calcVelocity(particle* particles)
	{
		unsigned int pos = blockDim.x * blockIdx.x + threadIdx.x;

		if (particles[pos].injected)
		{
			float aX{ particles[pos].m_accelaration.x }, aY(particles[pos].m_accelaration.y);
			glm::vec3 velocity = glm::vec3(aX * particles[pos].timestep, aY * particles[pos].timestep, 0);

			if (!particles[pos].collision.occur)
			{
				particles[pos].m_velocity = particles[pos].m_velocity + velocity;
			}
			else
			{
				//particles[pos].m_velocity = particles[pos].collision.m_velocity + velocity; //maybe try?
				particles[pos].m_velocity = particles[pos].collision.m_velocity;
			}
		}
	}

	__global__ void calcAccelaration(particle* particles)
	{
		unsigned int pos = blockDim.x * blockIdx.x + threadIdx.x;

		if (particles[pos].injected)
		{
			//for now just gravity
			particles[pos].m_accelaration = glm::vec3(0.0f, -0.5f, 0.0f);
		}


	}

	__global__ void collisionCheck(particle* particles)
	{
		unsigned int pos = blockDim.x * blockIdx.x + threadIdx.x;

		if (particles[pos].injected)
		{
			bool WallCollisiony = particles[pos].m_position.y <= -1.0f;//|| particles[pos].m_position.y >= 1.0f;
			if (WallCollisiony)
			{
				particles[pos].collision.occur = true;
				particles[pos].collision.m_velocity.y = -particles[pos].m_velocity.y;
			}
			else
			{
				particles[pos].collision.occur = false;
			}
			//bool WallCollisionx = particles[pos].m_position.x <= -1.0f || particles[pos].m_position.x >= 1.0f;
			//if (WallCollisionx)
			//{
			//	particles[pos].collision.occur = true;
			//	particles[pos].collision.m_velocity.x = -particles[pos].m_velocity.x;
			//}
			//else
			//{
			//	particles[pos].collision.occur = false;
			//}
		}


	}

}