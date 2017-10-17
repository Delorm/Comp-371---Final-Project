/*
 * Author: Adnan Utayim
 * October 12, 2017
 * Code Adopted and Modified
 * From learnopengl.com
 */

#ifndef _GL_UTILS_H
#define _GL_UTILS_H


#include "GL/glew.h"
#include "GLFW/glfw3.h"

class GlUtilities {
    public: 
	static GLuint loadShaders();
	static GLFWwindow* setupGlWindow(const GLuint & width, const GLuint & height);
};

#endif
