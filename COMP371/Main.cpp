/*
 * COMP 371: Final Project
 * October 12, 2017
 * Skeleton Adopted and Modified
 * From learnopengl.com
 */


#include "stdio.h"
#include "iostream"
#include "string"
#include "cstring"
#include "fstream"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "vertex_array_object.hpp"
#include "gl_utils.hpp"

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

// Configurable Constant Variables

// Constant Variables
const float PI = 3.14159265359f;
const float BACKGROUND_COLOR = 0.4f; 
const float PROJ_FAR_PLANE = 100.0f;
const GLuint INITIAL_WIDTH = 1280;
const GLuint INITIAL_HEIGHT = 720;
const glm::mat4 IDENTITY = glm::mat4(1.0f);
const glm::mat4 ORIGIN = glm::mat4(0.0f);

// Global Variables
bool close_window = false;
std::vector<VertexArrayObject> vaos;
glm::vec3 center(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 eye(0.0f, 0.0f, 5.0f);


// Prototypes definition
void initGl(void);
void drawGl(void);
void keyCallback(GLFWwindow*, int, int, int, int);
void registerCallbacks(GLFWwindow*);
void windowSizeCallback(GLFWwindow*, int, int); 
glm::mat4 setCameraPosition(void);


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    switch (key) {
	case GLFW_KEY_X:
	    close_window = true;
	    break;
    }
}


void initGl() {

    // Set Projection Matrix 
    glm::mat4 projection_matrix = glm::perspective(45.0f, (GLfloat)INITIAL_WIDTH / (GLfloat)INITIAL_HEIGHT, 1.0f, PROJ_FAR_PLANE);
    VertexArrayObject::setProjectionMatrix(projection_matrix);

    std::vector<glm::vec3> vertices;
    std::vector<GLuint> edges;
    VertexArrayObject vao = VertexArrayObject();

    // Example : Drawing One Triangle
    // Using Vertices Only (No indices)
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    vertices.push_back(up);
    vertices.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    vao.setGeometry(vertices);
    vao.setDrawingMode(VertexArrayObject::VERTICES);
    vao.setPrimitive(VertexArrayObject::TRIANGLES);
    vaos.push_back(vao);
    vao.clear(); vertices.clear();


    
    // Example: Drawing Three Triangels
    // Using indices
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));	//0
    vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));	//1
    vertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));	//2
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));	//3
    vertices.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));	//4
    vertices.push_back(glm::vec3(0.0f, -1.0f, 0.0f));	//5
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));	//6
    vertices.push_back(glm::vec3(-0.5f, 1.0f, 0.0f));	//7
    vertices.push_back(glm::vec3(-1.0f, 0.5f, 0.0f));	//8
    
    edges.push_back(0); edges.push_back(1); edges.push_back(2); edges.push_back(3); 
    edges.push_back(4); edges.push_back(5); edges.push_back(6); 
    edges.push_back(7); edges.push_back(8); 

    vao.setGeometry(vertices);
    vao.setTopology(edges);
    vao.setDrawingMode(VertexArrayObject::ELEMENTS);
    vao.setPrimitive(VertexArrayObject::TRIANGLES);
    glm::mat4 translate = glm::translate(IDENTITY, glm::vec3(2.0f, 0.0f, 0.0f));
    vao.setModelMatrix(translate);
    vaos.push_back(vao);
    vao.clear(); vertices.clear(); edges.clear();

}

void drawGl() {

    // One View Matrix per Iteration
    glm::mat4 view_matrix = setCameraPosition();
    VertexArrayObject::setViewMatrix(view_matrix);

    // Draw All Objects
    for (unsigned int i = 0; i < vaos.size(); i++) {
	vaos[i].draw();
    }
}


int main() {

    // Initialize Window
    GLFWwindow* window = GlUtilities::setupGlWindow(INITIAL_WIDTH, INITIAL_HEIGHT); 
    if (window == NULL) return -1;

    // Register Callbacks
    registerCallbacks(window);

    // Shaders
    GLuint shader_program = GlUtilities::loadShaders();
    VertexArrayObject::registerShaderProgram(shader_program);

    // Run Objects Initialization
    initGl();

    // Main OpenGL Loop 
    while (!glfwWindowShouldClose(window) && !close_window)
    {
	glfwPollEvents();
	glClearColor(BACKGROUND_COLOR, BACKGROUND_COLOR, BACKGROUND_COLOR, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Run Drawing function
	drawGl();

	glfwSwapBuffers(window);
	glfwSwapInterval(1);
    }

    glfwTerminate();
    return 0;
}

glm::mat4 setCameraPosition() {

    glm::mat4 view_matrix;
    view_matrix = glm::lookAt(eye, center, up);
    return view_matrix;
}

void registerCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
}

void windowSizeCallback(GLFWwindow* window, int width, int height) {
    // Define the new Viewport Dimensions{
    glfwGetFramebufferSize(window, & width, &height);
    glViewport(0, 0, width, height);
    glm::mat4 projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, PROJ_FAR_PLANE);
    VertexArrayObject::setProjectionMatrix(projection_matrix);
}
