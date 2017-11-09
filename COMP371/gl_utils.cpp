/*
 * Author: Adnan Utayim
 * October 12, 2017
 * Code Adopted and Modified
 * From learnopengl.com
 */

#include "fstream"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "gl_utils.hpp"
#include "vertex_array_object.hpp"
#include "stdio.h"
#include "iostream"
#include "stdlib.h"
#include "stdio.h"
#include "cstring"
#include "string"
#include "CImg.hpp"
#include "vector"
#include "list"
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


const float MESH_MAX_HEIGHT = 50.0f;

GLuint GlUtilities::loadShaders() {

    // Read, Compile, and Attach Shader Programs
    string vertex_shader_path = "vertex.shader";
    string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_shader_path, ios::in);

    if (VertexShaderStream.is_open()) {
	string Line = "";
	while (getline(VertexShaderStream, Line))
	    VertexShaderCode += "\n" + Line;
	VertexShaderStream.close();
    }
    else {
	printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
	getchar();
	exit(-1);
    }

    // Read the Fragment Shader code from the file
    string fragment_shader_path = "fragment.shader";
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

    if (FragmentShaderStream.is_open()) {
	std::string Line = "";
	while (getline(FragmentShaderStream, Line))
	    FragmentShaderCode += "\n" + Line;
	FragmentShaderStream.close();
    }
    else {
	printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
	getchar();
	exit(-1);
    }   

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
	glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader); //free up memory
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    return shaderProgram;

}

GLFWwindow* GlUtilities::setupGlWindow(GLuint global_width, GLuint global_height) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GL_DEPTH_BITS, 24);

    GLFWwindow* window = glfwCreateWindow(global_width, global_height, "Load one cube", nullptr, nullptr);
    if (window == nullptr)
    {   
	std::cout << "Failed to create GLFW window" << std::endl;
	glfwTerminate();
	return NULL;
    }   
    glfwMakeContextCurrent(window);

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {   
	std::cout << "Failed to initialize GLEW" << std::endl;
	return NULL;
    }   

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    return window;
}

void GlUtilities::createTerrain(vector<glm::vec3> & vertices, int & width, int & height) {

    cimg_library::CImg<unsigned char> image("resources/depth.bmp");
    width = image.width();
    height = image.height();
    int max_height = 0;
    int min_height = (int)INFINITY;

    for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	    int current_height = image(i, j); 
	    if (current_height > max_height) max_height = current_height;
	    if (current_height < min_height) min_height = current_height;
	}
    }   
    float height_scale = MESH_MAX_HEIGHT / (float)(max_height - min_height);


    for (int z = 0; z < height; z++) {
	for (int x = 0; x < width; x++) {
	    float y = (image(x, z) - min_height) * height_scale; 
	    vertices.push_back(glm::vec3(x, y, z));
	}
    }   
}

std::vector<GLuint> GlUtilities::findIndices(int width, int height) {

    std::vector<GLuint> indices;

    // First Pass
    for (int i = 0; i < width * (height - 1); i++) {
	int x = i % width;
	int y = i / width;
	if (x + 1 == width) continue;
	indices.push_back(i);
	indices.push_back((y + 1) * width + x);
	indices.push_back(i+1);
    }

    // Second Pass
    for (int i = width; i < width * height; i++) {
	int x = i % width;
	int y = i / width;
	if (x + 1 == width) continue;
	indices.push_back(i);
	indices.push_back(i+1);
	indices.push_back((y - 1) * width + (x + 1));
    }

    return indices;
}

void GlUtilities::interpolate(vector<glm::vec3> & vertices_vector, int skip_size, float step_size, int & width, int & height) {

    // Step 1: Skip points
    std::list<glm::vec3> vertices(vertices_vector.begin(), vertices_vector.end());
    int list_counter = 0;
    for (list<glm::vec3>::iterator it = vertices.begin(); it != vertices.end(); it++) {
	int x = list_counter % width;
	if (x % skip_size != 0) {
	    vertices.erase(it);
	    it--;
	}
	list_counter++;

    }
    width = vertices.size() / height;

    list_counter = 0;
    GlUtilities::transposeList(vertices, width, height);
    for (list<glm::vec3>::iterator it = vertices.begin(); it != vertices.end(); it++) {
	int x = list_counter % width;
	if (x % skip_size != 0) {
	    vertices.erase(it);
	    it--;
	}
	list_counter++;

    }
    width = vertices.size() / height;
    GlUtilities::transposeList(vertices, width, height);

    // Step 2: Interpolate on X
    GlUtilities::catmullRom(vertices, width, height, 0.5f, step_size);
    width = vertices.size() / height;

    // Step 3: Interpolate on Z
    GlUtilities::transposeList(vertices, width, height);
    GlUtilities::catmullRom(vertices, width, height, 0.5f, step_size);
    width = vertices.size() / height;
    GlUtilities::transposeList(vertices, width, height);

    vertices_vector = std::vector<glm::vec3> (vertices.begin(), vertices.end());
}

void GlUtilities::transposeList(std::list<glm::vec3>& vertices, int & w, int & h) {

    // Allocate Matrix
    glm::vec3** matrix = new glm::vec3* [w];
    for (int x = 0; x < w; x++) {
	matrix[x] = new glm::vec3 [h];
    }

    // Fill Data Transposedally
    std::list<glm::vec3>::iterator it = vertices.begin();
    for (int y = 0; y < h; y++) {
	for (int x = 0; x < w; x++) {
	    matrix[x][y] = *it;
	    it++;
	}
    }

    // Return Data to list
    vertices.clear();
    for (int x = 0; x < w; x++) {
	for (int y = 0; y < h; y++) {
	    vertices.push_back(matrix[x][y]);
	}
    }

    // De-Allocation Matrix
    for (int x = 0; x < w; x++) {
	delete [] matrix[x];
	matrix[x] = 0;
    }
    delete [] matrix;
    matrix = 0;

    // Swap Height & Width
    int temp = h;
    h = w;
    w = temp;
}


void GlUtilities::catmullRom(std::list<glm::vec3> & vertices, int w, int h, float s, float step_size) {

    // Basis Matrix 
    const int NUM_OF_PTS = 4;
    glm::mat4 basis = glm::transpose(glm::mat4 {
	    -s,         2.0f-s,     s-2.0f,         s,
	    2.0f*s,     s-3.0f,     3.0f-2.0f*s,    -s,
	    -s,         0.0f,       s,              0.0f,
	    0.0f,       1.0f,       0.0f,           0.0f
	    });

    // Position Iterators
    list<glm::vec3>::iterator p [NUM_OF_PTS];
    for (int i = 0; i < NUM_OF_PTS; i++) {
	p[i] = vertices.begin();
	std::advance(p[i], i - 1);
    }

    int list_counter = 0;
    while (list_counter < h * w) {

	glm::vec3 point [NUM_OF_PTS];
	for (int i = 0; i < NUM_OF_PTS; i++) {
	    point[i] = *p[i];
	}

	// Edge Cases:
	if (list_counter % w == 0) {                // Add a point to the left
	    point[0]   = point[1] + point[1] - point[2];
	    point[0].y = point[1].y;
	} else if ((list_counter + 2) % w == 0) {   // Add a point to the right
	    point[NUM_OF_PTS - 1]   = point[2] + point[2] - point[1];
	    point[NUM_OF_PTS - 1].y = point[2].y;
	}

	// Construct control Matrix
	glm::mat4 control = glm::transpose(glm::mat4 {
		point[0].x, point[0].y, point[0].z, 0.0f,
		point[1].x, point[1].y, point[1].z, 0.0f,
		point[2].x, point[2].y, point[2].z, 0.0f,
		point[3].x, point[3].y, point[3].z, 0.0f
		});
	glm::mat4 basis_control_matrix = basis * control;


	// Paremetarize u:
	for (float u = step_size; u < 1.0f; u += step_size) {
	    glm::vec4 u_vector = glm::vec4(u*u*u, u*u, u, 1);
	    glm::vec4 new_point = u_vector * basis_control_matrix;
	    vertices.insert(p[2], glm::vec3(new_point));
	}

	// Increment
	for (int i = 0; i < NUM_OF_PTS - 1; i++) {
	    p[i] = p[i+1];
	}
	p[NUM_OF_PTS - 1]++;
	list_counter++;

	// If at End of Line: Increment again
	if ((list_counter + 1) % w == 0) {
	    for (int i = 0; i < NUM_OF_PTS - 1; i++) {
		p[i] = p[i+1];
	    }
	    p[NUM_OF_PTS - 1]++;
	    list_counter++;
	}
    }
}

