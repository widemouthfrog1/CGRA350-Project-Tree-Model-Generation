#pragma once
#include "Include.h"
#include "Vertex.h"
#include "Rule.h"
#include "Branch.h"

struct SavePoint {
	glm::vec3 position;
	glm::vec3 direction;
	float distance;
	float angle;
	float radius;
	std::shared_ptr<Branch> branch;
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

	float evaluate() {
		if (constructor == 0) {
			if (exp1 != nullptr) {
				if (op == '+') {
					return exp1->evaluate() + exp2->evaluate();
				}
				else if (op == '-') {
					return exp1->evaluate() - exp2->evaluate();
				}
				else if (op == '*') {
					return exp1->evaluate() * exp2->evaluate();
				}
				else if (op == '/') {
					return exp1->evaluate() / exp2->evaluate();
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
	glm::vec3 position;
	glm::vec3 direction;
	std::vector<SavePoint> stack;
	std::vector<Vertex> vertices;
	float distance = 1;
	float angle = glm::pi<float>()/8;
	float radius = 1;
	std::shared_ptr<Branch> branch;
	bool moved = true;
	int id = 0;
	vector<Rule> ruleList;
	int level = 0;
public:
	Turtle(glm::vec3 startingPosition, glm::vec3 startingDirection) {
		this->position = startingPosition;
		this->direction = startingDirection;
	}

	void draw(std::string command);
	cgra::gl_mesh createMesh();
	std::vector<Branch> createBranches(std::string command, int resolution, glm::mat4 groundAngle);
	void loadRules(std::vector<std::string> rules);
	Expression parseExpression(std::string token);
	std::string getCommand(std::string axiom, int depth);
};