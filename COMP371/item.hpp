#ifndef _ITEM_H
#define _ITEM_H

#include "stdio.h"
#include "stdlib.h"
#include "iostream"
#include "vector"
#include "vertex_array_object.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Item {

    private:
	// Gemoetry & Topology
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> edges;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	glm::mat4 model_matrix;

	// Colors
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	// VAO
	VertexArrayObject vao;

    public:
	Item();
	void loadObject(char*);
	void setGeometry(std::vector<glm::vec3>);
	void setTopology(std::vector<unsigned int>);
	void setModelMatrix(glm::mat4);
	void draw();
	void clear();

};

#endif


