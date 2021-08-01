#include<iostream>
#include "../Header/ParticleSimulation.h"
#include"../Header/error_check.h"
#include<glm/gtc/type_ptr.hpp>
#include<vector>

int c = 0;

ParticleSimulation::ParticleSimulation() : objMgr()
{
    circleSizes[static_cast<int>(circleType::NORMAL)] = 10;
    circleSizes[static_cast<int>(circleType::NORMAL_STATIONARY)] = 10;
    circleSizes[static_cast<int>(circleType::GRAVITY)] = 10;
    circleSizes[static_cast<int>(circleType::GRAVITY_STATIONARY)] = 10;

    barPositions[0] =  0.625;
    barPositions[1] =  0.125;
    barPositions[2] = -0.375;
    barPositions[3] = -0.875;
    

    //shaders
    const char* vertexshader =
        "#version 330 core\n"
        "layout(location = 0) in vec2 position;\n"
        //"layout(location = 1) in vec3 color;\n"
        "uniform mat4 transform;\n"
        //"out vec3 vertexColor;\n"
        "void main(){\n"
        "gl_Position = transform*vec4(position,1,1);\n"
        //"vertexColor = color;\n"
        "}\0\n";

    const char* fragmentshader =
        "#version 330 core\n"
        "uniform vec3 Color;\n"
        "out vec4 color;\n"
        "void main(){\n"
        "color = vec4(Color,1.0);\n"
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
        -2.5f / 320.0f, 1.0f, //0.47f, 0.47f, 0.47f,
        -2.5f / 320.0f,-1.0f, //0.47f, 0.47f, 0.47f,
		 2.5f / 320.0f,-1.0f, //0.47f, 0.47f, 0.47f,
		 2.5f / 320.0f, 1.0f  //0.47f, 0.47f, 0.47f,
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
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float)*4, vertices, GL_STATIC_DRAW);

    //index buffers
    glGenBuffers(1, &ID.IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW);

    //enable vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2*sizeof(float)));

    glUseProgram(ID.shaderProgram);
    glBindVertexArray(0);

    transformLoc = glGetUniformLocation(ID.shaderProgram, "transform");
    colorLoc = glGetUniformLocation(ID.shaderProgram, "Color");

    ////////////////////////////////////////////////////////////////////////////////////////////////

    //now for circles
    float CircleVertices[(NOOFTRIANGLES + 1) * 2];
    unsigned int CircleIndices[(NOOFTRIANGLES * 3)];

    CircleVertices[0] = 0.0f;
    CircleVertices[1] = 0.0f;
    //CircleVertices[2] = 1.0f;
    //CircleVertices[3] = 0.5f;
    //CircleVertices[4] = 0.2f;

    int k = 2;
    for (int angle = 0; angle < 360; angle += 360 / NOOFTRIANGLES)
    {
        CircleVertices[k] = (CIRCLESIZE  * std::cos((PI / (float)180) * angle) / (float)(320.0 / 2));
        CircleVertices[k + 1] = (CIRCLESIZE * std::sin((PI / (float)180) * angle) / (float)(SIM_VIEW_HEIGHT / 2));
        //CircleVertices[k + 2] = 1.0f;
        //CircleVertices[k + 3] = 0.5f;
        //CircleVertices[k + 4] = 0.2f;
        k += 2;
    }

    CircleIndices[0] = 0;
    unsigned int temp = 1;
    for (int i = 0; i < 3 * NOOFTRIANGLES - 1; i++)
    {
        if (i % 3 == 0)
            CircleIndices[i] = 0;
        else if (i % 3 == 1)
        {
            CircleIndices[i] = temp;
            temp++;
        }
        else
        {
            CircleIndices[i] = temp;
        }
    }
    CircleIndices[3 * NOOFTRIANGLES - 1] = 1;

    //vertex arrays
    glGenVertexArrays(1, &CircleID.VAO);
    glBindVertexArray(CircleID.VAO);

    //vertex buffers
    glGenBuffers(1, &CircleID.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, CircleID.VBO);
    glBufferData(GL_ARRAY_BUFFER, (NOOFTRIANGLES + 1) * 5* sizeof(float) , CircleVertices, GL_STATIC_DRAW);

    //index buffers
    glGenBuffers(1, &CircleID.IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CircleID.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (NOOFTRIANGLES * 3) * sizeof(unsigned int), CircleIndices, GL_STATIC_DRAW);

    //enable vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);

}

bool checkHoverOnMarker(float x,float y,float barPos)
{
    //std::cout << "(" << x << "," << y << ")" << std::endl;
    if (x <= 0.5625 && x >= 0.4375 && y <= (barPos + 0.01) && y >= (barPos - 0.01))
    {
        return true;
    }
    else
    {
        return false;
    }
}

UserIface* ParticleSimulation::InputEventHandler()
{
    Mouse& mouse = Mouse::getInstance();

    mouseModes mMode = mouse.getMouseMode();
    mousePos mPos = mouse.getMousePosition();
    unsigned int count = mouse.getPressCount();

    if (mPos.x < 1600)
    {
		if (mMode == mouseModes::PRESSED && count % 5 == 0)
		{

			//setting the right coordinate system
			mPos.x = mPos.x - SIM_VIEW_WIDTH / 2;
			mPos.y = -(mPos.y - SIM_VIEW_HEIGHT / 2);

			//normalizing
			mPos.x = mPos.x / (float)(SIM_VIEW_WIDTH / 2);
			mPos.y = mPos.y / (float)(SIM_VIEW_HEIGHT / 2);

			injectParticles(mPos.x, mPos.y);
			std::cout << "   CURRENTLY PRESSED" << std::endl;
		}    
    }
    else
    {
        //std::cout << "(" << mPos.x << "," << mPos.y << ")" << std::endl;
        //setting the right coordinate system
        mPos.x = mPos.x -SIM_VIEW_WIDTH - UI_VIEW_WIDTH / 2;
        mPos.y = -(mPos.y - UI_VIEW_HEIGHT / 2);

        //normalizing
        mPos.x = mPos.x / (float)(UI_VIEW_WIDTH / 2);
        mPos.y = mPos.y / (float)(UI_VIEW_HEIGHT / 2);

        int h = -1;
        for (int i = 0; i < 4; i++)
        {
            hover[i] = checkHoverOnMarker(mPos.x, mPos.y,barPositions[i]);
            h = ((hover[i] == true) ? i : h);
        }


        if (mMode == mouseModes::PRESSED ) 
        {
            if (mPos.x < 0 && mPos.x > -0.5)
            {
                if (mPos.y > 0.5) typeSelected = circleType::NORMAL;
                else if (mPos.y > 0) typeSelected = circleType::NORMAL_STATIONARY;
                else if(mPos.y > -0.5) typeSelected = circleType::GRAVITY;
                else typeSelected = circleType::GRAVITY_STATIONARY;
            }

            if (h != -1)
            {
                if (mPos.y < +(0.75 - h * 0.5) + 0.125 && mPos.y >= (0.75 - h * 0.5) - 0.125)
                {
                    barPositions[h] = mPos.y;
                }
                else if (mPos.y >= (0.75 - h * 0.5) - 0.125)
                {
                    barPositions[h] = (0.75 - h * 0.5) + 0.125;
                }
                else
                {
                    barPositions[h] = (0.75 - h * 0.5) - 0.125;
                }
            }
        }

    }

    return this;
}

void ParticleSimulation::UpdateVertexBuffers()
{
    //first set the circle size according to the markers
    for (int i = 0; i < 4; i++)
    {
        circleSizes[i] = ((barPositions[i] - (0.625 - i * 0.5)) * 4 * 9 + 1);
    }


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
    glViewport(1600, 0, UI_VIEW_WIDTH, UI_VIEW_HEIGHT);

    //first draw the separating line
    glBindBuffer(GL_ARRAY_BUFFER,ID.VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID.IBO);
    glBindVertexArray(ID.VAO);
    glUseProgram(ID.shaderProgram);

    glm::vec3 color = glm::vec3(0.47f, 0.47f, 0.47f);

    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-0.9921875f, 0.0f, 0.0f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glUniform3f(colorLoc, color.x, color.y, color.z);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //now the size bars
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.5f, 0.75f, 0.0f));
    transform = glm::scale(transform, glm::vec3(0.5f, 0.125f, 1.0f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.5f, 0.25f, 0.0f));
    transform = glm::scale(transform, glm::vec3(0.5f, 0.125f, 1.0f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.5f,-0.25f, 0.0f));
    transform = glm::scale(transform, glm::vec3(0.5f, 0.125f, 1.0f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.5f,-0.75f, 0.0f));
    transform = glm::scale(transform, glm::vec3(0.5f, 0.125f, 1.0f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //scroll marker for the side bars

    for (int i = 0; i < 4; i++)
    {
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.5f, barPositions[i], 0.0f));
        transform = glm::scale(transform, glm::vec3(8, 0.01f, 1.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        if (hover[i] == true)
        {
            glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
        }
        else
        {
            glUniform3f(colorLoc, 0.47f, 0.47f, 0.47f);
        }
        

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //now draw the circles
    glBindVertexArray(CircleID.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, CircleID.VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CircleID.IBO);
    glUseProgram(ID.shaderProgram);

    color = glm::vec3(1.0f, 0.5f, 0.2f);
    

    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-0.25f, 0.75f, 0.0f));
    transform = glm::scale(transform, glm::vec3(circleSizes[0], circleSizes[0], 1));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glUniform3f(colorLoc, color.x, color.y, color.z);

    glDrawElements(GL_TRIANGLES, (NOOFTRIANGLES * 3), GL_UNSIGNED_INT, 0);

    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-0.25f, 0.25f, 0.0f));
    transform = glm::scale(transform, glm::vec3(circleSizes[1], circleSizes[1], 1));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glDrawElements(GL_TRIANGLES, (NOOFTRIANGLES * 3), GL_UNSIGNED_INT, 0);

    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-0.25f, -0.25f, 0.0f));
    transform = glm::scale(transform, glm::vec3(circleSizes[2], circleSizes[2], 1));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glDrawElements(GL_TRIANGLES, (NOOFTRIANGLES * 3), GL_UNSIGNED_INT, 0);

    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-0.25f, -0.75f, 0.0f));
    transform = glm::scale(transform, glm::vec3(circleSizes[3], circleSizes[3], 1));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glDrawElements(GL_TRIANGLES, (NOOFTRIANGLES * 3), GL_UNSIGNED_INT, 0);

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

    objMgr.injectParticles(x, y,circleSizes[static_cast<int>(typeSelected)], static_cast<int>(typeSelected));
}



ParticleSimulation::~ParticleSimulation()
{
    glDeleteShader(ID.vertexShader);
    glDeleteShader(ID.fragmentShader);

    GLCALL(glDeleteBuffers(1, &ID.VBO));
    GLCALL(glDeleteBuffers(1, &ID.IBO));
    GLCALL(glDeleteVertexArrays(1, &ID.VAO));
}
