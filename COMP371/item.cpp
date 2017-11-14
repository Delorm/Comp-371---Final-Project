#include "item.hpp"
#include "objloader.hpp"

Item::Item() {
    clear();
}

void Item::loadObject(char* path) {
    loadOBJ(path, vertices, normals, uvs);

    vao.setGeometry(vertices);
    vao.setDrawingMode(VertexArrayObject::VERTICES);
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

void Item::setModelMatrix(glm::mat4 model_matrix) {
    this->model_matrix = model_matrix;
    vao.setModelMatrix(model_matrix);
}

void Item::draw() {
    vao.draw();
}

void Item::clear() {
    vao.clear();
    vertices.clear();
    edges.clear();
    normals.clear();
    uvs.clear();
    model_matrix = glm::mat4(1);
}
