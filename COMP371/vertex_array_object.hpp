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
	enum modes {VERTICES, ELEMENTS, TRIANGLES, POINTS, LINES};

    public:
	static const char* MVP_SHADER_NAME;		// Shaders Variable Name
	static const char* M_SHADER_NAME;
	static const char* V_SHADER_NAME;
	static const char* P_SHADER_NAME;
	static const char* LIGHT_SHADER_NAME;
	static const char* EYE_SHADER_NAME;
	static const char* SKYCOLOR;
	static const char* FOG_DENSITY_SHADER_NAME;
	static const char* FOG_GRADIENT_SHADER_NAME;
	static const char* CLIPPING_PLANE_SHADER_NAME;
	static const char* WATER_OFFSET_SHADER_NAME;
	static const char* LIGHT_V_MATRIX_SHADER_NAME;
	static glm::vec3 skyColor;			// Static variables(apply to all Vaos)
	static glm::mat4 v_matrix, p_matrix, vp_matrix, light_v_matrix;
	static glm::vec4 light_direction, eye_location, clipping_plane;
	static float fog_density, fog_gradient, water_offset;
	GLuint mvp_loc, shader_program, m_loc, v_loc, p_loc, light_loc, eye_loc, sky_color, density_loc, gradient_loc, clipping_plane_loc, water_offset_loc, light_v_matrix_loc;
	int num_of_textures;
	GLuint* textures_loc;
	char** textures_sampler_names;
	int texture_index;


	// Member Variables
	glm::mat4 m_matrix;
	GLuint vao_loc;
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
	void recycle(int);

	// Static Routines
	static void setViewMatrix(glm::mat4 &);
	static void setProjectionMatrix(glm::mat4 &);
	static void setLightDirection(glm::vec4 &);
	static void setEyeLocation(glm::vec4 &);
	static void loadSkyColor(glm::vec3 &);
	void registerShaderProgram(GLuint shader_program);
	bool visibility;


	// Specialized Routines
	void setGeometry(std::vector<glm::vec3>);
	void setTopology(std::vector<GLuint>);
	void setUVs(vector<glm::vec2>);
	void setNormals(vector<glm::vec3>);
	void setColors(vector<glm::vec3>);
	void setTexture(char*, char*, int);
	void setTexture(GLuint, char*, int);
	void setNumOfTexture(int);
	void draw();

};

#endif
