#include "..\Header\Renderer.h"
#include"..\Header\mouse.h"
#include"..\Header\ParticleSimulation.h"

Renderer::Renderer()
{

    /* Initialize the library */
    if (!glfwInit())
        return ;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);

    //setting callbacks
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    if (!window)
    {
        glfwTerminate();
        return ;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        return ;
    }

    UI = new ParticleSimulation();
}

void Renderer::Start()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        handleMouseInputEvent(window, UI);


        /* 
        * cuda code - keep this here for later integration
        calculations using cuda
        count = particleinjector.getParticleCount();
        unsigned int threads = (count >= 32) ? 32 : count;
        unsigned int blocks = (unsigned int)ceil((float)count / 32.0f);



        cudaMemcpy(d_particles, particles, MAX_PARTICLES * sizeof(particle), cudaMemcpyHostToDevice);
        cudaMemcpy(d_vertexbufferdata, vertexbufferdata, MAX_PARTICLES * ((NOOFTRIANGLES + 1) * 2) * sizeof(float), cudaMemcpyHostToDevice);
        cuda::calcAccelaration<<<blocks, threads>>>(d_particles);
        cuda::calcVelocity<<<blocks, threads>>>(d_particles);
        cuda::calcPosition<<<blocks, threads>>>(d_particles, d_vertexbufferdata);
        cudaDeviceSynchronize();
        cuda::collisionCheck<<<blocks, threads>>>(d_particles);
        cudaMemcpy(particles, d_particles, MAX_PARTICLES * sizeof(particle), cudaMemcpyDeviceToHost);
        cudaMemcpy(vertexbufferdata, d_vertexbufferdata, MAX_PARTICLES * ((NOOFTRIANGLES + 1) * 2) * sizeof(float), cudaMemcpyDeviceToHost);
         
        */

        UI->UpdateVertexBuffers();
        UI->render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    //cudaFree(d_particles);
    //cudaFree(d_vertexbufferdata);

    glfwTerminate();
}

Renderer::~Renderer()
{
    delete UI;
}
