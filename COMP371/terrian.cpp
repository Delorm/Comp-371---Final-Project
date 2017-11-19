#include "terrian.hpp"
#include "time.h"

using namespace std;

Terrian::Terrian(int width, int height, int max, int shift) {
    this->width = width;
    this->height = height;
    this->max = max;
    this->shift = shift;
    seed = time(NULL);
    preCalculateMaps();
}

std::vector<glm::vec3> Terrian::generateMap() {

    std::vector<glm::vec3> vertices;
    for (float z = 0; z < height; z++) {
	for (float x = 0; x < width; x++) {
	    float y = getHeight(x, z);
	    glm::vec3 point = glm::vec3(x, y, z);
	    vertices.push_back(point);
	}
    }
    return vertices;
}

float Terrian::getHeight(float x, float z) {

    float total = getInterpolatedNoise(x / 16.0f, z / 16.0f) * max;
    total += getInterpolatedNoise(x / 8.0f, z / 8.0f) * (max / 3.0f);
    return total + shift;
}

float Terrian::calcNoise(int x, int z) {
    srand(x * X_MULT + z * Z_MULT + seed);
    float y = ((float)rand() / RAND_MAX * 2) - 1;
    return y;
}

float Terrian::getNoise(int x, int z) {
    if (x < 0 || z < 0 || x >= width || z >= height) {
	return calcNoise(x, z);
    }	
    return noise_map[x][z];
}


float Terrian::calcSmoothNoise(int x, int z) {
    float corners = (Terrian::getNoise(x-1, z-1) + Terrian::getNoise(x-1, z+1) + Terrian::getNoise(x+1, z-1) + Terrian::getNoise(x+1, z+1)) / 16.0f;
    float sides = (Terrian::getNoise(x-1, z) + Terrian::getNoise(x+1, z) + Terrian::getNoise(x, z-1) + Terrian::getNoise(x, z+1)) / 8.0f;
    float center = Terrian::getNoise(x, z) / 4.0f;
    return corners + sides + center;
}

float Terrian::getSmoothNoise(int x, int z) {
    if (x < 0 || z < 0 || x >= width || z >= height) {
	return calcSmoothNoise(x, z);
    }	
    return smooth_noise_map[x][z];
}

float Terrian::interpolate(float a, float b, float blend) {
    float theta = blend * pi;
    float f = (1.0f - cos(theta)) * 0.5f;
    return a * (1.0f - f) + b * f;
}

float Terrian::getInterpolatedNoise(float x, float z) {
    int int_x = (int)x;
    int int_z = (int)z;
    float frac_x = x - int_x;
    float frac_z = z - int_z;

    float p0 = getSmoothNoise(int_x, int_z);
    float p1 = getSmoothNoise(int_x + 1, int_z);
    float p2 = getSmoothNoise(int_x, int_z + 1);
    float p3 = getSmoothNoise(int_x + 1, int_z + 1);

    float i1 = interpolate(p0, p1, frac_x);
    float i2 = interpolate(p2, p3, frac_x);
    float i  = interpolate(i1, i2, frac_z);
    return i;
}

std::vector<GLuint> Terrian::findIndices() {

    std::vector<GLuint> indices;

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

void Terrian::preCalculateMaps() {

    noise_map = new float* [width];
    smooth_noise_map = new float* [width];
    for (int i = 0; i < width; i++) {
	noise_map[i] = new float [height];
	smooth_noise_map[i] = new float[height];
    }

    // Noise
    for (int x = 0; x < width; x++) {
	for (int z = 0; z < height; z++) {
	    noise_map[x][z] = calcNoise(x, z);
	}
    }

    // Smooth Noise
    for (int x = 0; x < width; x++) {
	for (int z = 0; z < height; z++) {
	    smooth_noise_map[x][z] = calcSmoothNoise(x, z);
	}
    }
}

std::vector<glm::vec2> Terrian::generateUVs() {

    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> map = generateMap();
    for (int i = 0; i < map.size(); i++) {
	uvs.push_back(glm::vec2(map[i].x / width, map[i].z / height));
    }
    return uvs;
}
