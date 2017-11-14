/*
 * October 13, 2017
 * Skeleton Adopted and Modified
 * From learnopengl.com
 */
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "vertex_array_object.hpp"
#include "stdio.h"
#include "iostream"
#include "stdlib.h"

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

// Static Initilization
const char* VertexArrayObject::MVP_SHADER_NAME = "mvp_matrix";
glm::mat4 VertexArrayObject::v_matrix = glm::mat4(1.0f);
glm::mat4 VertexArrayObject::p_matrix = glm::mat4(1.0f);
glm::mat4 VertexArrayObject::vp_matrix = glm::mat4(1.0f);

// Basic Rountines
VertexArrayObject::VertexArrayObject() {
    this->clear();
}


VertexArrayObject::~VertexArrayObject() {

}

void VertexArrayObject::clear(void) {
    m_matrix = glm::mat4(1.0f);
    glGenVertexArrays(1, &vao_loc);
    visibility = true;
    drawing_mode = ELEMENTS;
    primitive = TRIANGLES;
}

void VertexArrayObject::setVisibility(bool visibility) {
    this->visibility = visibility;
}

void VertexArrayObject::setDrawingMode(modes drawing_mode) {
   this->drawing_mode = drawing_mode; 
}

void VertexArrayObject::setPrimitive(modes primitive) {
    this->primitive = primitive;
}

void VertexArrayObject::setViewMatrix(glm::mat4 & new_view_matrix) {
    v_matrix = glm::mat4(new_view_matrix);
    vp_matrix = p_matrix * v_matrix;
}

void VertexArrayObject::setProjectionMatrix(glm::mat4 & new_proj_matrix) {
    p_matrix = glm::mat4(new_proj_matrix);
    vp_matrix = p_matrix * v_matrix;
}

void VertexArrayObject::setModelMatrix(glm::mat4 & new_model_matrix) {
    m_matrix = glm::mat4(new_model_matrix);
}

void VertexArrayObject::registerShaderProgram(GLuint new_shader_program) {
    shader_program = new_shader_program;
    mvp_loc = glGetUniformLocation(new_shader_program, MVP_SHADER_NAME);
}


// Specialized Routines
void VertexArrayObject::setGeometry(std::vector<glm::vec3>& vertices) {

    geometry_size = vertices.size();
    GLuint vbo_loc;
    glGenBuffers(1, &vbo_loc);
    glBindVertexArray(vao_loc);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_loc);
    glBufferData(
	    GL_ARRAY_BUFFER, 
	    vertices.size() * sizeof(glm::vec3), 
	    &vertices.front(), 
	    GL_STATIC_DRAW
	    );
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void VertexArrayObject::setTopology(std::vector<GLuint>& edges) {

    topology_size = edges.size();
    GLuint ebo_loc;
    glBindVertexArray(vao_loc);
    glGenBuffers(1, &ebo_loc);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_loc);
    glBufferData(
	    GL_ELEMENT_ARRAY_BUFFER, 
	    edges.size() * sizeof(GLuint), 
	    &edges.front(), 
	    GL_STATIC_DRAW
	    );
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void VertexArrayObject::draw() {
    if (!visibility) return;

    glm::mat4 mvp_matrix = vp_matrix * m_matrix;
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp_matrix));

    int renderring_mode = (primitive == POINTS) ? GL_POINTS : GL_TRIANGLES;
    glBindVertexArray(vao_loc);
    if (drawing_mode == VERTICES) {
	glDrawArrays(renderring_mode, 0, geometry_size);
    } else {
	glDrawElements(renderring_mode, topology_size, GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
}

