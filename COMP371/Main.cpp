/*
 * COMP 371: Final Project
 * October 12, 2017
 * Skeleton Adopted and Modified
 * From learnopengl.com
 */

#include "stdio.h"
#include "iostream"
#include "string"
#include "cstring"
#include "fstream"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "vertex_array_object.hpp"
#include "gl_utils.hpp"
#include "CImg.hpp"
#include "terrian.hpp"
#include "objloader.hpp"
#include "item.hpp"
#include "time.h"
#include "set"

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

// Configurable Constant Variables
const float MOUSE_SENSITIVITY = 0.1f;
const float CHAR_HEIGHT = 1.0f;
const float WALK_SPEED = 5.0f;
const float FLY_SPEED = 50.0f;

// Terrian
const int T_WIDTH = 128;
const int T_HEIGHT = 128;
const int T_MAX = 10.0f;	// Highest & Lowest point in terrian
const int T_SHIFT = 2;		// Increases land to water ratio

// Rocks
const int R_NUMBER = 250;
const int R_MAX_RADIUS = 3;
const int R_POINTS = 20;

// Constant Variables
const float PI = 3.14159265359f;
const float BACKGROUND_COLOR = 0.4f; 
const float PROJ_NEAR_PLANE = 0.1f;
const float PROJ_FAR_PLANE = 2000.0f;
const GLuint INITIAL_WIDTH = 1280;
const GLuint INITIAL_HEIGHT = 720;
const glm::mat4 IDENTITY = glm::mat4(1.0f);
const glm::vec3 ORIGIN = glm::vec3(0.0f);

// Global Variables
GLFWwindow* window;
bool close_window = false;
std::vector<Item> items;
glm::vec3 center(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 eye(0.0f, 0.0f, 5.0f);
bool free_look = false;
bool wireframe = false; 
Terrian terrian;
float mov_speed = WALK_SPEED;
float teapot_ang = 0.0f;
set<int> key_set;
int skybox_index;
float skybox_theta = 0.0f;
timeval last_time;		// Timekeeping variables
timeval current_time;
float delta_time = 0.0f;
float one_sec_counter = 0.0f;
int frames_counter = 0;
float light_angle = 45.0f;
bool light_mov = true;


//std::vector<glm::vec3> terrian;
int terrian_width;
int terrian_height;

// Prototypes definition
void initGl(void);
void drawGl(void);
void keyCallback(GLFWwindow*, int, int, int, int);
void registerCallbacks(GLFWwindow*);
void windowSizeCallback(GLFWwindow*, int, int); 
float mapHeight(float, float); 
glm::mat4 setCameraPosition(void);
std::vector<GLuint> findIndices(int width, int height); 
bool validMove(glm::vec3);
void move(glm::vec3);
void processInput(void);
void getTimeNow(timeval &);
void calcDeltaTime(void);


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {

    if (action == GLFW_PRESS) {
	key_set.insert(key);	
    } else if (action == GLFW_RELEASE) {
	key_set.erase(key);
    }
}

void processInput() {

    for (set<int>::iterator it = key_set.begin(); it != key_set.end(); it++) {

	int key = *it;
	switch (key) {
	    case GLFW_KEY_ESCAPE:
		close_window = true;
		break;

	    case GLFW_KEY_W: {
		glm::vec3 direction = glm::normalize(center - eye);
		move(direction);
		break;
	    }

	    case GLFW_KEY_S: {
		glm::vec3 direction = glm::normalize(center - eye);
		move(-direction);
		break;
	    }

	    case GLFW_KEY_D: {
		glm::vec3 forward = center - eye;
		glm::vec3 side = glm::normalize(glm::cross(forward, up)); 
		move(side);
		break;
	    }

	    case GLFW_KEY_A: {
		glm::vec3 forward = center - eye;
		glm::vec3 side = glm::normalize(glm::cross(forward, up)); 
		move(-side);
		break;
	    }

	    case GLFW_KEY_V: {
		key_set.erase(key); free_look = !free_look;
		mov_speed = (free_look == false) ? WALK_SPEED : FLY_SPEED;
		break;
	    }

	    case GLFW_KEY_T: {
		key_set.erase(key);
		wireframe = !wireframe;
		if (wireframe) {
		    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
		    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		break;
	    }

	    case GLFW_KEY_P: {
		key_set.erase(key);
		light_mov = !light_mov;
		break;
	    }

	    case GLFW_KEY_1: {
		light_mov = false;
		light_angle = 350.0f;
		break;
	    }

	    case GLFW_KEY_2: {
		light_mov = false;
		light_angle = 75.0f;
		break;
	    }
			     
	    case GLFW_KEY_3: {
		light_mov = false;
		light_angle = 120.0f;
		break;
	    }

	    case GLFW_KEY_4: {
		light_mov = false;
		light_angle = 270.0f;
		break;
	    }

	}
    }
}


void initGl() {

    // Set Projection Matrix 
    glm::mat4 projection_matrix = glm::perspective(45.0f, (GLfloat)INITIAL_WIDTH / (GLfloat)INITIAL_HEIGHT, PROJ_NEAR_PLANE, PROJ_FAR_PLANE);
    VertexArrayObject::setProjectionMatrix(projection_matrix);
    glm::mat4 model_matrix = IDENTITY;

    // Terrain
    Item item(3);
    terrian = Terrian(T_WIDTH, T_HEIGHT, T_MAX, T_SHIFT);
    item.setGeometry(terrian.generateMap());
    item.setTopology(terrian.findIndices());
    item.setUVs(terrian.generateUVs());
    item.setNormals(terrian.generateNormals());

    // Texture
    item.setNumOfTexture(7);
    item.setShaderProgram(GlUtilities::loadShaders("resources/terrain_vertex.shader", "resources/terrain_fragment.shader"));
    item.setTexture("resources/grass.png", "grass", GL_LINEAR);
    item.setTexture("resources/terrain_background.png", "background", GL_LINEAR);
    item.setTexture("resources/terrain_r.png", "r_texture", GL_LINEAR);
    item.setTexture("resources/terrain_g.png", "g_texture", GL_LINEAR);
    item.setTexture("resources/terrain_b.png", "b_texture", GL_LINEAR);
    item.setTexture("resources/blend_map.png", "blend_map", GL_LINEAR);
    item.setTexture("resources/normal_map.png", "normal_map", GL_LINEAR);

    model_matrix = glm::translate(IDENTITY, glm::vec3( (float)-T_WIDTH / 2.0f, 0.0f, (float)-T_HEIGHT / 2.0f));
    item.setModelMatrix(model_matrix);
    items.push_back(item);
    
    

    // Cube #1
    item.clear(2);
    item.loadObject("resources/cube.obj");
    model_matrix = glm::translate(IDENTITY, glm::vec3(0, 10, 0));
    item.setModelMatrix(model_matrix);
    item.setShaderProgram(GlUtilities::loadShaders("resources/tex_vertex.shader", "resources/tex_fragment.shader"));
    item.setTexture("resources/grass.png");
    items.push_back(item);

    // Cube #2
    item.clear(2);
    item.loadObject("resources/cube.obj");
    model_matrix = glm::translate(IDENTITY, glm::vec3(10, 10, 0));
    item.setModelMatrix(model_matrix);
    item.setShaderProgram(GlUtilities::loadShaders("resources/tex_vertex.shader", "resources/tex_fragment.shader"));
    item.setTexture("resources/image2.png");
    items.push_back(item);

    // Rock
    item.clear(2);
    item.loadObject("resources/capsule.obj");
    model_matrix = glm::translate(IDENTITY, glm::vec3(6, 8, 0));
    item.setModelMatrix(model_matrix);
    item.setShaderProgram(GlUtilities::loadShaders("resources/rock_vertex.shader", "resources/rock_fragment.shader"));
    item.setNumOfTexture(1);
    item.setTexture("resources/rocky.jpg");


    // Trees
    int num_of_trees = 10;

    item.clear(2);
    item.loadObject("resources/tree.obj");
    item.setShaderProgram(GlUtilities::loadShaders("resources/tex_vertex.shader", "resources/tex_fragment.shader"));
    item.setTexture("resources/trunc.jpg");
    for (int i = 0; i < num_of_trees; i++) {

	int x_loc = (rand() % T_WIDTH) - T_WIDTH / 2.0f;
	int z_loc = (rand() % T_HEIGHT) - T_HEIGHT / 2.0f;
	float y_loc = mapHeight(x_loc, z_loc);
	if (y_loc < 0) continue;
	float rad = rand() / RAND_MAX * 2 * PI;
	model_matrix = glm::rotate(IDENTITY, rad, up);
	model_matrix = glm::translate(model_matrix, glm::vec3(x_loc, y_loc, z_loc));
	item.setModelMatrix(model_matrix);
	items.push_back(item);
    }


    // Fern
    item.clear(2);
    item.loadObject("resources/fern.obj");
    model_matrix = glm::translate(IDENTITY, glm::vec3(32, 4, 0));
    item.setModelMatrix(model_matrix);
    item.setShaderProgram(GlUtilities::loadShaders("resources/tex_vertex.shader", "resources/tex_fragment.shader"));
    item.setTexture("resources/fern.png");
    items.push_back(item);

    // Tree
    item.clear(2);
    item.loadObject("resources/tree.obj");
    model_matrix = glm::translate(IDENTITY, glm::vec3(16, 4, 0));
    item.setModelMatrix(model_matrix);
    item.setShaderProgram(GlUtilities::loadShaders("resources/tex_vertex.shader", "resources/tex_fragment.shader"));
    item.setTexture("resources/tree.png");
    items.push_back(item);

    // Grass
    glm::mat4 trans;
    trans = glm::rotate(trans, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    item.clear(2);
    item.loadObject("resources/grassModel.obj");
    model_matrix = glm::translate(IDENTITY, glm::vec3(2, 2, 0));
    item.setModelMatrix(model_matrix);
    item.setShaderProgram(GlUtilities::loadShaders("resources/tex_vertex.shader", "resources/tex_fragment.shader"));
    item.setTexture("resources/grassTexture.png");
    items.push_back(item);
    
    
    // Water plane
    item.clear(1);
    item.loadObject("resources/water.obj");
    model_matrix = glm::scale(IDENTITY, glm::vec3(T_WIDTH, 0, T_HEIGHT));
    item.setModelMatrix(model_matrix);
    item.setShaderProgram(GlUtilities::loadShaders("resources/water_vertex.shader", "resources/water_fragment.shader"));
    item.setTexture("resources/water.png");
    items.push_back(item);


    // Random Rocks
    srand(time(NULL));
    item.clear(2);
    item.setShaderProgram(GlUtilities::loadShaders("resources/tex_vertex.shader", "resources/tex_fragment.shader"));
    item.setNumOfTexture(3);
    item.setTexture("resources/rocky.jpg");
    item.setTexture("resources/rock_nor.png", "nor_map", GL_LINEAR);
    items.push_back(item);
    for (int i = 0; i < R_NUMBER; i++) {

	int x_loc = (rand() % T_WIDTH) - T_WIDTH / 2.0f;
	int z_loc = (rand() % T_HEIGHT) - T_HEIGHT / 2.0f;
	float y_loc = mapHeight(x_loc, z_loc);
	if (y_loc < 0) continue;

	std::vector<glm::vec3> vertices = GlUtilities::genRandomRock(R_MAX_RADIUS, R_POINTS);
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> normals;
	GlUtilities::convexHull(vertices, indices, normals);
	std::vector<glm::vec2> uvs = GlUtilities::genSphericalUVs(vertices);

	item.recycle(3);
	item.setGeometry(vertices);
	item.setTopology(indices);
	item.setUVs(uvs);
	item.setNormals(normals);

	model_matrix = glm::translate(IDENTITY, glm::vec3(x_loc, y_loc, z_loc));
	item.setModelMatrix(model_matrix);
	items.push_back(item);

    }

    // Skybox
    skybox_index = items.size();
    item.clear(2);
    item.loadObject("resources/skybox.obj");

    item.setNumOfTexture(2);
    item.setShaderProgram(GlUtilities::loadShaders("resources/skybox_vertex.shader", "resources/skybox_fragment.shader"));
    item.setTexture("resources/skybox.png", "day", GL_NEAREST);
    item.setTexture("resources/skybox_night.png", "night", GL_NEAREST);
    
    item.setTexture("resources/skybox2.png", "ourTexture1", GL_NEAREST);
    items.push_back(item);

}


void drawGl() {

    // One View Matrix per Iteration
    glm::mat4 view_matrix = setCameraPosition();
    VertexArrayObject::setViewMatrix(view_matrix);
    glm::vec4 eye4d = glm::vec4(eye.x, eye.y, eye.z, 1.0f);
    Item::setEyeLocation(eye4d);


    // Light Direction
    if (light_mov) {
	light_angle += ((30 * delta_time) + 360 % 360); 
	cout << light_angle << endl;
    }
    glm::vec4 light_direction = glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);
    float light_angle_rad = (light_angle * PI / 180.0f);
    glm::mat4 light_rotation = glm::rotate(IDENTITY, light_angle_rad, glm::vec3(0, 0, 1));
    light_direction = light_rotation * light_direction;
    Item::setLightDirection(light_direction);

    // Skybox Model Matrix
    float scalar = T_WIDTH + T_HEIGHT;
    skybox_theta += PI / 240.0f * delta_time;
    glm::mat4 model_matrix = IDENTITY;
    model_matrix = translate(model_matrix, eye);
    model_matrix = glm::scale(model_matrix, glm::vec3(scalar));
    model_matrix = glm::rotate(model_matrix, skybox_theta, up);
    items[skybox_index].setModelMatrix(model_matrix);


    // Draw All Objects
    for (unsigned int i = 0; i < items.size(); i++) {
	items[i].draw();
    }
}

int main() {

    // Initialize Window
    window = GlUtilities::setupGlWindow(INITIAL_WIDTH, INITIAL_HEIGHT); 
    if (window == NULL) return -1;

    // Register Callbacks
    registerCallbacks(window);

    // Run Objects Initialization
    initGl();
    getTimeNow(last_time);

    // Main OpenGL Loop 
    while (!glfwWindowShouldClose(window) && !close_window)
    {
	calcDeltaTime();

	glfwPollEvents();
	processInput();
	glClearColor(BACKGROUND_COLOR, BACKGROUND_COLOR, BACKGROUND_COLOR, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Run Drawing function
	drawGl();

	glfwSwapBuffers(window);
	glfwSwapInterval(1);
    }

    glfwTerminate();
    return 0;
}

glm::mat4 setCameraPosition() {

    // Get Map Height at eye position to clip the character to ground
    if (!free_look) {
	float old_eye_y = eye.y;
	eye.y = mapHeight(eye.x, eye.z) + CHAR_HEIGHT;
	float centerShift = eye.y - old_eye_y;
	center.y += centerShift;
    }

    // Mouse Looking around
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    int screen_center_x = width / 2;
    int screen_center_y = height / 2;
    glfwSetCursorPos(window, screen_center_x, screen_center_y);

    double mov_x = xpos - screen_center_x;
    double mov_y = ypos - screen_center_y;

    glm::mat4 trans = glm::translate(IDENTITY, ORIGIN - (center * 2.0f) + eye);

    // Horizontal Rotation:
    float rad = (mov_x * MOUSE_SENSITIVITY) * PI / 180.0f;
    glm::mat4 rot = glm::rotate(IDENTITY, rad, up);
    glm::vec4 hom_point = glm::vec4(center, 1.0f);
    hom_point = -trans * rot * trans * hom_point;
    center = glm::vec3(hom_point);

    // Vertical Rotation:
    rad = (mov_y *  MOUSE_SENSITIVITY) * PI / 180.0f;
    glm::vec3 axis = glm::normalize(cross(up, (center - eye)));
    rot = glm::rotate(IDENTITY, -rad, axis);
    hom_point = glm::vec4(center, 1.0f);
    hom_point = -trans * rot * trans * hom_point;
    center = glm::vec3(hom_point);

    glm::mat4 view_matrix = glm::lookAt(eye, center, up);
    return view_matrix;
}

void registerCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
}

void windowSizeCallback(GLFWwindow* window, int width, int height) {

    // Define the new Viewport Dimensions{
    glfwGetFramebufferSize(window, & width, &height);
    glViewport(0, 0, width, height);
    glm::mat4 projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, PROJ_NEAR_PLANE, PROJ_FAR_PLANE);
    VertexArrayObject::setProjectionMatrix(projection_matrix);
}

float mapHeight(float x, float z) {

    float char_x = x + T_WIDTH / 2.0f;
    float char_z = z + T_HEIGHT / 2.0f;
    return terrian.getHeight(char_x, char_z);     

}

bool validMove(glm::vec3 step) {

    if (free_look) return true;

    // Water Collistion
    glm::vec3 char_pos = eye - glm::vec3(0, CHAR_HEIGHT, 0);
    glm::vec3 next_pos = char_pos + step;
    if (mapHeight(next_pos.x, next_pos.z) <= 0) {
	return false;
    }

    return true;
}

void move(glm::vec3 direction) {

    glm::vec3 step = mov_speed * delta_time * direction;
    if (validMove(step)) {
	center = center + step;
	eye = eye + step;
    }
}

void getTimeNow(timeval & time) {

#ifdef __linux__ 

    gettimeofday(&time, NULL);

#elif _WIN32

    clock_t time_now = clock();
    time.tv_sec = time_now / CLOCKS_PER_SEC * 2; 
    time.tv_usec = time_now % CLOCKS_PER_SEC * 2;

#endif
}

void calcDeltaTime() {

    timeval current_time;
    getTimeNow(current_time);
    delta_time = (current_time.tv_sec - last_time.tv_sec) + (current_time.tv_usec - last_time.tv_usec) * 0.000001f;
    last_time = current_time;

    one_sec_counter += delta_time;
    frames_counter++;
    if (one_sec_counter >= 1) {
	cout << frames_counter << endl;
	one_sec_counter = 0.0f;
	frames_counter = 0;
    } 

}
