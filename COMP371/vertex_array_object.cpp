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
    #include "SOIL.h"
#elif _WIN32
    // windows code goes here
    #include "..\glew\glew.h"	// include GL Extension Wrangler
    #include "..\glfw\glfw3.h"	// include GLFW helper library
    #include "..\SOIL2\SOIL2.h"
#endif

using namespace std;

// Static Initilization
const char* VertexArrayObject::MVP_SHADER_NAME = "mvp_matrix";
const char* VertexArrayObject::M_SHADER_NAME   = "m_matrix";
const char* VertexArrayObject::V_SHADER_NAME   = "v_matrix";
const char* VertexArrayObject::SKYCOLOR   = "skyColor";
const char* VertexArrayObject::LIGHT_SHADER_NAME = "light_direction";
const char* VertexArrayObject::EYE_SHADER_NAME = "eye_location";

glm::vec3 VertexArrayObject::skyColor = glm::vec3(1.0f);
glm::mat4 VertexArrayObject::v_matrix = glm::mat4(1.0f);
glm::mat4 VertexArrayObject::p_matrix = glm::mat4(1.0f);
glm::mat4 VertexArrayObject::vp_matrix = glm::mat4(1.0f);
glm::vec4 VertexArrayObject::light_direction = glm::vec4(1);
glm::vec4 VertexArrayObject::eye_location = glm::vec4(1);

// Basic Rountines
VertexArrayObject::VertexArrayObject(int num_vbos) {
    this->clear(num_vbos);
}


VertexArrayObject::~VertexArrayObject() {

}

void VertexArrayObject::clear(int num_vbos) {

    recycle(num_vbos);

    // Clear Texture
    glGenBuffers(num_vbos, vbos_loc);
    has_texture = false;
    textures_loc = 0;
    texture_index = 0;
    num_of_textures = 0;
}

void VertexArrayObject::recycle(int num_vbos) {
    m_matrix = glm::mat4(1.0f);
    visibility = true;
    drawing_mode = ELEMENTS;
    primitive = TRIANGLES;
    this->num_vbos = num_vbos;
    glGenVertexArrays(1, &vao_loc);
    vbos_loc = new GLuint [num_vbos];
    vbos_counter = 0;
    glGenBuffers(num_vbos, vbos_loc);
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

void VertexArrayObject::setLightDirection(glm::vec4 & direction) {
    light_direction = direction;
}

void VertexArrayObject::setEyeLocation(glm::vec4 & location) {
    eye_location = location;
}

void VertexArrayObject::registerShaderProgram(GLuint new_shader_program) {
    shader_program = new_shader_program;
    mvp_loc = glGetUniformLocation(new_shader_program, MVP_SHADER_NAME);
    m_loc = glGetUniformLocation(new_shader_program, M_SHADER_NAME);
    v_loc = glGetUniformLocation(new_shader_program, V_SHADER_NAME);
    light_loc = glGetUniformLocation(new_shader_program, LIGHT_SHADER_NAME);
    eye_loc = glGetUniformLocation(new_shader_program, EYE_SHADER_NAME);
    sky_color = glGetUniformLocation(new_shader_program, SKYCOLOR);

}


// Specialized Routines
void VertexArrayObject::setGeometry(std::vector<glm::vec3> vertices) {

    geometry_size = vertices.size();
    glBindVertexArray(vao_loc);
    glBindBuffer(GL_ARRAY_BUFFER, vbos_loc[vbos_counter]);
    glBufferData(
	    GL_ARRAY_BUFFER, 
	    vertices.size() * sizeof(glm::vec3), 
	    &vertices.front(), 
	    GL_STATIC_DRAW
	    );
    glVertexAttribPointer(vbos_counter, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(vbos_counter);
    vbos_counter++;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void VertexArrayObject::setTopology(std::vector<GLuint> edges) {

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

void VertexArrayObject::setUVs(std::vector<glm::vec2> uvs) {

    geometry_size = uvs.size();
    glBindVertexArray(vao_loc);
    glBindBuffer(GL_ARRAY_BUFFER, vbos_loc[vbos_counter]);
    glBufferData(
	    GL_ARRAY_BUFFER, 
	    uvs.size() * sizeof(glm::vec2), 
	    &uvs.front(), 
	    GL_STATIC_DRAW
	    );
    glVertexAttribPointer(vbos_counter, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(vbos_counter);
    vbos_counter++;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}

void VertexArrayObject::setNormals(std::vector<glm::vec3> normals) {

    geometry_size = normals.size();
    glBindVertexArray(vao_loc);
    glBindBuffer(GL_ARRAY_BUFFER, vbos_loc[vbos_counter]);
    glBufferData(
	    GL_ARRAY_BUFFER, 
	    normals.size() * sizeof(glm::vec3), 
	    &normals.front(), 
	    GL_STATIC_DRAW
	    );
    glVertexAttribPointer(vbos_counter, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(vbos_counter);
    vbos_counter++;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void VertexArrayObject::setColors(vector<glm::vec3> colors) {

    geometry_size = colors.size();
    glBindVertexArray(vao_loc);
    glBindBuffer(GL_ARRAY_BUFFER, vbos_loc[vbos_counter]);
    glBufferData(
	    GL_ARRAY_BUFFER, 
	    colors.size() * sizeof(glm::vec3), 
	    &colors.front(), 
	    GL_STATIC_DRAW
	    );
    glVertexAttribPointer(vbos_counter, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(vbos_counter);
    vbos_counter++;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void VertexArrayObject::setTexture(char* imageName, char* sampler_name, int interpolation_mode) {

    has_texture = true;
    if (num_of_textures == 0) {
	setNumOfTexture(1);
    }
    
    glActiveTexture(GL_TEXTURE0 + texture_index);
    glEnable(GL_TEXTURE_2D);
    GLuint uni_loc = glGetUniformLocation(shader_program, sampler_name);
    glUniform1i(uni_loc, texture_index); 

    glBindTexture(GL_TEXTURE_2D, textures_loc[texture_index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation_mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation_mode);

    int width, height;
    unsigned char *image = SOIL_load_image(imageName, &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    textures_sampler_names[texture_index] = sampler_name;
    texture_index++;

}

void VertexArrayObject::setNumOfTexture(int num_of_textures) {
    this->num_of_textures = num_of_textures;
    textures_loc = new GLuint [num_of_textures];
    textures_sampler_names = new char* [num_of_textures];
    glGenTextures(num_of_textures, textures_loc);
}

void VertexArrayObject::draw() {
    if (!visibility) return;

    // Use Shader
    glUseProgram(shader_program);

    glm::mat4 mvp_matrix = vp_matrix * m_matrix;
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp_matrix));
    glUniformMatrix4fv(m_loc, 1, GL_FALSE, glm::value_ptr(m_matrix));
    glUniformMatrix4fv(v_loc, 1, GL_FALSE, glm::value_ptr(v_matrix));
    glUniform3fv(sky_color, 1, glm::value_ptr(skyColor));
    glUniform4fv(light_loc, 1, glm::value_ptr(light_direction));
    glUniform4fv(eye_loc, 1, glm::value_ptr(eye_location));

    // Bind Texture
    for (int i = 0; i < num_of_textures; i++) {
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, textures_loc[i]);
    }
    

    int renderring_mode = (primitive == POINTS) ? GL_POINTS : GL_TRIANGLES;
    if (primitive == LINES) renderring_mode = GL_LINE_STRIP;
    glBindVertexArray(vao_loc);
    if (drawing_mode == VERTICES) {
	glDrawArrays(renderring_mode, 0, geometry_size);
    } else {
	glDrawElements(renderring_mode, topology_size, GL_UNSIGNED_INT, nullptr);
    }

    for (int i = 0; i < num_of_textures; i++) {
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, 0);
    }
    glBindVertexArray(0);
}

void VertexArrayObject::loadSkyColor(GLuint loc,float r, float g,float b) {
        //loadVector(location_skyColor, Vector3f(r,g,b));
        glUniform3f(loc, r, g, b);
	    
}

