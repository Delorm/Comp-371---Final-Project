/*
 * Author: Adnan Utayim
 * October 12, 2017
 * Code Adopted and Modified
 * From learnopengl.com
 */

#include "fstream"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "gl_utils.hpp"
#include "vertex_array_object.hpp"
#include "stdio.h"
#include "iostream"
#include "stdlib.h"
#include "stdio.h"
#include "cstring"
#include "string"
#include "CImg.hpp"
#include "vector"
#include "list"
#ifdef __linux__ 
    //linux code goes here
    #include "GL/glew.h"	// include GL Extension Wrangler
    #include "GLFW/glfw3.h"	// include GLFW helper library
    #include "../quickhull/QuickHull.hpp"
#elif _WIN32
    // windows code goes here
    #include "..\glew\glew.h"	// include GL Extension Wrangler
    #include "..\glfw\glfw3.h"	// include GLFW helper library
    #include "..\quickhull\QuickHull.hpp"
#endif

using namespace std;
using namespace quickhull;


const float MESH_MAX_HEIGHT = 50.0f;

GLuint GlUtilities::loadShaders(string vertexShaderFile, string fragmentShaderFile) {

    // Read, Compile, and Attach Shader Programs
    string vertex_shader_path = vertexShaderFile;
    string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_shader_path, ios::in);

    if (VertexShaderStream.is_open()) {
	string Line = "";
	while (getline(VertexShaderStream, Line))
	    VertexShaderCode += "\n" + Line;
	VertexShaderStream.close();
    }
    else {
	printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
	getchar();
	exit(-1);
    }

    // Read the Fragment Shader code from the file
    string fragment_shader_path = fragmentShaderFile;
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

    if (FragmentShaderStream.is_open()) {
	std::string Line = "";
	while (getline(FragmentShaderStream, Line))
	    FragmentShaderCode += "\n" + Line;
	FragmentShaderStream.close();
    }
    else {
	printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
	getchar();
	exit(-1);
    }   

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
	glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader); //free up memory
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    return shaderProgram;

}

GLFWwindow* GlUtilities::setupGlWindow(GLuint global_width, GLuint global_height) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GL_DEPTH_BITS, 24);

    GLFWwindow* window = glfwCreateWindow(global_width, global_height, "Load one cube", nullptr, nullptr);
    if (window == nullptr)
    {   
	std::cout << "Failed to create GLFW window" << std::endl;
	glfwTerminate();
	return NULL;
    }   
    glfwMakeContextCurrent(window);

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {   
	std::cout << "Failed to initialize GLEW" << std::endl;
	return NULL;
    }   

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    return window;
}


void GlUtilities::convexHull(std::vector<glm::vec3> & vertices, std::vector<unsigned int> & indices, std::vector<glm::vec3> & normals) {

    QuickHull<float> quickHull;
    std::vector<Vector3<float>> pointCloud;

    // Add vertices
    for (int i = 0; i < vertices.size(); i++) {
	glm::vec3 ver = vertices[i];
	Vector3<float> point(ver.x, ver.y, ver.z);
	pointCloud.push_back(point);
    }

    auto hull = quickHull.getConvexHull(pointCloud, true, false);
    auto indexBuffer = hull.getIndexBuffer();
    auto vertexBuffer = hull.getVertexBuffer();


    // Convert to glm
    vertices.clear();

    for (int i = 0; i < vertexBuffer.size(); i++) {
	Vector3<float> vec = vertexBuffer[i];
	glm::vec3 point = glm::vec3(vec.x, vec.y, vec.z);
	vertices.push_back(point);
    }

    for (int i = 0; i < indexBuffer.size(); i++) {
	indices.push_back(indexBuffer[i]);
    }
    
    glm::vec3* arr = new glm::vec3 [vertices.size()];

    for (int i = 0; i < indices.size(); i += 3) {
	glm::vec3 p1 = vertices[indices[i+0]];
	glm::vec3 p2 = vertices[indices[i+1]];
	glm::vec3 p3 = vertices[indices[i+2]];

	glm::vec3 u1 = p2 - p1; 
	glm::vec3 u2 = p3 - p1; 
	glm::vec3 n  = glm::normalize(glm::cross(u1, u2));
	arr[indices[i+0]] = n;
	arr[indices[i+1]] = n;
	arr[indices[i+2]] = n;
    }   
    normals = std::vector<glm::vec3>(arr, arr + vertices.size());
    delete [] arr;

}


std::vector<glm::vec3> GlUtilities::genRandomRock(float max_r, int points) {

    std::vector<glm::vec3> vertices;
    max_r = sqrt(max_r);
    float r = (float)rand() / RAND_MAX * max_r;
    for (int i = 0; i < points; i++) {
	float px = (((float)rand() / RAND_MAX) * 2.0f * r) - (r);
	float py = (((float)rand() / RAND_MAX) * 2.0f * r) - (r);
	float pz = (((float)rand() / RAND_MAX) * 2.0f * r) - (r);
	vertices.push_back(glm::vec3(px, py, pz));
    }
    return vertices;
}

const float PI = 3.14159265359f;
std::vector<glm::vec2> GlUtilities::genSphericalUVs(std::vector<glm::vec3> & vertices) {

    std::vector<glm::vec2> uvs;
    for (int i = 0; i < vertices.size(); i++) {
	glm::vec3 point = vertices[i];
	glm::vec3 normal = glm::normalize(point);
	float tu = atan2(normal.x, normal.z) / (2.0f*PI) + 0.5f;
	float tv = normal.y * 0.5f + 0.5f;
	uvs.push_back(glm::vec2(tu, tv));
    }
    return uvs;
}

