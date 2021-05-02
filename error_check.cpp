#include "error_check.h"

#include "error_check.h"
#include<iostream>

void glclearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool glcheckError(const char* function, const char* file, int line) {
    bool err = false;
    while (GLenum glerror = glGetError()) {
        std::cout << file << " : " << function << " : " << line << " : " << "opengl error : " << glerror << std::endl;
        err = true;
    }

    return err;
}