#include<iostream>
#include "../Header/ParticleSimulation.h"
#include"../Header/error_check.h"

const char* vertexshader =
"#version 330 core\n"
"layout(location = 0) in vec4 position;\n"
"void main(){\n"
"gl_Position = position;\n"
"}\0\n";

const char* fragmentshader =
"#version 330 core\n"
"out vec4 color;\n"
"void main(){\n"
"color = vec4(1.0f , 0.5f , 0.2f , 1.0f);\n"
"}\0\n";

ParticleSimulation::ParticleSimulation()
{
    //setting up the vertex data
    //getting the points of a circle

    count = 0;

    vertexBuffer[0] = 0.0f;
    vertexBuffer[1] = 0.0f;
    int k = 2;
    for (int angle = 0; angle < 360; angle += 360 / NOOFTRIANGLES)
    {
        vertexBuffer[k] = (CIRCLESIZE * std::cos((PI / (float)180) * angle) / (float)960);
        vertexBuffer[k + 1] = (CIRCLESIZE * std::sin((PI / (float)180) * angle) / (float)540);
        k += 2;
    }

    //propagating the circle coordinates throughout the buffer
    for (int i = 0; i < MAX_PARTICLES * ((NOOFTRIANGLES + 1) * 2); i++)
    {
        vertexBuffer[i] = vertexBuffer[i % ((NOOFTRIANGLES + 1) * 2)];
    }

    //setting up the index data;

    indexBuffer[0] = 0;
    unsigned int temp = 1;
    for (int i = 0; i < 3 * NOOFTRIANGLES - 1; i++)
    {
        if (i % 3 == 0)
            indexBuffer[i] = 0;
        else if (i % 3 == 1)
        {
            indexBuffer[i] = temp;
            temp++;
        }
        else
        {
            indexBuffer[i] = temp;
        }
    }
    indexBuffer[3 * NOOFTRIANGLES - 1] = 1;

    int second_last = indexBuffer[3 * NOOFTRIANGLES - 2];
    for (int j = (NOOFTRIANGLES * 3); j < MAX_PARTICLES * (NOOFTRIANGLES * 3); j++)
    {
        indexBuffer[j] = indexBuffer[j % (NOOFTRIANGLES * 3)] + second_last + 1;
        if (j % (NOOFTRIANGLES * 3) == (NOOFTRIANGLES * 3) - 1)
            second_last = indexBuffer[j - 1];
    }

    //setting up the buffers


    /// <summary>
    /// setting up shaders
    /// </summary>

    int success;
    char info[512];

    ID.vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(ID.vertexShader, 1, &vertexshader, NULL);
    glCompileShader(ID.vertexShader);

    glGetShaderiv(ID.vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(ID.vertexShader, 512, NULL, info);
        std::cout << "compilation failed :: vertex shader " << std::endl << info << std::endl;
    }

    ID.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(ID.fragmentShader, 1, &fragmentshader, NULL);
    glCompileShader(ID.fragmentShader);

    glGetShaderiv(ID.fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(ID.fragmentShader, 512, NULL, info);
        std::cout << "compilation failed :: fragment shader " << std::endl << info << std::endl;
    }

    ID.shaderProgram = glCreateProgram();
    glAttachShader(ID.shaderProgram, ID.vertexShader);
    glAttachShader(ID.shaderProgram, ID.fragmentShader);
    glLinkProgram(ID.shaderProgram);

    glGetProgramiv(ID.shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID.shaderProgram, 512, NULL, info);
        std::cout << "linking failed " << std::endl << info << std::endl;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    glGenVertexArrays(1, &ID.VAO);
    glBindVertexArray(ID.VAO);

    glGenBuffers(1, &ID.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, ID.VBO);
    //glBufferData(GL_ARRAY_BUFFER, 2 * (NOOFTRIANGLES + 1) * sizeof(float), vertexBuffer, GL_STATIC_DRAW);

    GLCALL(glGenBuffers(1, &ID.IBO));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID.IBO));
    //GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * NOOFTRIANGLES * sizeof(unsigned int), indexBuffer, GL_STATIC_DRAW));


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glUseProgram(ID.shaderProgram);
}

void ParticleSimulation::InputEventHandler()
{
}

void ParticleSimulation::UpdateVertexBuffers()
{
    calcAccelaration();
    calcVelocity();
    calcPosition();
    collisionCheck();
}

void ParticleSimulation::render()
{
    GLCALL(glBufferData(GL_ARRAY_BUFFER, count * ((NOOFTRIANGLES + 1) * 2) * sizeof(float), vertexBuffer, GL_STATIC_DRAW));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * (3 * NOOFTRIANGLES) * sizeof(unsigned int), indexBuffer, GL_STATIC_DRAW));
    GLCALL(glDrawElements(GL_TRIANGLES, NOOFTRIANGLES * 3 * count, GL_UNSIGNED_INT, 0));
}

void ParticleSimulation::calcPosition() 
{
    //unsigned int pos = blockDim.x * blockIdx.x + threadIdx.x;
    for (int pos = 0; pos < count; pos++)
    {
        if (particles[pos].injected)
        {
            float vX{ particles[pos].m_velocity.x }, vY(particles[pos].m_velocity.y);
            glm::vec3 dist = glm::vec3(vX * particles[pos].timestep + 0.5f * particles[pos].m_accelaration.x * particles[pos].timestep * particles[pos].timestep,
                vY * particles[pos].timestep + 0.5f * particles[pos].m_accelaration.y * particles[pos].timestep * particles[pos].timestep, 0);

            if ((particles[pos].m_velocity.x == 0 || particles[pos].m_velocity.y == 0) && particles[pos].collision.occur)
            {
                if (particles[pos].m_velocity.x == 0)
                {
                    dist.x = 0;
                }
                if (particles[pos].m_velocity.y == 0)
                {
                    dist.y = 0;
                }
            }
            particles[pos].m_position = particles[pos].m_position + dist;

            updateVBPositions(dist, pos);
        }
    }
}

void ParticleSimulation::calcVelocity()
{
    //unsigned int pos = blockDim.x * blockIdx.x + threadIdx.x;
    for (int pos = 0; pos < count; pos++)
    {
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
}

void ParticleSimulation::calcAccelaration()
{
    //unsigned int pos = blockDim.x * blockIdx.x + threadIdx.x;
    for (int pos = 0; pos < count; pos++)
    {
        if (particles[pos].injected)
        {
            //for now just gravity
            if (particles[pos].m_position.y < -0.8f)
            {
                particles[pos].m_accelaration = glm::vec3(0.0f, 0.0f, 0.0f);
            }
            else
            {
                particles[pos].m_accelaration = glm::vec3(0.0f, -0.5f, 0.0f);
            }
        }
    }
}

void ParticleSimulation::collisionCheck()
{
    //unsigned int pos = blockDim.x * blockIdx.x + threadIdx.x;
    for (int pos = 0; pos < count; pos++)
    {
        if (particles[pos].injected)
        {
            bool WallCollisiony = particles[pos].m_position.y <= -1.0f || particles[pos].m_position.y >= 1.0f;
            bool WallCollisionx = particles[pos].m_position.x <= -1.0f || particles[pos].m_position.x >= 1.0f;
            if ((WallCollisionx || WallCollisiony) && !particles[pos].collision.occur)
            {
                if (WallCollisiony)
                {
                    particles[pos].collision.occur = true;
                    particles[pos].collision.m_velocity.y = -particles[pos].m_velocity.y;
                    particles[pos].collision.m_velocity.x = particles[pos].m_velocity.x;
                }
                if (WallCollisionx)
                {
                    particles[pos].collision.occur = true;
                    particles[pos].collision.m_velocity.x = -particles[pos].m_velocity.x;
                    particles[pos].collision.m_velocity.y = particles[pos].m_velocity.y;
                }
            }
            else
            {
                particles[pos].collision.occur = false;
            }

        }
    }
}

void ParticleSimulation::updateVBPositions(glm::vec3 dist, unsigned int pos)
{
    unsigned int vb_pos = (NOOFTRIANGLES + 1) * 2 * pos;

    for (; vb_pos < (NOOFTRIANGLES + 1) * 2 * (pos + 1); vb_pos += 2)
    {
        vertexBuffer[vb_pos] = vertexBuffer[vb_pos] + dist.x;
        vertexBuffer[vb_pos + 1] = vertexBuffer[vb_pos + 1] + dist.y;
        //printf("position: %i and position in vertex buffer: %i\n", pos, vb_pos);
    }
}

void ParticleSimulation::injectParticles(float x, float y)
{
    if (count == MAX_PARTICLES)
        return;

    particles[count].injected = true;
    particles[count].m_position = glm::vec3(x - (float)2.0 * CIRCLESIZE * 10.0 / 960.0, y, 1.0);
    for (unsigned int i = count * ((NOOFTRIANGLES + 1) * 2); i < (count + 1) * ((NOOFTRIANGLES + 1) * 2); i += 2)
    {
        vertexBuffer[i] = vertexBuffer[i] + particles[count].m_position.x;
        vertexBuffer[i + 1] = vertexBuffer[i + 1] + particles[count].m_position.y;
    }
    count++;

    particles[count].injected = true;
    particles[count].m_position = glm::vec3(x - (float)2.0 * CIRCLESIZE * 5.0 / 960.0, y, 1.0);
    for (unsigned int i = count * ((NOOFTRIANGLES + 1) * 2); i < (count + 1) * ((NOOFTRIANGLES + 1) * 2); i += 2)
    {
        vertexBuffer[i] = vertexBuffer[i] + particles[count].m_position.x;
        vertexBuffer[i + 1] = vertexBuffer[i + 1] + particles[count].m_position.y;
    }
    count++;

    particles[count].injected = true;
    particles[count].m_position = glm::vec3(x, y, 1.0);
    for (unsigned int i = count * ((NOOFTRIANGLES + 1) * 2); i < (count + 1) * ((NOOFTRIANGLES + 1) * 2); i += 2)
    {
        vertexBuffer[i] = vertexBuffer[i] + particles[count].m_position.x;
        vertexBuffer[i + 1] = vertexBuffer[i + 1] + particles[count].m_position.y;
    }
    count++;

    particles[count].injected = true;
    particles[count].m_position = glm::vec3(x + (float)2.0 * CIRCLESIZE * 5.0 / 960.0, y, 1.0);
    for (unsigned int i = count * ((NOOFTRIANGLES + 1) * 2); i < (count + 1) * ((NOOFTRIANGLES + 1) * 2); i += 2)
    {
        vertexBuffer[i] = vertexBuffer[i] + particles[count].m_position.x;
        vertexBuffer[i + 1] = vertexBuffer[i + 1] + particles[count].m_position.y;
    }
    count++;

    particles[count].injected = true;
    particles[count].m_position = glm::vec3(x + (float)2.0 * CIRCLESIZE * 10.0 / 960.0, y, 1.0);
    for (unsigned int i = count * ((NOOFTRIANGLES + 1) * 2); i < (count + 1) * ((NOOFTRIANGLES + 1) * 2); i += 2)
    {
        vertexBuffer[i] = vertexBuffer[i] + particles[count].m_position.x;
        vertexBuffer[i + 1] = vertexBuffer[i + 1] + particles[count].m_position.y;
    }
    count++;

    std::cout << count;
}



ParticleSimulation::~ParticleSimulation()
{
    glDeleteShader(ID.vertexShader);
    glDeleteShader(ID.fragmentShader);

    GLCALL(glDeleteBuffers(1, &ID.VBO));
    GLCALL(glDeleteBuffers(1, &ID.IBO));
    GLCALL(glDeleteVertexArrays(1, &ID.VAO));
}