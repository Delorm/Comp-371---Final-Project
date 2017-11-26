#include "l_system.hpp"
#include "iostream"
#include "stack"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stdlib.h"
#include "time.h"

using namespace std;

LSystem::LSystem(float seg_length, float radius, float alpha) {

    axiom = "A";
    message = axiom;
    
    makeRule("A", "f[-s(0.8)r(0.2)C]s(0.5)r(0.9)f[+^r(0.2)C]fs(2)[+B][-B][<B][>B]");
    makeRule("B", "s(0.7)r(0.6)(f[+B][-B][^B][vfB]");
    makeRule("C", "f");


    // Initialization:
    this->seg_length = seg_length;
    this->radius = radius;
    this-> alpha = alpha;

}

void LSystem::makeRule(string a, string b) {

    Rule r;
    r.a = a;
    r.b = b;
    rules.push_back(r);
}


void LSystem::generate(int times) {

    for (int t = 0; t < times; t++) {
	string newMessage = "";
	for (int i = 0; i < message.length(); i++) {
	    char c = message[i];
	    string s = string(1, c);

	    // Find a rule
	    bool found = false;
	    for (int r = 0; r < rules.size(); r++) {
		if (s == rules[r].a) {
		    newMessage += rules[r].b;
		    found = true;
		    break;
		}
	    }
	    if (!found) {
		newMessage += s;
	    }
	}
	message = newMessage;
    }
}

string LSystem::getMessage() {
    return message;
}


void LSystem::getTree(
	std::vector<glm::vec3> & vertices, 
	std::vector<unsigned int> & indices, 
	std::vector<glm::vec2> & uvs, 
	std::vector<glm::vec3> & normals, 
	std::vector<glm::vec3> & l_vertices, 
	std::vector<unsigned int> & l_indices, 
	std::vector<glm::vec2> & l_uvs, 
	std::vector<glm::vec3> & l_normals) {

    glm::vec3 curr_pos = glm::vec3(0, 0, 0);
    glm::vec3 curr_dir = glm::vec3(0, 1, 0);
    float curr_len = seg_length;
    float curr_tru = radius;
    stack<glm::vec3> pos_stack;
    stack<glm::vec3> dir_stack;
    stack<float> len_stack;
    stack<float> tru_stack;


    for (int i = 0; i < message.length(); i++) {
	char c = message[i];
	
	if (c == ' ') {

	    continue;

	} else if (c == 'f') {

	    float ran = ((((float)rand() / RAND_MAX) * 2 - 1) * random * curr_len) + curr_len;
	    glm::vec3 next_pos = curr_pos +  ran * curr_dir;
	    drawBark(curr_pos, next_pos, curr_tru, vertices, indices, uvs, normals);
	    curr_pos = next_pos;

	} else if (c == '+') {

	    float ran = (((float)rand() / RAND_MAX) * 2 - 1) * random * alpha + alpha;
	    glm::mat4 rot = glm::rotate(glm::mat4(1), +ran, glm::vec3(0, 0, 1));
	    curr_dir = glm::vec3(rot * glm::vec4(curr_dir, 0));

	} else if (c == '-') {

	    float ran = (((float)rand() / RAND_MAX) * 2 - 1) * random * alpha + alpha;
	    glm::mat4 rot = glm::rotate(glm::mat4(1), -ran, glm::vec3(0, 0, 1));
	    curr_dir = glm::vec3(rot * glm::vec4(curr_dir, 0));

	} else if (c == '>') {

	    float ran = (((float)rand() / RAND_MAX) * 2 - 1) * random * alpha + alpha;
	    glm::mat4 rot = glm::rotate(glm::mat4(1), +ran, glm::vec3(1, 0, 0));
	    curr_dir = glm::vec3(rot * glm::vec4(curr_dir, 0));

	} else if (c == '<') { 
	
	    float ran = (((float)rand() / RAND_MAX) * 2 - 1) * random * alpha + alpha;
	    glm::mat4 rot = glm::rotate(glm::mat4(1), -ran, glm::vec3(1, 0, 0));
	    curr_dir = glm::vec3(rot * glm::vec4(curr_dir, 0));
	
	} else if (c == '^') {

	    float ran = (((float)rand() / RAND_MAX) * 2 - 1) * random * alpha + alpha;
	    glm::mat4 rot = glm::rotate(glm::mat4(1), +ran, glm::vec3(0, 1, 0));
	    curr_dir = glm::vec3(rot * glm::vec4(curr_dir, 0));
	    
	} else if (c == 'v') {
	    
	    float ran = (((float)rand() / RAND_MAX) * 2 - 1) * random * alpha + alpha;
	    glm::mat4 rot = glm::rotate(glm::mat4(1), -ran, glm::vec3(0, 1, 0));
	    curr_dir = glm::vec3(rot * glm::vec4(curr_dir, 0));
	    
	} else if (c == '[') {

	    pos_stack.push(curr_pos);
	    dir_stack.push(curr_dir);
	    len_stack.push(curr_len);
	    tru_stack.push(curr_tru);

	} else if (c == ']') {

	    curr_pos = pos_stack.top();
	    curr_dir = dir_stack.top();
	    curr_len = len_stack.top();
	    curr_tru = tru_stack.top();
	    pos_stack.pop();
	    dir_stack.pop();
	    len_stack.pop();
	    tru_stack.pop();

	} else if (c == 's') {

	    float mult = getMultiplier(i);
	    curr_len *= mult;

	} else if (c == 'r') {

	    float mult = getMultiplier(i);
	    curr_tru *= mult;

	} else if (c == 'A' || c == 'B' || c == 'C') {
	    drawLeaves(curr_pos, curr_dir, curr_tru, l_vertices, l_indices, l_uvs, l_normals);
	} 
    }
}

float LSystem::getMultiplier(int & i) {
    
    string s = "";
    char c = '-';
    while (c != ')') {
	i++;
	c = message[i];
	if (c != ')' && c != '(') {
	    s += c;
	}
    }
    return atof(s.c_str());
}

void LSystem::drawBark(
	glm::vec3 & start, 
	glm::vec3 & end, 
	float radius, 
	std::vector<glm::vec3> & vertices, 
	std::vector<unsigned int> & indices, 
	std::vector<glm::vec2> & uvs, 
	std::vector<glm::vec3> & normals) {

    int offset = vertices.size();
    glm::vec3 dir = glm::normalize(end - start);
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec3 pos = 0.5f * (end + start); 
    float length = glm::distance(start, end);
    float half_length = length / 2;
    float a_step = 2.0f * PI / points;

    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = translate(model_matrix, pos); 

    if (dot(dir, up) < 0.99) {
	float angle = acos(glm::dot(up, dir));
	glm::vec3 rot_axis = glm::normalize(glm::cross(up, dir));
	model_matrix = rotate(model_matrix, angle, rot_axis);
    }

    
    bool even = true;
    glm::vec3 last_point = glm::vec3(0, 0, 0);
    int iteration = 0;
    for (float a = 2.0f * PI; a >= -0.1f; a -= a_step) {

	// Vertices
	glm::vec4 p = glm::vec4(cos(a) * radius, -half_length, sin(a) * radius, 1);
	glm::vec3 p3 = glm::vec3(model_matrix * p);
	vertices.push_back(p3);
	p.y = +half_length;
	p3 = glm::vec3(model_matrix * p);
	vertices.push_back(p3);
	glm::vec3 u1 = p3;
	glm::vec3 u2 = last_point - p3;

	// UVs
	even = !even;
	uvs.push_back(glm::vec2(iteration * 1.0f / points, 0));
	uvs.push_back(glm::vec2(iteration * 1.0f / points, 1));

	// Normals
	glm::vec3 normal = glm::normalize(glm::cross(u1, u2));
	normals.push_back(normal);
	normals.push_back(normal);

	last_point = p3;
	iteration++;
    }	

    for (int i = 0; i < 2 * points; i += 2)  {

	// Indices
	indices.push_back(offset + i + 0);
	indices.push_back(offset + i + 1); 
	indices.push_back(offset + i + 2); 

	indices.push_back(offset + i + 1); 
	indices.push_back(offset + i + 3); 
	indices.push_back(offset + i + 2); 
	
    }
    
}

void LSystem::drawLeaves(
	glm::vec3 & start, 
	glm::vec3 & dir, 
	float radius, 
	std::vector<glm::vec3> & vertices, 
	std::vector<unsigned int> & indices, 
	std::vector<glm::vec2> & uvs, 
	std::vector<glm::vec3> & normals) {

	int offset = vertices.size();
	float random = (float)rand() / RAND_MAX;
	float width = random * l_width;
	float height = random * l_height;

	// Vertices
	glm::vec3 end = start + height * dir;
	glm::vec3 side = width * glm::normalize(glm::cross(end - start, glm::vec3(0, 1, 0))); 

	vertices.push_back(start);
	vertices.push_back(0.5f * (start + end) + side);
	vertices.push_back(end);

	vertices.push_back(start);
	vertices.push_back(0.5f * (start + end) - side);
	vertices.push_back(end);


	// Indices
	for (int i = 0; i < 6; i++) {
	    indices.push_back(offset + i);
	}
	
	// UVS
	uvs.push_back(glm::vec2(0, 0.5));
	uvs.push_back(glm::vec2(0.5, 0));
	uvs.push_back(glm::vec2(1, 0.5));
	uvs.push_back(glm::vec2(0, 0.5));
	uvs.push_back(glm::vec2(0.5, 1));
	uvs.push_back(glm::vec2(1, 0.5));

	// normals
	glm::vec3 normal = glm::normalize(glm::cross(side, dir));
	for (int i = 0; i < 4; i++) {
	    normals.push_back(normal);
	}
}


