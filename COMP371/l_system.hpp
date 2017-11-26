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
	LSystem();
	void makeRule(string, string);
	string getMessage();
	void generate(int);
	void getBark(
		std::vector<glm::vec3> &, 
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

	float getMultiplier(int &);

    private:
	std::vector<Rule> rules;
	string axiom;
	string message;
	const float PI = 3.14159265359f;
	float alpha = PI / 4.0f;
	float random = 0.1;
	float points = 10;


	
};


#endif
