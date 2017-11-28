/*
 * COMP 271 Project
 * October 12, 2017
 * Code Adopted and Modified
 * From learnopengl.com
 */

#ifndef _L_SYSTEM_H
#define _L_SYSTEM_H

#include "iostream"
#include "string"
#include "cstring"
#include "string.h"
#include "vector"
#include "glm/glm.hpp"

using namespace std;


class LSystem {

    public: 
	struct Rule {	
	    string a;
	    string b;
	};
	LSystem(float, float, float);
	void makeRule(string, string);		// Adds a rule to the grammer
	string getMessage();
	void generate(int);			// Takes number of iterations
	void getTree(
		std::vector<glm::vec3> &,	// Trunc Vectors
		std::vector<unsigned int> &, 
		std::vector<glm::vec2> &, 
		std::vector<glm::vec3> &, 
		std::vector<glm::vec3> &,	// Leaves Vectors
		std::vector<unsigned int> &, 
		std::vector<glm::vec2> &, 
		std::vector<glm::vec3> &
		);

	void drawBark(
		glm::vec3 &, glm::vec3 &, 
		float, std::vector<glm::vec3> &, 
		std::vector<unsigned int> &, 
		std::vector<glm::vec2> &, 
		std::vector<glm::vec3> &
		);

	void drawLeaves(
		glm::vec3 &, glm::vec3 &, 
		float, std::vector<glm::vec3> &, 
		std::vector<unsigned int> &, 
		std::vector<glm::vec2> &, 
		std::vector<glm::vec3> &
		);

	float getMultiplier(int &);

    private:
	std::vector<Rule> rules;
	string axiom;
	string message;
	const float PI = 3.14159265359f;
	float alpha = PI / 4.0f;	    // Angle of brancing
	float random = 0.5;
	float points = 5;		    // or sides of cylinder
	float l_width = 0.1f;		    // Leaf width
	float l_height = 0.4f;		    // Leaf Height
	float radius;			    // Bark Initial Radius
	float seg_length;


	
};


#endif
