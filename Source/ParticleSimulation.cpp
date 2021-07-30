#include<iostream>
#include "../Header/ParticleSimulation.h"
#include"../Header/error_check.h"
#include<glm/gtx/norm.hpp>
#include<vector>

int c = 0;

ParticleSimulation::ParticleSimulation() : objMgr()
{

    //shaders
    const char* vertexshader =
        "#version 330 core\n"
        "layout(location = 0) in vec2 position;\n"
        "layout(location = 1) in vec3 color;\n"
        "out vec3 vertexColor;\n"
        "void main(){\n"
        "gl_Position = vec4(position,1,1);\n"
        "vertexColor = color;\n"
        "}\0\n";

    const char* fragmentshader =
        "#version 330 core\n"
        "in vec3 vertexColor;\n"
        "out vec4 color;\n"
        "void main(){\n"
        "color = vec4(vertexColor,1.0);\n"
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
        std::cout << "compilation failed :: vertex shader " << std::endl << info << std::endl;
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

    ////////////////////////////////////////////////////////////////////////////

    
    float vertices[] =
    {
		-1.0f,                 1.0f, 0.47f, 0.47f, 0.47f,
		-1.0f,                -1.0f, 0.47f, 0.47f, 0.47f,
		-1.0f + 5.0f / 320.0f,-1.0f, 0.47f, 0.47f, 0.47f,
		-1.0f + 5.0f / 320.0f, 1.0f, 0.47f, 0.47f, 0.47f,
    };
    
    unsigned int indices[] =
    {
        0,1,2,
        0,2,3
    };

    //vertex arrays
    glGenVertexArrays(1, &ID.VAO);
    glBindVertexArray(ID.VAO);

    //vertex buffers
    glGenBuffers(1, &ID.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, ID.VBO);
    glBufferData(GL_ARRAY_BUFFER, 5 * sizeof(float)*4, vertices, GL_STATIC_DRAW);

    //index buffers
    glGenBuffers(1, &ID.IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW);

    //enable vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2*sizeof(float)));

    glUseProgram(ID.shaderProgram);
    glBindVertexArray(0);

}

void ParticleSimulation::InputEventHandler()
{
}

void ParticleSimulation::UpdateVertexBuffers()
{
    
    objMgr.calcAccelaration();
    objMgr.calcVelocity();
    objMgr.calcPosition();
    objMgr.collisionCheck();
    
}

void ParticleSimulation::render()
{
    renderUI();
    renderSIM();   
}

void ParticleSimulation::renderUI()
{
    glViewport(1600, 0, 320, 1080);

    glBindBuffer(GL_ARRAY_BUFFER,ID.VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID.VBO);
    glBindVertexArray(ID.VAO);
    glUseProgram(ID.shaderProgram);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);

}

void ParticleSimulation::renderSIM()
{
    objMgr.renderObjects();
}



void ParticleSimulation::injectParticles(float x, float y)
{

    objMgr.injectParticles(x, y);
}



ParticleSimulation::~ParticleSimulation()
{
    glDeleteShader(ID.vertexShader);
    glDeleteShader(ID.fragmentShader);

    GLCALL(glDeleteBuffers(1, &ID.VBO));
    GLCALL(glDeleteBuffers(1, &ID.IBO));
    GLCALL(glDeleteVertexArrays(1, &ID.VAO));
}
