#ifndef _TERRIAN_H
#define _TERRIAN_H

#include "stdlib.h"
#include "stdio.h"
#include "iostream"
#include "vector"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#ifdef __linux__ 
    //linux code goes here
    #include "GL/glew.h"        // include GL Extension Wrangler
    #include "GLFW/glfw3.h"     // include GLFW helper library
#elif _WIN32
    // windows code goes here
    #include "..\glew\glew.h"   // include GL Extension Wrangler
    #include "..\glfw\glfw3.h"  // include GLFW helper library
#endif


using namespace std;

class Terrian {
    
    private:
	int width, height;
	int max;
	float step;
	int seed;
	int X_MULT = 23562;
	int Z_MULT = 512351;

    public:
	Terrian() {};
	Terrian(int, int, float, int);
	std::vector<glm::vec3> generateMap();
	float getHeight(float, float);
	float getNoise(int, int);
	float getSmoothNoise(int, int);
	vector<GLuint> findIndices();
};

#endif
