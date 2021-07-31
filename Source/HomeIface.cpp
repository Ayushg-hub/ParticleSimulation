#include "..\Header\HomeIface.h"
#include"..\Header\error_check.h"


HomeIface::HomeIface()
{
	int success{ 0 };
	char info[512];

	ID.vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(ID.vertexShader, 1, &vertexshader,NULL);
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

	glGenVertexArrays(1, &ID.VAO);
	glBindVertexArray(ID.VAO);

	//creating vertex buffers
	glGenBuffers(1, &ID.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, ID.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, vertexBuffer, GL_STATIC_DRAW);

	//creating and binding index buffer
	glGenBuffers(1, &ID.IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 18, indexBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 2,
		(void*)0
	);

	glUseProgram(ID.shaderProgram);

}

UserIface* HomeIface::InputEventHandler()
{
	Mouse& mouse = Mouse::getInstance();

	mouseModes mMode = mouse.getMouseMode();
	mousePos mPos = mouse.getMousePosition();
	//unsigned int count = mouse.getPressCount();

	if (mMode == mouseModes::PRESSED)
	{
		//setting the right coordinate system
		mPos.x = mPos.x - SCREEN_WIDTH / 2;
		mPos.y = -(mPos.y - SCREEN_HEIGHT / 2);

		//normalizing
		mPos.x = mPos.x / (float)(SCREEN_WIDTH / 2);
		mPos.y = mPos.y / (float)(SCREEN_HEIGHT / 2);

		if (mPos.x >= -0.8f && mPos.x <= -0.4f && mPos.y >= -0.2f && mPos.y <= 0.2f)
		{
			return new ParticleSimulation();
			//HomeIface* P = static_cast<HomeIface*>(UI);
		}
	}

	return this;
}

void HomeIface::UpdateVertexBuffers()
{

}

void HomeIface::render()
{
	GLCALL(glDrawElements(GL_TRIANGLES, 18 , GL_UNSIGNED_INT,0));
}

IfaceType HomeIface::getIfaceType()
{
	return IFType;
}

std::string HomeIface::getType()
{
	return std::string("none");
}

HomeIface::~HomeIface()
{
	glDeleteShader(ID.vertexShader);
	glDeleteShader(ID.fragmentShader);

	GLCALL(glDeleteBuffers(1, &ID.VBO));
	GLCALL(glDeleteBuffers(1, &ID.IBO));
	GLCALL(glDeleteVertexArrays(1, &ID.VAO));
}
