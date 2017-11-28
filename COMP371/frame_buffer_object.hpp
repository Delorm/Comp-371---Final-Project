/*
 * October 13, 2017
 * Skeleton Adopted and Modified
 * From learnopengl.com
 */

#ifndef _FRAGMENT_BUFFER_OBJECT_H
#define _FRAGMENT_BUFFER_OBJECT_H

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

class FrameBufferObject {


    public:
	GLuint reflection_id, reflection_texture_id, reflection_depth_buffer_id;
	GLuint refraction_id, refraction_texture_id, refraction_depth_buffer_id;
	GLuint shadowmap_id, shadow_map_texture_id, debugging_id;
	const int width = 1024;
	const int height = 512; 
	const int shadow_width = 512;
	const int shadow_height = 256;
	


    public:
	void bind(GLuint, int, int);
	void unbind(int, int);
	GLuint createFrameBuffer();
	GLuint createTextureAttachment(int, int);
	GLuint createDepthTextureAttachment(int, int);
	GLuint createDepthBufferAttachment(int, int);
	void check();

	void createReflectionFrameBuffer(int, int);
	void createRefractionFrameBuffer(int, int);
	void createShadowMapFrameBuffer(int, int);

};

#endif
