//#include "../Header/particleInjector.h"
//
//const char* vertexshader =
//"#version 330 core\n"
//"layout(location = 0) in vec4 position;\n"
//"void main(){\n"
//"gl_Position = position;\n"
//"}\0\n";
//
//const char* fragmentshader =
//"#version 330 core\n"
//"out vec4 color;\n"
//"void main(){\n"
//"color = vec4(1.0f , 0.5f , 0.2f , 1.0f);\n"
//"}\0\n";
//
//
//void particleInjector::init()
//{
//    //setting up the vertex data
//    //getting the points of a circle
//
//    m_count = 0;
//
//    vertexBuffer[0] = 0.0f;
//    vertexBuffer[1] = 0.0f;
//    int k = 2;
//    for (int angle = 0; angle < 360; angle += 360 / NOOFTRIANGLES)
//    {
//        vertexBuffer[k] = (CIRCLESIZE * std::cos((PI / (float)180) * angle) / (float)960);
//        vertexBuffer[k + 1] = (CIRCLESIZE * std::sin((PI / (float)180) * angle) / (float)540);
//        k += 2;
//    }
//    
//    //propagating the circle coordinates throughout the buffer
//    for (int i = 0; i < MAX_PARTICLES * ((NOOFTRIANGLES + 1) * 2); i++)
//    {
//        vertexBuffer[i] = vertexBuffer[i % ((NOOFTRIANGLES + 1) * 2)];
//    }
//
//    //setting up the index data;
//
//    indexBuffer[0] = 0 ;
//    unsigned int temp = 1;
//    for (int i = 0; i < 3 * NOOFTRIANGLES - 1; i++)
//    {
//        if (i % 3 == 0)
//            indexBuffer[i] = 0;
//        else if (i % 3 == 1)
//        {
//            indexBuffer[i] = temp;
//            temp++;
//        }
//        else
//        {
//            indexBuffer[i] = temp;
//        }
//    }
//    indexBuffer[3 * NOOFTRIANGLES - 1] = 1;
//
//    int second_last = indexBuffer[3 * NOOFTRIANGLES - 2];
//    for (int j = (NOOFTRIANGLES * 3); j < MAX_PARTICLES * (NOOFTRIANGLES * 3); j++)
//    {
//        indexBuffer[j] = indexBuffer[j % (NOOFTRIANGLES * 3)] + second_last + 1;
//        if (j % (NOOFTRIANGLES * 3) == (NOOFTRIANGLES * 3) - 1)
//            second_last = indexBuffer[j - 1];
//    }
//
//    //setting up the buffers
//
//    int success;
//    char info[512];
//
//    ID.vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(ID.vertexShader, 1, &vertexshader, NULL);
//    glCompileShader(ID.vertexShader);
//
//    glGetShaderiv(ID.vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(ID.vertexShader, 512, NULL, info);
//        std::cout << "compilation failed :: vertex shader " << std::endl << info << std::endl;
//    }
//
//    ID.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(ID.fragmentShader, 1, &fragmentshader, NULL);
//    glCompileShader(ID.fragmentShader);
//
//    glGetShaderiv(ID.fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(ID.fragmentShader, 512, NULL, info);
//        std::cout << "compilation failed :: fragment shader " << std::endl << info << std::endl;
//    }
//
//    ID.shaderProgram = glCreateProgram();
//    glAttachShader(ID.shaderProgram, ID.vertexShader);
//    glAttachShader(ID.shaderProgram, ID.fragmentShader);
//    glLinkProgram(ID.shaderProgram);
//
//    glGetProgramiv(ID.shaderProgram, GL_LINK_STATUS, &success);
//    if (!success)
//    {
//        glGetProgramInfoLog(ID.shaderProgram, 512, NULL, info);
//        std::cout << "linking failed " << std::endl << info << std::endl;
//    }
//
//    glGenVertexArrays(1, &ID.VAO);
//    glBindVertexArray(ID.VAO);
//
//    glGenBuffers(1, &ID.VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, ID.VBO);
//    //glBufferData(GL_ARRAY_BUFFER, 2 * (NOOFTRIANGLES + 1) * sizeof(float), vertexBuffer, GL_STATIC_DRAW);
//
//    GLCALL(glGenBuffers(1, &ID.IBO));
//    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID.IBO));
//    //GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * NOOFTRIANGLES * sizeof(unsigned int), indexBuffer, GL_STATIC_DRAW));
//
//
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//
//    glUseProgram(ID.shaderProgram);
//
//}
//
//particleInjector::~particleInjector()
//{
//    glDeleteShader(ID.vertexShader);
//    glDeleteShader(ID.fragmentShader);
//
//    GLCALL(glDeleteBuffers(1, &ID.VBO));
//    GLCALL(glDeleteBuffers(1, &ID.IBO));
//    GLCALL(glDeleteVertexArrays(1, &ID.VAO));
//}
//
//void particleInjector::injectParticles(float x,float y)
//{
//    //unsigned int threads = (m_count >= 32 )? 32 : m_count;
//    //unsigned int blocks = ceil((float)m_count/32.0f);
//
//    if (m_count == MAX_PARTICLES)
//        return;
//
//    m_particles[m_count].injected = true;
//    m_particles[m_count].m_position = glm::vec3(x - (float)2.0 * CIRCLESIZE * 10.0 / 960.0, y, 1.0);
//    for (unsigned int i = m_count * ((NOOFTRIANGLES + 1) * 2); i < (m_count + 1) * ((NOOFTRIANGLES + 1) * 2); i += 2)
//    {
//        vertexBuffer[i] = vertexBuffer[i] + m_particles[m_count].m_position.x;
//        vertexBuffer[i+1] = vertexBuffer[i+1] + m_particles[m_count].m_position.y;
//    }
//    m_count++;
//    
//    m_particles[m_count].injected = true;
//    m_particles[m_count].m_position = glm::vec3(x - (float)2.0 * CIRCLESIZE * 5.0 / 960.0, y, 1.0);
//    for (unsigned int i = m_count * ((NOOFTRIANGLES + 1) * 2); i < (m_count + 1) * ((NOOFTRIANGLES + 1) * 2); i += 2)
//    {
//        vertexBuffer[i] = vertexBuffer[i] + m_particles[m_count].m_position.x;
//        vertexBuffer[i + 1] = vertexBuffer[i + 1] + m_particles[m_count].m_position.y;
//    }
//    m_count++;
//
//    m_particles[m_count].injected = true;
//    m_particles[m_count].m_position = glm::vec3(x , y, 1.0);
//    for (unsigned int i = m_count * ((NOOFTRIANGLES + 1) * 2); i < (m_count + 1) * ((NOOFTRIANGLES + 1) * 2); i += 2)
//    {
//        vertexBuffer[i] = vertexBuffer[i] + m_particles[m_count].m_position.x;
//        vertexBuffer[i + 1] = vertexBuffer[i + 1] + m_particles[m_count].m_position.y;
//    }
//    m_count++;
//
//    m_particles[m_count].injected = true;
//    m_particles[m_count].m_position = glm::vec3(x + (float)2.0 * CIRCLESIZE * 5.0 / 960.0, y, 1.0);
//    for (unsigned int i = m_count * ((NOOFTRIANGLES + 1) * 2); i < (m_count + 1) * ((NOOFTRIANGLES + 1) * 2); i += 2)
//    {
//        vertexBuffer[i] = vertexBuffer[i] + m_particles[m_count].m_position.x;
//        vertexBuffer[i + 1] = vertexBuffer[i + 1] + m_particles[m_count].m_position.y;
//    }
//    m_count++;
//
//    m_particles[m_count].injected = true;
//    m_particles[m_count].m_position = glm::vec3(x + (float)2.0 * CIRCLESIZE * 10.0 / 960.0, y, 1.0);
//    for (unsigned int i = m_count * ((NOOFTRIANGLES + 1) * 2); i < (m_count + 1) * ((NOOFTRIANGLES + 1) * 2); i += 2)
//    {
//        vertexBuffer[i] = vertexBuffer[i] + m_particles[m_count].m_position.x;
//        vertexBuffer[i + 1] = vertexBuffer[i + 1] + m_particles[m_count].m_position.y;
//    }
//    m_count++;
//
//    std::cout << m_count;
//
//}
//
//void particleInjector::renderParticles()
//{
//    GLCALL(glBufferData(GL_ARRAY_BUFFER, m_count*((NOOFTRIANGLES + 1) * 2) * sizeof(float), vertexBuffer, GL_STATIC_DRAW));
//    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count*(3 * NOOFTRIANGLES) * sizeof(unsigned int), indexBuffer, GL_STATIC_DRAW));
//    GLCALL(glDrawElements(GL_TRIANGLES, NOOFTRIANGLES * 3* m_count, GL_UNSIGNED_INT, 0));
//}
