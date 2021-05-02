#include<glew/include/GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>
#include"Header/error_check.h"
#include<chrono>
#include<thread>
#include"Header/mouse.h"
#include"Header/particleInjector.h"
#include"Header/parameters.h"

//#include"device_launch_parameters.h"
//#include<cuda_runtime.h>

const char* vertexShader =
"#version 330 core\n"
"layout(location = 0) in vec4 position;\n"
"void main(){\n"
"gl_Position = position;\n"
"}\0\n";

const char* fragmentShader =
"#version 330 core\n"
"out vec4 color;\n"
"void main(){\n"
"color = vec4(1.0f , 0.5f , 0.2f , 1.0f);\n"
"}\0\n";


bool release{ false };

	void updateVBPositions(glm::vec3 dist, unsigned int pos, float* vertexbuffer)
	{
		unsigned int vb_pos = (NOOFTRIANGLES + 1) * 2 * pos;

		for (; vb_pos < (NOOFTRIANGLES + 1) * 2 * (pos + 1); vb_pos += 2)
		{
			vertexbuffer[vb_pos] = vertexbuffer[vb_pos] + dist.x;
			vertexbuffer[vb_pos + 1] = vertexbuffer[vb_pos + 1] + dist.y;
			//printf("position: %i and position in vertex buffer: %i\n", pos, vb_pos);
		}
	}

	void calcPosition(particle* particles, float* vertexbuffer,unsigned int count)
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

                updateVBPositions(dist, pos, vertexbuffer);
            }
        }
	}

	void calcVelocity(particle* particles, unsigned int count)
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

	void calcAccelaration(particle* particles, unsigned int count)
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

	void collisionCheck(particle* particles, unsigned int count)
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


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);

    //setting callbacks
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        return 1;
    }

    /*
    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[2 * (NOOFTRIANGLES + 1)]{ -0.5,-0.5 };

    //getting the points of a circle
    int k = 2;
    std::cout << "(" << positions[0] << "," << positions[1] << ")" << std::endl;
    for (int angle = 0; angle < 360; angle += 360 / NOOFTRIANGLES)
    {
        positions[k] = (CIRCLESIZE * std::cos((PI / (float)180) * angle) / (float)960) - 0.5;
        positions[k + 1] = (CIRCLESIZE * std::sin((PI / (float)180) * angle) / (float)540) - 0.5;
        std::cout << "(" << positions[k] << "," << positions[k + 1] << ")" << std::endl;
        k += 2;

    }

    unsigned int indices[3 * NOOFTRIANGLES]{ 0 };
    unsigned int temp = 1;
    for (int i = 0; i < 3 * NOOFTRIANGLES - 1; i++)
    {
        if (i % 3 == 0)
            indices[i] = 0;
        else if (i % 3 == 1)
        {
            indices[i] = temp;
            temp++;
        }
        else
        {
            indices[i] = temp;
        }
        std::cout << indices[i] << std::endl;
    }
    indices[3 * NOOFTRIANGLES - 1] = 1;

    int success;
    char info[512];

    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexShader, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info);
        std::cout << "compilation failed :: vertex shader " << std::endl << info << std::endl;
    }

    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentShader, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info);
        std::cout << "compilation failed :: fragment shader " << std::endl << info << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex_shader);
    glAttachShader(shaderProgram, fragment_shader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, info);
        std::cout << "linking failed " << std::endl << info << std::endl;
    }

    unsigned int vertexarrayID;
    glGenVertexArrays(1, &vertexarrayID);
    glBindVertexArray(vertexarrayID);

    unsigned int vertexBufferID;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, 2 * (NOOFTRIANGLES + 1) * sizeof(float), positions, GL_DYNAMIC_DRAW);

    unsigned int indexBufferID;
    GLCALL(glGenBuffers(1, &indexBufferID));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * NOOFTRIANGLES * sizeof(unsigned int), indices, GL_STATIC_DRAW));


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glUseProgram(shaderProgram);

    */

    //initializing the particle injector
    particleInjector particleinjector;
    particleinjector.init();

    unsigned int count;
    //particle* d_particles;
    //float* d_vertexbufferdata;
    particle* particles(particleinjector.getParticlePointer());
    float* vertexbufferdata(particleinjector.getVertexBufferData());

    //cudaMalloc(&d_particles,MAX_PARTICLES*sizeof(particle));
    //cudaMalloc(&d_vertexbufferdata, MAX_PARTICLES* ((NOOFTRIANGLES + 1) * 2)*sizeof(float));
    //cudaMemcpy(d_particles,particles, MAX_PARTICLES * sizeof(particle),cudaMemcpyHostToDevice);
    //cudaMemcpy(d_vertexbufferdata, vertexbufferdata, MAX_PARTICLES* ((NOOFTRIANGLES + 1) * 2) * sizeof(float), cudaMemcpyHostToDevice);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        handleMouseInputEvent(window,particleinjector);

        //calculations using cuda
        count = particleinjector.getParticleCount();
        unsigned int threads = (count >= 32 )? 32 : count;
        unsigned int blocks = (unsigned int)ceil((float)count/32.0f);



        //cudaMemcpy(d_particles, particles, MAX_PARTICLES * sizeof(particle), cudaMemcpyHostToDevice);
        //cudaMemcpy(d_vertexbufferdata, vertexbufferdata, MAX_PARTICLES * ((NOOFTRIANGLES + 1) * 2) * sizeof(float), cudaMemcpyHostToDevice);
        //cuda::calcAccelaration<<<blocks, threads>>>(d_particles);
        //cuda::calcVelocity<<<blocks, threads>>>(d_particles);
        //cuda::calcPosition<<<blocks, threads>>>(d_particles, d_vertexbufferdata);
        //cudaDeviceSynchronize();
        //cuda::collisionCheck<<<blocks, threads>>>(d_particles);
        //cudaMemcpy(particles, d_particles, MAX_PARTICLES * sizeof(particle), cudaMemcpyDeviceToHost);
        //cudaMemcpy(vertexbufferdata, d_vertexbufferdata, MAX_PARTICLES * ((NOOFTRIANGLES + 1) * 2) * sizeof(float), cudaMemcpyDeviceToHost);

        calcAccelaration(particles,count);
        calcVelocity(particles, count);
        calcPosition(particles, vertexbufferdata, count);
        collisionCheck(particles, count);


        for (int h = 0; h < 10; h+=2)
            std::cout << vertexbufferdata[h] <<","<< vertexbufferdata[h+1] << std::endl;

        //rendering
        particleinjector.renderParticles();

        /*
        for (int t = 0; t < 2 * (NOOFTRIANGLES + 1); t+=2)
        {
            positions[t] += 0.001f;

            positions[t + 1] += 0.001f;
        }
        glBufferData(GL_ARRAY_BUFFER, 2 * (NOOFTRIANGLES + 1) * sizeof(float), positions, GL_DYNAMIC_DRAW);

        //glDrawArrays(GL_TRIANGLES, 0, (NOOFTRIANGLES + 1));
        GLCALL(glDrawElements(GL_TRIANGLES, NOOFTRIANGLES * 3, GL_UNSIGNED_INT, 0));
        */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /*
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    GLCALL(glDeleteBuffers(1, &vertexBufferID));
    GLCALL(glDeleteBuffers(1, &indexBufferID));
    GLCALL(glDeleteVertexArrays(1, &vertexarrayID));
    */

    //cudaFree(d_particles);
    //cudaFree(d_vertexbufferdata);

    glfwTerminate();
    return 0;
}

