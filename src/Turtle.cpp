#include "Turtle.h"

void Turtle::draw(std::string command)
{
	vertices.clear();
	
	for (int i = 0; i < command.size(); i++) {
		std::string token;
		token.push_back(command.at(i));
		if (token.at(0) == 'F') {
			glm::vec3 startPosition = position;
			this->position += direction * distance;
			Vertex end;
			end.pos = position;
			
			if (moved) {
				Vertex start;
				start.pos = startPosition;
				start.id = id++;
				end.id = id++;
				end.link(start);
				vertices.push_back(start);
			}
			else {
				end.id = id++;
				end.link(vertices.at(vertices.size() - 1));
			}
			vertices.push_back(end);
			moved = false;
		}
		else if (token.at(0) == 'f') {
			this->position += direction * distance;
			moved = true;
		}
		else if (token.at(0) == '[') {
			SavePoint point;
			point.position = position;
			point.direction = direction;
			point.distance = this->distance;
			point.angle = this->angle;
			this->level++;
			stack.push_back(point);
		}
		else if (token.at(0) == ']') {
			SavePoint point = stack.at(stack.size() - 1);
			stack.pop_back();
			this->position = point.position;
			this->direction = point.direction;
			this->distance = point.distance;
			this->angle = point.angle;
			this->level--;
			moved = true;
		}
		else if (token.at(0) == '+') {
			glm::mat4 rotation = glm::rotate(glm::mat4(1), this->angle, glm::vec3(0, 0, 1));
			this->direction = rotation * glm::vec4(direction, 1);
		}
		else if (token.at(0) == '-') {
			glm::mat4 rotation = glm::rotate(glm::mat4(1), -this->angle, glm::vec3(0, 0, 1));
			this->direction = rotation * glm::vec4(direction, 1);
		}
		else if (token.at(0) == '&') {//pitch down
			glm::mat4 rotation = glm::rotate(glm::mat4(1), this->angle, glm::vec3(-1, 0, 0));
			this->direction = rotation * glm::vec4(direction, 1);
		}
		else if (token.at(0) == '^') {//pitch up
			glm::mat4 rotation = glm::rotate(glm::mat4(1), -this->angle, glm::vec3(-1, 0, 0));
			this->direction = rotation * glm::vec4(direction, 1);
		}
		else if (token.at(0) == '\\') {
			glm::mat4 rotation = glm::rotate(glm::mat4(1), this->angle, glm::vec3(0, 1, 0));
			this->direction = rotation * glm::vec4(direction, 1);
		}
		else if (token.at(0) == '/') {
			glm::mat4 rotation = glm::rotate(glm::mat4(1), -this->angle, glm::vec3(0, 1, 0));
			this->direction = rotation * glm::vec4(direction, 1);
		}
		else if (token.at(0) == '|') {
			glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 0, 1));
			this->direction = rotation * glm::vec4(direction, 1);
		}
		else if (token.at(0) == 'D') {
			token = "";
			i += 2;
			int j = 0;
			while (true) {
				if (command.at(i) == ')') {
					if (j == 0) {
						break;
					}
					token += command.at(i);
					i++;
					j--;
				}
				else {
					if (command.at(i) == '(') {
						j++;
					}
					token += command.at(i);
					i++;
				}
			}
			this->distance = parseExpression(token).evaluate();
		}
		else if (token.at(0) == 'A') {
			token = "";
			i += 2;
			int j = 0;
			while (true) {
				if (command.at(i) == ')') {
					if (j == 0) {
						break;
					}
					token += command.at(i);
					i++;
					j--;
				}
				else {
					if (command.at(i) == '(') {
						j++;
					}
					token += command.at(i);
					i++;
				}
			}
			this->angle = parseExpression(token).evaluate();
		}
	}
}

Expression Turtle::parseExpression(std::string token) {
	for (int i = 0; i < token.size(); i++) {
		if (token.at(i) == '(') {
			int j = i;
			int d = 1;
			for (i++; d > 0; i++) {
				if (token.at(i) == '(') {
					d++;
				}
				if (token.at(i) == ')') {
					d--;
				}
			}
			i--;
			if (j - i == 1) {
				return Expression(); //empty brackets: ()
			}
			if (i == token.size() - 1) {
				if (token.at(j - 1) == '*') {
					return Expression(parseExpression(token.substr(0, j - 2)),'*',parseExpression(token.substr(j + 1, i - 1)));
				}
				if (token.at(j - 1) == '/') {
					return Expression(parseExpression(token.substr(0, j - 2)),'/',parseExpression(token.substr(j + 1, i - 1)));
				}
				if (token.at(j - 1) == '+') {
					return Expression(parseExpression(token.substr(0, j - 2)),'+',parseExpression(token.substr(j + 1, i - 1)));
				}
				if (token.at(j - 1) == '-') {
					return Expression(parseExpression(token.substr(0, j - 2)),'-',parseExpression(token.substr(j + 1, i - 1)));
				}
			}
			if (token.at(i + 1) == '*') {
				if (j == 0) {
					return Expression(parseExpression(token.substr(j + 1, i - 1)),'*',parseExpression(token.substr(i + 2, token.size() - 1)));
				}
				if (token.at(j - 1) == '*') {
					return Expression(parseExpression(token.substr(0, j - 2)),'*', Expression(parseExpression(token.substr(j + 1, i - 1)),'*',parseExpression(token.substr(i + 2, token.size() - 1))));
				}
				if (token.at(j - 1) == '/') {
					return Expression(parseExpression(token.substr(0, j - 2)),'/', Expression(parseExpression(token.substr(j + 1, i - 1)),'*',parseExpression(token.substr(i + 2, token.size() - 1))));
				}
				if (token.at(j - 1) == '+') {
					return Expression(parseExpression(token.substr(0, j - 2)),'+', Expression(parseExpression(token.substr(j + 1, i - 1)),'*',parseExpression(token.substr(i + 2, token.size() - 1))));
				}
				if (token.at(j - 1) == '-') {
					return Expression(parseExpression(token.substr(0, j - 2)),'-', Expression(parseExpression(token.substr(j + 1, i - 1)),'*',parseExpression(token.substr(i + 2, token.size() - 1))));
				}
			}
			if (token.at(i + 1) == '/') {
				if (j == 0) {
					return Expression(parseExpression(token.substr(j + 1, i - 1)), '/', parseExpression(token.substr(i + 2, token.size() - 1)));
				}
				if (token.at(j - 1) == '*') {
					return Expression(parseExpression(token.substr(0, j - 2)), '*', Expression(parseExpression(token.substr(j + 1, i - 1)), '/', parseExpression(token.substr(i + 2, token.size() - 1))));
				}
				if (token.at(j - 1) == '/') {
					return Expression(parseExpression(token.substr(0, j - 2)), '/', Expression(parseExpression(token.substr(j + 1, i - 1)), '/', parseExpression(token.substr(i + 2, token.size() - 1))));
				}
				if (token.at(j - 1) == '+') {
					return Expression(parseExpression(token.substr(0, j - 2)), '+', Expression(parseExpression(token.substr(j + 1, i - 1)), '/', parseExpression(token.substr(i + 2, token.size() - 1))));
				}
				if (token.at(j - 1) == '-') {
					return Expression(parseExpression(token.substr(0, j - 2)), '-', Expression(parseExpression(token.substr(j + 1, i - 1)), '/', parseExpression(token.substr(i + 2, token.size() - 1))));
				}
			}
			if (token.at(i + 1) == '+') {
				if (j == 0) {
					return Expression(parseExpression(token.substr(j + 1, i - 1)), '+', parseExpression(token.substr(i + 2, token.size() - 1)));
				}
				if (token.at(j - 1) == '*') {
					return Expression(parseExpression(token.substr(0, j - 2)), '*', Expression(parseExpression(token.substr(j + 1, i - 1)), '+', parseExpression(token.substr(i + 2, token.size() - 1))));
				}
				if (token.at(j - 1) == '/') {
					return Expression(parseExpression(token.substr(0, j - 2)), '/', Expression(parseExpression(token.substr(j + 1, i - 1)), '+', parseExpression(token.substr(i + 2, token.size() - 1))));
				}
				if (token.at(j - 1) == '+') {
					return Expression(parseExpression(token.substr(0, j - 2)), '+', Expression(parseExpression(token.substr(j + 1, i - 1)), '+', parseExpression(token.substr(i + 2, token.size() - 1))));
				}
				if (token.at(j - 1) == '-') {
					return Expression(parseExpression(token.substr(0, j - 2)), '-', Expression(parseExpression(token.substr(j + 1, i - 1)), '+', parseExpression(token.substr(i + 2, token.size() - 1))));
				}
			}
			if (token.at(i + 1) == '-') {
				if (j == 0) {
					return Expression(parseExpression(token.substr(j + 1, i - 1)), '-', parseExpression(token.substr(i + 2, token.size() - 1)));
				}
				if (token.at(j - 1) == '*') {
					return Expression(parseExpression(token.substr(0, j - 2)), '*', Expression(parseExpression(token.substr(j + 1, i - 1)), '-', parseExpression(token.substr(i + 2, token.size() - 1))));
				}
				if (token.at(j - 1) == '/') {
					return Expression(parseExpression(token.substr(0, j - 2)), '/', Expression(parseExpression(token.substr(j + 1, i - 1)), '-', parseExpression(token.substr(i + 2, token.size() - 1))));
				}
				if (token.at(j - 1) == '+') {
					return Expression(parseExpression(token.substr(0, j - 2)), '+', Expression(parseExpression(token.substr(j + 1, i - 1)), '-', parseExpression(token.substr(i + 2, token.size() - 1))));
				}
				if (token.at(j - 1) == '-') {
					return Expression(parseExpression(token.substr(0, j - 2)), '-', Expression(parseExpression(token.substr(j + 1, i - 1)), '-', parseExpression(token.substr(i + 2, token.size() - 1))));
				}
			}
		}
	}
	for (int i = 0; i < token.size(); i++) {
		if (token.at(i) == '+') {
			return Expression(parseExpression(token.substr(0, i)), '+', parseExpression(token.substr(i + 1, token.size() - i - 1)));
		}
		if (token.at(i) == '-') {
			return Expression(parseExpression(token.substr(0, i)), '-', parseExpression(token.substr(i + 1, token.size() - i - 1)));
		}
	}
	for (int i = 0; i < token.size(); i++) {
		if (token.at(i) == '*') {
			return Expression(parseExpression(token.substr(0, i)),'*', parseExpression(token.substr(i + 1, token.size() - i - 1)));
		}
		if (token.at(i) == '/') {
			return Expression(parseExpression(token.substr(0, i)), '/', parseExpression(token.substr(i + 1, token.size() - i - 1)));
		}
	}
	if (token.at(0) == 'd') {
		return Expression(this->distance);
	}
	if (token.at(0) == 'a') {
		return Expression(this->angle);
	}
	if (token.at(0) == 'l') {
		return Expression(this->level);
	}
	return Expression(std::stof(token, 0));//it must be a float now if correct
}


cgra::gl_mesh Turtle::createMesh()
{
	for (int i = 0; i < this->vertices.size(); i++) {
		this->vertices.at(i).resetConnections();
	}
	std::vector<cgra::mesh_vertex> vertices;
	std::vector<unsigned int> indices;
	for (int i = 0; i < this->vertices.size(); i++) {
		cgra::mesh_vertex vertex;
		vertex.pos = this->vertices.at(i).pos;
		vertices.push_back(vertex);
	}
	for (int i = 0; i < this->vertices.size(); i++) {
		Vertex* vertex = &this->vertices.at(i);
		for (int j = 0; j < vertex->connectionsSize(); j++) {
			if (vertex->useConnection(this->vertices.at(vertex->getConnection(j)))) {
				indices.push_back(vertex->id);
				indices.push_back(vertex->getConnection(j));
			}
		}
	}
	cgra::mesh_builder builder;
	builder.indices = indices;
	builder.vertices = vertices;
	builder.mode = GL_LINES;
	return builder.build();
}

std::vector<Branch> Turtle::createBranches(string command, int resolution, mat4 groundAngle)
{
	std::vector<Branch> branches;
	for (int i = 0; i < command.size(); i++) {
		std::string token;
		token.push_back(command.at(i));
		if (token.at(0) == 'F') {
			glm::vec3 startPosition = position;
			this->position += direction * distance;
			mat4 rotation = glm::rotate(mat4(1), acos(glm::dot(direction, vec3(0, 1, 0))), glm::cross(direction, vec3(0, 1, 0)));
			Circle end(position, radius, 1, rotation);
			std::shared_ptr<Branch> start = make_shared<Branch>(Branch());
			if (branches.size() == 0) {
				start->base = Circle(startPosition, radius, resolution, groundAngle);
			}
			else if(moved){
				start = branch;
			}
			else {
				start = make_shared<Branch>(branches.at(branches.size() - 1));
			}
			Branch b = Branch(end);
			start->addBranch(b);
			branches.push_back(b);
			moved = false;
		}
		else if (token.at(0) == 'f') {
			//this would make an odd tree
			this->position += direction * distance;
			moved = true;
		}
		else if (token.at(0) == '[') {
			SavePoint point;
			point.position = position;
			point.direction = direction;
			point.distance = this->distance;
			point.angle = this->angle;
			point.radius = this->radius;
			point.branch = this->branch;
			this->level++;
			stack.push_back(point);
		}
		else if (token.at(0) == ']') {
			SavePoint point = stack.at(stack.size() - 1);
			stack.pop_back();
			this->position = point.position;
			this->direction = point.direction;
			this->distance = point.distance;
			this->angle = point.angle;
			this->radius = point.radius;
			this->branch = point.branch;
			this->level--;
			moved = true;
		}
		else if (token.at(0) == '+') {
			glm::mat4 rotation = glm::rotate(glm::mat4(1), this->angle, glm::vec3(0, 0, 1));
			this->direction = rotation * glm::vec4(direction, 1);
		}
		else if (token.at(0) == '-') {
			glm::mat4 rotation = glm::rotate(glm::mat4(1), -this->angle, glm::vec3(0, 0, 1));
			this->direction = rotation * glm::vec4(direction, 1);
		}
		else if (token.at(0) == '&') {//pitch down
			glm::mat4 rotation = glm::rotate(glm::mat4(1), this->angle, glm::vec3(-1, 0, 0));
			this->direction = rotation * glm::vec4(direction, 1);
		}
		else if (token.at(0) == '^') {//pitch up
			glm::mat4 rotation = glm::rotate(glm::mat4(1), -this->angle, glm::vec3(-1, 0, 0));
			this->direction = rotation * glm::vec4(direction, 1);
		}
		else if (token.at(0) == '\\') {
			glm::mat4 rotation = glm::rotate(glm::mat4(1), this->angle, glm::vec3(0, 1, 0));
			this->direction = rotation * glm::vec4(direction, 1);
		}
		else if (token.at(0) == '/') {
			glm::mat4 rotation = glm::rotate(glm::mat4(1), -this->angle, glm::vec3(0, 1, 0));
			this->direction = rotation * glm::vec4(direction, 1);
		}
		else if (token.at(0) == '|') {
			glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 0, 1));
			this->direction = rotation * glm::vec4(direction, 1);
		}
		else if (token.at(0) == 'D') {
			token = "";
			i += 2;
			int j = 0;
			while (true) {
				if (command.at(i) == ')') {
					if (j == 0) {
						break;
					}
					token += command.at(i);
					i++;
					j--;
				}
				else {
					if (command.at(i) == '(') {
						j++;
					}
					token += command.at(i);
					i++;
				}
			}
			this->distance = parseExpression(token).evaluate();
		}
		else if (token.at(0) == 'A') {
			token = "";
			i += 2;
			int j = 0;
			while (true) {
				if (command.at(i) == ')') {
					if (j == 0) {
						break;
					}
					token += command.at(i);
					i++;
					j--;
				}
				else {
					if (command.at(i) == '(') {
						j++;
					}
					token += command.at(i);
					i++;
				}
			}
			this->angle = parseExpression(token).evaluate();
		}
	}
	return branches;
}

void Turtle::loadRules(std::vector<std::string> rules)
{
	for (int i = 0; i < rules.size(); i++) {
		std::string input;
		std::string output;
		bool foundColon = false;
		for (char c : rules.at(i)) {
			if (foundColon) {
				output.push_back(c);
			}
			else {
				if (c == ':') {
					foundColon = true;
				}
				else {
					input.push_back(c);
				}
			}
		}
		this->ruleList.push_back(Rule(input, output));
	}
}

std::string Turtle::getCommand(std::string axiom, int depth)
{
	std::string command = std::string(axiom);
	for(int i = 0; i < depth; i++){
		for (Rule rule : this->ruleList) {
			command = rule.applyRule(command);
		}
	}
	return command;
}
