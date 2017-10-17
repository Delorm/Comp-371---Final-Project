/*
 * Author: Adnan Utayim
 * October 13, 2017
 * Skeleton Adopted and Modified
 * From learnopengl.com
 */

#ifndef _VERTEX_ARRAY_OBJECT_H
#define _VERTEX_ARRAY_OBJECT_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "string"
#include "vector"

using namespace std;


class VertexArrayObject {

    public:
	enum modes {VERTICES, ELEMENTS, TRIANGLES, POINTS};

    private:
	static const char* MVP_SHADER_NAME;
	static glm::mat4 v_matrix, p_matrix, vp_matrix;
	static GLuint mvp_loc, shader_program;

	glm::mat4 m_matrix;
	GLuint vao_loc;
	bool visibility;
	int geometry_size, topology_size;
	modes drawing_mode, primitive;


    public:
	// Basic Routines
	VertexArrayObject();
	~VertexArrayObject();
	void setVisibility(bool);	
	void setModelMatrix(glm::mat4&);
	void setDrawingMode(modes);
	void setPrimitive(modes);

	// Static Routines
	static void setViewMatrix(glm::mat4&);
	static void setProjectionMatrix(glm::mat4&);
	static void registerShaderProgram(GLuint shader_program);


	// Specialized Routines
	void setGeometry(std::vector<glm::vec3> &);
	void setTopology(std::vector<GLushort> &);
	void draw();

};

#endif
