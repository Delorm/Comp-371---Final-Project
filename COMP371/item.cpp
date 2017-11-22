#include "item.hpp"
#include "objloader.hpp"

Item::Item(int num_vbos) {
    clear(num_vbos);
}

void Item::loadObject(char* path) {
    loadOBJ(path, vertices, normals, uvs);

    vao.setGeometry(vertices);
    vao.setDrawingMode(VertexArrayObject::VERTICES);
    vao.setUVs(uvs);
}

void Item::setGeometry(std::vector<glm::vec3> vertices) {
    this->vertices = vertices;
    vao.setGeometry(vertices);
}

void Item::setTopology(std::vector<unsigned int> edges) {
    this->edges = edges;
    vao.setTopology(edges);
    vao.setDrawingMode(VertexArrayObject::ELEMENTS);
}

void Item::setUVs(std::vector<glm::vec2> uvs) {
    vao.setUVs(uvs);
}

void Item::setColors(std::vector<glm::vec3> colors) {
    vao.setColors(colors);
}

void Item::setTexture(char* texture_name) {
    vao.setTexture(texture_name, "ourTexture1", GL_LINEAR);
}

void Item::setTexture(char* texture_name, char* sampler_name, int interpolation_mode) {
    vao.setTexture(texture_name, sampler_name, interpolation_mode);
}

void Item::setNumOfTexture(int num) {
    vao.setNumOfTexture(num);
}

void Item::setModelMatrix(glm::mat4 & model_matrix) {
    this->model_matrix = model_matrix;
    vao.setModelMatrix(model_matrix);
}

void Item::setShaderProgram(GLuint shaderProgram) {
    vao.registerShaderProgram(shaderProgram);
}

void Item::draw() {
    vao.draw();
}

void Item::recycle(int num_vbos) {
    vao.recycle(num_vbos);
    vertices.clear();
    edges.clear();
    normals.clear();
    uvs.clear();
    model_matrix = glm::mat4(1);
}

void Item::clear(int num_vbos) {
    recycle(num_vbos);
    vao.clear(num_vbos);
}

std::vector<glm::vec3> & Item::getVertices() {
    return vertices;
}

