#pragma once
#include<glew/include/GL/glew.h>

#define ASSERT(X) if(X) __debugbreak();
#define GLCALL(X) glclearError(); X; ASSERT(glcheckError(#X,__FILE__,__LINE__))

void glclearError();
bool glcheckError(const char* function, const char* file, int line);



