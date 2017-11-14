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
	int shift;
	int seed;
	int X_MULT = 49632;
	int Z_MULT = 325176;
	float pi = 3.14159265359f;
	float** noise_map;
	float** smooth_noise_map;

    public:
	Terrian() {};
	Terrian(int, int, int, int);
	std::vector<glm::vec3> generateMap();
	float getHeight(float, float);
	float calcNoise(int, int);
	float getNoise(int, int);
	float calcSmoothNoise(int, int);
	float getSmoothNoise(int, int);
	float interpolate(float, float, float);
	float getInterpolatedNoise(float, float);
	vector<GLuint> findIndices();
	void preCalculateMaps();
};

#endif
