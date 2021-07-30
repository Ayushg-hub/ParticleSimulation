#include "../Header/Circle.h"
#include"../Header/error_check.h"
#include<iostream>

Circles::Circles()
{
    count = 0;

    vertexBuffer[0] = 0.0f;
    vertexBuffer[1] = 0.0f;

    int k = 2;
    for (int angle = 0; angle < 360; angle += 360 / NOOFTRIANGLES)
    {
        vertexBuffer[k] = (CIRCLESIZE * std::cos((PI / (float)180) * angle) / (float)(SIM_VIEW_WIDTH / 2));
        vertexBuffer[k + 1] = (CIRCLESIZE * std::sin((PI / (float)180) * angle) / (float)(SIM_VIEW_HEIGHT / 2));
        k += 2;
    }

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

    //shaders
    const char* vertexshader =
        "#version 330 core\n"
        "layout(location = 0) in vec2 position;\n"
        "layout(location = 1) in vec2 offset;\n"
        "void main(){\n"
        "gl_Position = vec4(position + offset,1,1);\n"
        "}\0\n";

    const char* fragmentshader =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main(){\n"
        "color = vec4(1.0f , 0.5f , 0.2f , 1.0f);\n"
        "}\0\n";

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

    //create VAO and bind VAO

    glGenVertexArrays(1, &ID.VAO);
    glBindVertexArray(ID.VAO);

    //create & bind vertex buffers 
    glGenBuffers(1, &ID.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, ID.VBO);
    glBufferData(GL_ARRAY_BUFFER, (NOOFTRIANGLES + 1) * 2 * sizeof(float), vertexBuffer, GL_STATIC_DRAW);

    //creating attrib pointers for the current buffer
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2 , GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    //create & bind index buffers
    glGenBuffers(1, &ID.IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (NOOFTRIANGLES * 3) * sizeof(GLuint),indexBuffer , GL_STATIC_DRAW);

    //create and bind instance buffer
    glGenBuffers(1, &ID.instance_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, ID.instance_VBO);
    
    //creating attrib pointers for the current buffer
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glVertexAttribDivisor(1, 1);

    glUseProgram(ID.shaderProgram);
    glBindVertexArray(0);
}

void Circles::draw()
{
    if (count == 0)
    {
        return;
    }

    glViewport(0, 0, SIM_VIEW_WIDTH, SIM_VIEW_HEIGHT );
    updateInstanceBuffer();

    glBindVertexArray(ID.VAO);
    glUseProgram(ID.shaderProgram);

    GLCALL(glBufferData(GL_ARRAY_BUFFER, count * 2 * sizeof(float), positions, GL_STATIC_DRAW));
    GLCALL(glDrawElementsInstanced(GL_TRIANGLES, NOOFTRIANGLES * 3, GL_UNSIGNED_INT, 0, count))
}

void Circles::injectParticles(float x, float y)
{
    if (count == MAX_PARTICLES)
        return;

    //circles[count].data.injected = true;
    circles[count].data.m_position = glm::vec3(x - (float)2.0 * CIRCLESIZE * 10.0 / (SIM_VIEW_WIDTH / 2), y, 1.0);

    positions[2 * count] = circles[count].data.m_position.x;
    positions[2 * count + 1] = circles[count].data.m_position.y;

    count++;

    //circles[count].data.injected = true;
    circles[count].data.m_position = glm::vec3(x - (float)2.0 * CIRCLESIZE * 5.0 / (SIM_VIEW_WIDTH / 2), y, 1.0);

    positions[2 * count] = circles[count].data.m_position.x;
    positions[2 * count + 1] = circles[count].data.m_position.y;

    count++;

    //particles[count].injected = true;
    circles[count].data.m_position = glm::vec3(x, y, 1.0);

    positions[2 * count] = circles[count].data.m_position.x;
    positions[2 * count + 1] = circles[count].data.m_position.y;

    count++;

    //particles[count].injected = true;
    circles[count].data.m_position = glm::vec3(x + (float)2.0 * CIRCLESIZE * 5.0 / (SIM_VIEW_WIDTH/2), y, 1.0);

    positions[2 * count] = circles[count].data.m_position.x;
    positions[2 * count + 1] = circles[count].data.m_position.y;

    count++;

    //particles[count].injected = true;
    circles[count].data.m_position = glm::vec3(x + (float)2.0 * CIRCLESIZE * 10.0 / (SIM_VIEW_WIDTH / 2), y, 1.0);

    positions[2 * count] = circles[count].data.m_position.x;
    positions[2 * count + 1] = circles[count].data.m_position.y;

    count++;

    std::cout << count;
}

void Circles::updateInstanceBuffer()
{
    for (unsigned int i = 0; i < count; i++)
    {
        positions[2 * i] = circles[i].data.m_position.x;
        positions[2 * i + 1] = circles[i].data.m_position.y;
    }
}

Shape Circles::getShape()
{
	return Shape::CIRCLE;
}

void* Circles::getInfo()
{
	return (void*)circles;
}

unsigned int Circles::getCount()
{
    return count;
}

Circles::~Circles()
{
    glDeleteShader(ID.vertexShader);
    glDeleteShader(ID.fragmentShader);

    GLCALL(glDeleteBuffers(1, &ID.VBO));
    GLCALL(glDeleteBuffers(1, &ID.instance_VBO));
    GLCALL(glDeleteBuffers(1, &ID.IBO));
    GLCALL(glDeleteVertexArrays(1, &ID.VAO));
}
