#include "item.hpp"
#include "objloader.hpp"

Item::Item(int num_vbos) {
    clear(num_vbos);
}

void Item::setLightDirection(glm::vec4 & direction) {
    VertexArrayObject::setLightDirection(direction);
}

void Item::setEyeLocation(glm::vec4 & loc) {
    VertexArrayObject::setEyeLocation(loc);
}

void Item::loadObject(char* path) {		    // Also creates the Vao
    char full_path [30] = "";
    strcat(full_path, "res/obj/");
    strcat(full_path, path);
    strcat(full_path, ".obj");
    loadOBJ(full_path, vertices, normals, uvs);

    vao.setGeometry(vertices);
    vao.setDrawingMode(VertexArrayObject::VERTICES);
    vao.setUVs(uvs);
    for (int i = 0; i < vertices.size(); i++) {
	edges.push_back(i);
    }
    vao.setNormals(normals);
}

void Item::setGeometry(std::vector<glm::vec3> vertices) {   // Vertices
    this->vertices = vertices;
    vao.setGeometry(vertices);
}

void Item::setTopology(std::vector<unsigned int> edges) {   // Indices
    this->edges = edges;
    vao.setTopology(edges);
    vao.setDrawingMode(VertexArrayObject::ELEMENTS);
}

void Item::setUVs(std::vector<glm::vec2> uvs) {
    vao.setUVs(uvs);
}

void Item::setNormals(std::vector<glm::vec3> normals) {
    this-> normals = normals;
    vao.setNormals(normals);
}

void Item::setColors(std::vector<glm::vec3> colors) {
    vao.setColors(colors);
}

void Item::setTexture(char* texture_name) {		    
    setTexture(texture_name, "ourTexture1", GL_LINEAR_MIPMAP_LINEAR);
}   

void Item::setTexture(char* texture_name, char* sampler_name, int interpolation_mode) {
    char full_path [30] = "";
    strcat(full_path, "res/texture/");
    strcat(full_path, texture_name);
    strcat(full_path, ".png");
    vao.setTexture(full_path, sampler_name, interpolation_mode);
}

void Item::setNumOfTexture(int num) {
    vao.setNumOfTexture(num);
}

void Item::setModelMatrix(glm::mat4 & model_matrix) {
    this->model_matrix = model_matrix;
    vao.setModelMatrix(model_matrix);
}

void Item::setShaderProgram(GLuint shader_program) {
    vao.registerShaderProgram(shader_program);
}

void Item::draw() {
    vao.draw();
}

void Item::recycle(int num_vbos) {		// Clear item but keep texture
    vao.recycle(num_vbos);
    vertices.clear();
    edges.clear();
    normals.clear();
    uvs.clear();
    model_matrix = glm::mat4(1);
    collidable = false;
}

void Item::clear(int num_vbos) {		// Recycle and remove texture
    recycle(num_vbos);
    vao.clear(num_vbos);
    type = -1;
}

std::vector<glm::vec3> & Item::getVertices() {
    return vertices;
}

void Item::setCollidable(bool collide) {	// Converts Normas and vertices to world space

    if (collide) {
	std::vector<glm::vec3> temp;

	for (int i = 0; i < vertices.size(); i++) {
	    glm::vec4 point = glm::vec4(vertices[i], 1);
	    temp.push_back(glm::vec3(model_matrix * point));
	}
	vertices = temp;
	temp.clear();

	normals.clear();
	d_coeff.clear();			// Pre calc plane equation(Optimization)
	for (int i = 0; i < edges.size(); i += 3) {
	    glm::vec3 p0 = vertices[edges[i+0]];
	    glm::vec3 p1 = vertices[edges[i+1]];
	    glm::vec3 p2 = vertices[edges[i+2]];

	    glm::vec3 u0 = p1 - p0;
	    glm::vec3 u1 = p2 - p0;
	    glm::vec3 nor = glm::normalize(glm::cross(u0, u1));;
	    float d = -glm::dot(nor, p0);
	    for (int j = 0; j < 3; j++) {
		normals.push_back(nor);
		d_coeff.push_back(d);
	    }

	}
    }

    collidable = collide; 
}

bool Item::isCollidable() {
    return collidable;
}

