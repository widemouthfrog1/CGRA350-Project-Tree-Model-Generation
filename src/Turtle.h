#pragma once
#include "Include.h"
#include "Vertex.h"
#include "Rule.h"
#include "Branch.h"

struct SavePoint {
	glm::vec3 position;
	float angleH;
	float angleL;
	float angleU;
	glm::mat4 rotation;
	float distance;
	float angle;
	float radius;
	std::shared_ptr<Branch> branch;
	std::shared_ptr<Circle> circle;//for cylinders
};


struct Expression {
	std::shared_ptr<Expression> exp1;
	std::shared_ptr<Expression> exp2;
	char op;
	float number;
	int constructor;

	Expression(Expression exp1, char op,  Expression exp2) {
		this->exp1 = std::make_shared<Expression>(exp1);
		this->exp2 = std::make_shared<Expression>(exp2);
		this->op = op;
		constructor = 0;
	}
	Expression(float number) {
		this->number = number;
		constructor = 1;
	}
	Expression() {
		constructor = 2;
	}
	string toString() {
		if (constructor == 0) {
			return "(" +exp1->toString() + op + exp2->toString() + ")";
		}
		else if (constructor == 1) {
			return std::to_string(number);
		}
		else if (constructor == 2) {
			return "NAN";
		}
	}

	float evaluate(std::mt19937 &randomNumberGenerator) {
		if (constructor == 0) {
			if (exp1 != nullptr) {
				if (op == '+') {
					return exp1->evaluate(randomNumberGenerator) + exp2->evaluate(randomNumberGenerator);
				}
				else if (op == '-') {
					return exp1->evaluate(randomNumberGenerator) - exp2->evaluate(randomNumberGenerator);
				}
				else if (op == '*') {
					return exp1->evaluate(randomNumberGenerator) * exp2->evaluate(randomNumberGenerator);
				}
				else if (op == '/') {
					return exp1->evaluate(randomNumberGenerator) / exp2->evaluate(randomNumberGenerator);
				}
				else if (op == '>') {
					std::uniform_real_distribution<float> dis(exp1->evaluate(randomNumberGenerator), exp2->evaluate(randomNumberGenerator));
					return dis(randomNumberGenerator);
				}
			}
			else {
				return number;
			}
		}
		else if (constructor == 1) {
			return number;
		}
		else if (constructor == 2) {
			return NAN;
		}
	}

};

class Turtle {
	glm::vec3 position = vec3(0,0,0);
	float angleH = 0;
	float angleL = 0;
	float angleU = 0;
	glm::mat4 rotation = mat4(1);
	std::vector<SavePoint> stack;
	std::vector<Vertex> vertices;
	float distance = 1;
	float angle = glm::pi<float>()/8;
	float radius = 0.1;
	std::shared_ptr<Branch> branch;
	std::shared_ptr<Branch> lastBranch;
	bool moved = true;
	int id = 0;
	vector<Rule> ruleList;
	int level = 0;
	std::shared_ptr<Circle> circle;
	std::shared_ptr<Circle> lastCircle;
public:
	Turtle(glm::mat4 rotation) {
		this->rotation = rotation;
	}
	Turtle() {
	}

	void draw(std::string command, std::mt19937 randomNumberGenerator);
	cgra::gl_mesh createMesh();
	std::vector<Branch> createBranches(std::string command, int resolution, glm::mat4 groundAngle, std::mt19937 randomNumberGenerator);
	std::vector<cgra::gl_mesh> cylinders(std::string command, int resolution, std::mt19937 randomNumberGenerator);
	cgra::gl_mesh cylinder(Circle base, Circle branch);
	void loadRules(std::vector<std::string> rules);
	Expression parseExpression(std::string token);
	std::string getCommand(std::string axiom, int depth);
};