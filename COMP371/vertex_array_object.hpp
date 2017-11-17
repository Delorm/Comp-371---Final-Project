/*
 * October 13, 2017
 * Skeleton Adopted and Modified
 * From learnopengl.com
 */

#ifndef _VERTEX_ARRAY_OBJECT_H
#define _VERTEX_ARRAY_OBJECT_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "string"
#include "cstring"
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

class VertexArrayObject {

    public:
	enum modes {VERTICES, ELEMENTS, TRIANGLES, POINTS};

    private:
	static const char* MVP_SHADER_NAME;
	static const char* M_SHADER_NAME;
	static glm::mat4 v_matrix, p_matrix, vp_matrix;
	GLuint mvp_loc, shader_program, texture_loc, m_loc;

	glm::mat4 m_matrix;
	GLuint vao_loc;
	bool visibility;
	int geometry_size, topology_size;
	modes drawing_mode, primitive;
	int num_vbos;
	int vbos_counter;
	GLuint* vbos_loc;
	bool has_texture;


    public:
	// Basic Routines
	VertexArrayObject() {};
	VertexArrayObject(int);
	~VertexArrayObject();
	void setVisibility(bool);	
	void setModelMatrix(glm::mat4 &); 
	void setDrawingMode(modes);
	void setPrimitive(modes);
	void clear(int);

	// Static Routines
	static void setViewMatrix(glm::mat4 &);
	static void setProjectionMatrix(glm::mat4 &);
	void registerShaderProgram(GLuint shader_program);


	// Specialized Routines
	void setGeometry(std::vector<glm::vec3>);
	void setTopology(std::vector<GLuint>);
	void setUV(vector<glm::vec2>);
	void setColors(vector<glm::vec3>);
	void setTexture(char*);
	void draw();

};

#endif
