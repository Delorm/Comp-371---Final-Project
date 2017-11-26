/*
 * COMP 371: Final Project
 * October 12, 2017
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
#include "l_system.hpp"

#ifdef __linux__ 
    //linux code goes here
    #include "GL/glew.h"	    // include GL Extension Wrangler
    #include "GLFW/glfw3.h"	    // include GLFW helper library
#elif _WIN32
    // windows code goes here
    #include "..\glew\glew.h"	    // include GL Extension Wrangler
    #include "..\glfw\glfw3.h"	    // include GLFW helper library
#endif

using namespace std;

// Configurable Constant Variables
const float MOUSE_SENSITIVITY = 0.1f;
const float CHAR_HEIGHT = 1.0f;	    // Height From Ground
const float WALK_SPEED = 5.0f;
const float FLY_SPEED = 50.0f;
const float PI = 3.14159265359f;

// Terrian
const int T_WIDTH = 128;
const int T_HEIGHT = 128;
const int T_MAX = 10.0f;	    // Highest & Lowest point in terrian
const int T_SHIFT = 2;		    // Increases land to water ratio
const int T_LAND = 10;		    // Radius of Land in the center

// Rocks
const int R_NUMBER = 100;	    // Number of Rocks
const int R_MAX_RADIUS = 3;	    // Max Radius of Rock
const int R_POINTS = 20;	    // Number of points(resolution)

// Tress
const int TR_NUM = 30;		    // Total Number of Trees
const int TR_ITERATIONS = 5;	    // L-System Iterations
const float TR_ALPHA = PI / 4;	    // Branching Angle
const float TR_LENGTH = 1.0f;	    // Initial Trunc Length
const float TR_RADIUS = 0.2;	    // Trunc Radius

// Fog
const float FOG_DENSITY = 0.0145;
const float FOG_GRADIENT = 3.0;


// Constant Variables
const float BACKGROUND_COLOR = 0.4f; 
const float PROJ_NEAR_PLANE = 0.1f;
const float PROJ_FAR_PLANE = 2000.0f;
const GLuint INITIAL_WIDTH = 1280;
const GLuint INITIAL_HEIGHT = 720;
const glm::mat4 IDENTITY = glm::mat4(1.0f);
const glm::vec3 ORIGIN = glm::vec3(0.0f);
const float EPSILON = 0.01f;
const int TYPE_CONVEX_HULL = 0;	    // Numeric Types
const int TYPE_SKY_BOX = 1;
const int TYPE_LEAVES = 2;

// Global Variables
GLFWwindow* window;

std::vector<Item> items;
glm::vec3 center(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 eye(0.0f, 0.0f, 5.0f);

Terrian terrian;
set<int> key_set;
timeval last_time;		// Timekeeping variables
timeval current_time;
int frames_counter = 0;
int skybox_index;
float mov_speed = WALK_SPEED;
float light_angle = 45.0f;
float fog_density = FOG_DENSITY;
float fog_gradient = FOG_GRADIENT;
float one_sec_counter = 0.0f;
float delta_time = 0.0f;
float skybox_theta = 0.0f;
bool fog = true;
bool free_look = false;
bool wireframe = false; 
bool close_window = false;
bool light_mov = true;


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
float angle(glm::vec3, glm::vec3, glm::vec3);
void printVector(glm::vec3);
void printVector(char*, glm::vec3); 
void addItem(glm::vec3);
bool checkInsideTriangle(std::vector<glm::vec3>, glm::vec3);
glm::vec3 findValidPosition(void);


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

	    case GLFW_KEY_C: {
		key_set.erase(key);
		for (int i = 0; i < items.size(); i++) {
		    if (items[i].type == TYPE_CONVEX_HULL) {
			items[i].vao.visibility = !items[i].vao.visibility;
		    }
		} 
		break;
	    }

	    case GLFW_KEY_F: {
		key_set.erase(key);
		fog = !fog;
		if (fog) {
		    fog_density = FOG_DENSITY;
		    fog_gradient = FOG_GRADIENT;
		} else {
		    fog_density = 0.0f;
		    fog_gradient = 1.0f;
		}

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

    terrian = Terrian(T_WIDTH, T_HEIGHT, T_MAX, T_SHIFT, T_LAND);
    item.setGeometry(terrian.generateMap());
    item.setTopology(terrian.findIndices());
    item.setUVs(terrian.generateUVs());
    item.setNormals(terrian.generateNormals());

    // Texture
    item.setNumOfTexture(7);
    item.setShaderProgram(GlUtilities::loadShaders("terrain_vertex", "terrain_fragment"));
    item.setTexture("grass", "grass", GL_LINEAR);
    item.setTexture("terrain_background", "background", GL_LINEAR);
    item.setTexture("terrain_r", "r_texture", GL_LINEAR);
    item.setTexture("terrain_g", "g_texture", GL_LINEAR);
    item.setTexture("terrain_b", "b_texture", GL_LINEAR);
    item.setTexture("blend_map", "blend_map", GL_LINEAR);
    item.setTexture("normal_map", "nor_map", GL_LINEAR);

    model_matrix = glm::translate(IDENTITY, glm::vec3( (float)-T_WIDTH / 2.0f, 0.0f, (float)-T_HEIGHT / 2.0f));
    item.setModelMatrix(model_matrix);
    items.push_back(item);
    

    // Fern
    item.clear(3);
    item.loadObject("fern");
    model_matrix = glm::translate(IDENTITY, glm::vec3(32, 4, 0));
    item.setModelMatrix(model_matrix);
    item.setShaderProgram(GlUtilities::loadShaders("tex_vertex", "tex_fragment"));
    item.setTexture("fern");
    items.push_back(item);


    // Grass
    glm::mat4 trans;
    trans = glm::rotate(trans, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    item.clear(3);
    item.loadObject("grassModel");
    model_matrix = glm::translate(IDENTITY, glm::vec3(2, 2, 0));
    item.setModelMatrix(model_matrix);
    item.setShaderProgram(GlUtilities::loadShaders("tex_vertex", "tex_fragment"));
    item.setTexture("grassTexture");
    items.push_back(item);
    
    // Water plane
    /*
    item.clear(1);
    item.loadObject("water");
    model_matrix = glm::scale(IDENTITY, glm::vec3(T_WIDTH, 0, T_HEIGHT));
    item.setModelMatrix(model_matrix);
    item.setShaderProgram(GlUtilities::loadShaders("water_vertex", "water_fragment"));
    item.setTexture("water");
    items.push_back(item);
    */

    // Random Rocks
    srand(time(NULL));
    item.clear(3);
    item.setShaderProgram(GlUtilities::loadShaders("rock_vertex", "rock_fragment"));
    item.setNumOfTexture(2);
    item.setTexture("rocky", "ourTexture1", GL_LINEAR);
    item.setTexture("rock_nor", "nor_map", GL_LINEAR);

    Item volume(1);		// Bounding Volume
    volume.setShaderProgram(GlUtilities::loadShaders("vertex", "fragment"));

    for (int i = 0; i < R_NUMBER; i++) {

	glm::vec3 position = findValidPosition();	
	if (position == glm::vec3(0)) continue;

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
	item.position = position;

	model_matrix = glm::translate(IDENTITY, position);
	item.setModelMatrix(model_matrix);
	items.push_back(item);

	// Bounding Volume
	volume.clear(1);
	GlUtilities::convexHull(vertices, indices, normals);
	volume.vao.setVisibility(false);
	volume.setGeometry(vertices);
	volume.setTopology(indices);
	volume.normals = normals;
	volume.setModelMatrix(model_matrix);
	volume.type = TYPE_CONVEX_HULL;
	volume.position = position;
	volume.setCollidable(true);
	items.push_back(volume);
    }
    item.setCollidable(false);

    // Skybox
    skybox_index = items.size();
    item.clear(3);
    item.loadObject("skybox");

    item.setNumOfTexture(2);
    item.setShaderProgram(GlUtilities::loadShaders("skybox_vertex", "skybox_fragment"));
    item.setTexture("skybox", "day", GL_NEAREST);
    item.setTexture("skybox_night", "night", GL_NEAREST);
    items.push_back(item);



    // L-System
    item.clear(3);		// Bark
    item.setShaderProgram(GlUtilities::loadShaders("tex_vertex", "tex_fragment"));
    item.setTexture("trunc");

    Item leaves(3);		// Leaves
    leaves.setShaderProgram(GlUtilities::loadShaders("tex_vertex", "tex_fragment"));
    leaves.setTexture("leaf");
    
    volume.clear(1);		// Bounding Volume
    volume.setShaderProgram(GlUtilities::loadShaders("vertex", "fragment"));
     
    for (int i = 0; i < TR_NUM; i++) {

	glm::vec3 position = findValidPosition();
	if (position == glm::vec3(0)) continue;
	position.y -= 0.01;


	std::vector<glm::vec3> vertices, l_vertices;
	std::vector<unsigned int> indices, l_indices;
	std::vector<glm::vec2> uvs, l_uvs;
	std::vector<glm::vec3> normals, l_normals;


	// Randomness
	float random = (float)rand() / RAND_MAX;
	LSystem lsystem(random * TR_LENGTH, random * TR_RADIUS, max(random * TR_ALPHA, PI/6.0f));
	lsystem.generate(TR_ITERATIONS);
	lsystem.getTree(vertices, indices, uvs, normals, l_vertices, l_indices, l_uvs, l_normals);

	// Bark
	item.recycle(3);
	item.setGeometry(vertices);
	item.setTopology(indices);
	item.setUVs(uvs);
	item.setNormals(normals);
	model_matrix = glm::translate(IDENTITY, position);
	item.setModelMatrix(model_matrix);
	items.push_back(item);

	// Bounding Volume
	volume.clear(1);
	GlUtilities::convexHull(vertices, indices, normals);
	volume.vao.setVisibility(false);
	volume.setGeometry(vertices);
	volume.setTopology(indices);
	volume.normals = normals;
	volume.setModelMatrix(model_matrix);
	volume.type = TYPE_CONVEX_HULL;
	volume.position = position;
	volume.setCollidable(true);
	items.push_back(volume);

	
	// Leaves
	leaves.recycle(3);
	leaves.setGeometry(l_vertices);
	leaves.setTopology(l_indices);
	leaves.setUVs(l_uvs);
	leaves.setNormals(l_normals);
	leaves.vao.setPrimitive(VertexArrayObject::TRIANGLES);
	leaves.setModelMatrix(model_matrix);
	leaves.type = TYPE_LEAVES;
	items.push_back(leaves);
	

	// Clear Memory
	vertices.clear(); indices.clear(); uvs.clear(); normals.clear();
	l_vertices.clear(); l_indices.clear(); l_uvs.clear(); l_normals.clear();
    }

}


void drawGl() {

    // One View Matrix per Iteration
    glm::mat4 view_matrix = setCameraPosition();
    VertexArrayObject::setViewMatrix(view_matrix);
    VertexArrayObject::fog_density = fog_density;
    VertexArrayObject::fog_gradient = fog_gradient;
    glm::vec4 eye4d = glm::vec4(eye.x, eye.y, eye.z, 1.0f);
    Item::setEyeLocation(eye4d);


    // Light Direction
    if (light_mov) {
	light_angle += ((30 * delta_time) + 360 % 360); 
    }
    glm::vec4 light_direction = glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);
    float light_angle_rad = (light_angle * PI / 180.0f);
    glm::mat4 light_rotation = glm::rotate(IDENTITY, light_angle_rad, glm::vec3(0, 0, 1));
    light_direction = light_rotation * light_direction;
    Item::setLightDirection(light_direction);

    // Fog SkyBlend Color (adjusted with Daytime)
    float bg = BACKGROUND_COLOR;
    float mult = (dot(-glm::vec3(light_direction), up) * 0.2f)  + 0.5f;
    glm::vec3 skyColor = glm::vec3(mult, mult, mult);
    VertexArrayObject::loadSkyColor(skyColor);

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
	if (items[i].type == TYPE_CONVEX_HULL) {
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else if (wireframe) {
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
	    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
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
	//return false;
    }

    // Terrain Size Bounding
    float safety_margin = 2.0f;
    if (abs(next_pos.x) + safety_margin >= T_WIDTH  / 2.0f) return false;
    if (abs(next_pos.z) + safety_margin >= T_HEIGHT / 2.0f) return false;

    // Collision Detection:
    glm::vec3 dir = glm::normalize(step);
    glm::vec3 pos = eye;
    for (int i = 0; i < items.size(); i++) {
	Item item = items[i];
	if (item.isCollidable()) {

	    //Spherical Bounding Volume
	    if (glm::distance(eye, item.position) > R_MAX_RADIUS * 2) continue;	

	    std::vector<glm::vec3> vertices = item.vertices;
	    std::vector<unsigned int> indices = item.edges;
	    std::vector<glm::vec3> normals = item.normals;
	    std::vector<float> d_coeff = item.d_coeff;
	    std::vector<glm::vec3> points;

	    for (int j = 0; j < indices.size(); j += 3) {
		glm::vec3 normal = normals[j];
		if (dot(normal, step) >= 0) continue;	    // Back Face Optimization

		points.clear();
		points.push_back(vertices[indices[j+0]]);
		points.push_back(vertices[indices[j+1]]);
		points.push_back(vertices[indices[j+2]]);

		// Get t:
		float d = d_coeff[j];
		float t = - (glm::dot(normal, pos) + d) / glm::dot(normal, dir);
		if (t > glm::length(step) + 1 || t < 0) continue;
		glm::vec3 point = pos + t * dir ;

		bool inside = checkInsideTriangle(points, point);

		if (inside) {              // Point is inside the triangle
		    return false;
		}
	    }
	}
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

void printVector(glm::vec3 vec) {
    cout << vec.x << " " << vec.y << " " << vec.z << endl;
}

void printVector(char* name, glm::vec3 vec) {
    cout << name << ": " << vec.x << " " << vec.y << " " << vec.z << endl;
}

void addItem(glm::vec3 pos) {
    glPointSize(10);
    Item item(1);
    std::vector<glm::vec3> vertices; vertices.push_back(pos);
    item.setGeometry(vertices);

    item.setShaderProgram(GlUtilities::loadShaders("resources/vertex.shader", "resources/fragment.shader"));
    item.vao.setDrawingMode(VertexArrayObject::VERTICES);
    item.vao.setPrimitive(VertexArrayObject::POINTS);
    items.push_back(item);
}

bool checkInsideTriangle(std::vector<glm::vec3> t, glm::vec3 p) {

    glm::vec3 u1 = t[1] - t[0];
    glm::vec3 u2 = t[2] - t[0];
    float area = glm::length(glm::cross(u1, u2)) / 2.0f;

    float a = (float)glm::length(glm::cross(t[0] - p, t[1] - p)) / (2.0f * area);
    float b = (float)glm::length(glm::cross(t[1] - p, t[2] - p)) / (2.0f * area);
    float c = (float)glm::length(glm::cross(t[2] - p, t[0] - p)) / (2.0f * area);

    if (abs(a+b+c - 1.0) < EPSILON && a >= 0 && a <= 1 && b >= 0 && b <= 1 && c >= 0 && c <= 1) {
	return true;
    }
    return false;
} 

glm::vec3 findValidPosition() {

    int x_loc = (rand() % T_WIDTH) - T_WIDTH / 2.0f;
    int z_loc = (rand() % T_HEIGHT) - T_HEIGHT / 2.0f;
    float y_loc = mapHeight(x_loc, z_loc);

    // Under Water Rejection
    if (y_loc < 0) {
	return glm::vec3(0);
    }	

    // Center Spawn Rejection
    if (abs(x_loc) < T_LAND && abs(z_loc) < T_LAND) {
	return glm::vec3(0);
    }	

    return glm::vec3(x_loc, y_loc, z_loc);
}
