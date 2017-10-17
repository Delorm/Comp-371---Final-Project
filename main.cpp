/*
 * Author: Adnan Utayim
 * October 12, 2017
 * Skeleton Adopted and Modified
 * From learnopengl.com
 */

#include "GL/glew.h"	
#include "GLFW/glfw3.h"	
#include "stdio.h"
#include "iostream"
#include "string"
#include "fstream"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "gl_utils.hpp"
#include "vertex_array_object.hpp"

using namespace std;


// Tweakable Constant Variables

// Constant Variables
const GLuint INITIAL_WIDTH = 800;
const GLuint INITIAL_HEIGHT = 800;
const float BACKGROUND_COLOR = 0.4f; 
const float POINT_SIZE = 5.0f;
const glm::mat4 IDENTITY = glm::mat4(1.0f);

// Global Variables
bool close_window = false;
std::vector<VertexArrayObject> vaos;


glm::vec3 center(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 eye(0.0f, 0.0f, 3.0f);


// Prototypes definition
void keyCallback(GLFWwindow* window, int key, int scandoe, int action, int mode);
glm::mat4 setCameraPosition(void);
void initGl(void);
void drawGl(void);
void registerCallbacks(GLFWwindow* window);


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    switch (key) {
	case GLFW_KEY_X:
	    close_window = true;
	    break;
    }
}


void initGl() {

    // Set Projection Matrix 
    glm::mat4 projection_matrix = glm::perspective(45.0f, (GLfloat)INITIAL_WIDTH / (GLfloat)INITIAL_HEIGHT, 0.0f, 100.0f);
    VertexArrayObject::setProjectionMatrix(projection_matrix);



    std::vector<glm::vec3> vertices;
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));	//0
    vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));	//1
    vertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));	//2
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));	//3
    vertices.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));	//4
    vertices.push_back(glm::vec3(0.0f, -1.0f, 0.0f));	//5
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));	//6
    vertices.push_back(glm::vec3(-0.5f, 1.0f, 0.0f));	//7
    vertices.push_back(glm::vec3(-1.0f, 0.5f, 0.0f));	//8
    
    std::vector<GLushort> edges;
    edges.push_back(0); edges.push_back(1); edges.push_back(2); edges.push_back(3); 
    edges.push_back(4); edges.push_back(5); edges.push_back(6); 
    edges.push_back(7); edges.push_back(8); 

    VertexArrayObject vao = VertexArrayObject();
    vao.setGeometry(vertices);
    vao.setTopology(edges);
    vao.setDrawingMode(VertexArrayObject::ELEMENTS);
    glm::mat4 translate = glm::translate(IDENTITY, glm::vec3(1.0f, 0.0f, 0.0f));
    vao.setModelMatrix(translate);
    vaos.push_back(vao);



}

void drawGl() {

    // One View Matrix per Iteration
    glm::mat4 view_matrix = setCameraPosition();
    VertexArrayObject::setViewMatrix(view_matrix);
  
    glPointSize(POINT_SIZE); 
    vaos[0].draw();


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
	glClear(GL_COLOR_BUFFER_BIT);

	// Run Drawing function
	drawGl();

	glfwSwapBuffers(window);
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
}
