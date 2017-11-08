/*
 * Author: Adnan Utayim
 * October 12, 2017
 * Code Adopted and Modified
 * From learnopengl.com
 */

#ifndef _GL_UTILS_H
#define _GL_UTILS_H

#include "iostream"
#include "stdio.h"
#include "stdlib.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "vector"
#ifdef __linux__ 
    //linux code goes here
    #include "GL/glew.h"	// include GL Extension Wrangler
    #include "GLFW/glfw3.h"	// include GLFW helper library
#elif _WIN32
    // windows code goes here
    #include "..\glew\glew.h"	// include GL Extension Wrangler
    #include "..\glfw\glfw3.h"	// include GLFW helper library
#endif

using namespace std;

class GlUtilities {
    public: 
	static GLuint loadShaders();
	static GLFWwindow* setupGlWindow(GLuint width, GLuint height);
	static void createTerrain(vector<glm::vec3> &, vector<unsigned int> &, int &, int &);
	static vector<GLuint> findIndices(int, int);
};

#endif
