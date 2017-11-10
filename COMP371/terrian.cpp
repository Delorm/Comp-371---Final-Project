#include "terrian.hpp"
#include "time.h"

using namespace std;

Terrian::Terrian(int width, int height, float step, int max) {
    this->width = width;
    this->height = height;
    this->step = step;
    this->max = max;
    seed = (time(NULL));
}

std::vector<glm::vec3> Terrian::generateMap() {
    std::vector<glm::vec3> vertices;
    for (float z = 0; z < height; z += step ) {
	for (float x = 0; x < width; x += step) {
	    float y = getHeight(x, z);
	    glm::vec3 point = glm::vec3(x, y, z);
	    vertices.push_back(point);
	}
    }
    return vertices;
}

float Terrian::getHeight(float x, float z) {

    float y = Terrian::getNoise(x, z) * max;
    return y;
}

float Terrian::getNoise(int x, int z) {
    srand(x * X_MULT + z * Z_MULT + seed);
    float y = ((float)rand() / RAND_MAX * 2) - 1;
    return y;
}

float Terrian::getSmoothNoise(int x, int z) {

}


std::vector<GLuint> Terrian::findIndices() {

    std::vector<GLuint> indices;

    // First Pass
    for (int i = 0; i < width * (height - 1); i++) {
	int x = i % width;
	int y = i / width;
	if (x + 1 == width) continue;
	indices.push_back(i);
	indices.push_back((y + 1) * width + x); 
	indices.push_back(i+1);
    }   

    // Second Pass
    for (int i = width; i < width * height; i++) {
	int x = i % width;
	int y = i / width;
	if (x + 1 == width) continue;
	indices.push_back(i);
	indices.push_back(i+1);
	indices.push_back((y - 1) * width + (x + 1));
    }   

    return indices;
}

