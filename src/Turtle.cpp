#include "Turtle.h"

void Turtle::draw(std::string command, std::mt19937 randomNumberGenerator)
{
	vertices.clear();
	
	for (int i = 0; i < command.size(); i++) {
		std::string token;
		token.push_back(command.at(i));
		if (token.at(0) == 'F') {
			glm::vec3 startPosition = position;
			mat4 rotation = glm::rotate(glm::mat4(1), this->angleH, glm::vec3(0, 1, 0))
				*glm::rotate(glm::mat4(1), this->angleL, glm::vec3(0, 0, 1))
				*glm::rotate(glm::mat4(1), this->angleU, glm::vec3(1, 0, 0));
			this->position += vec3(this->rotation * rotation * vec4(0, 1, 0, 1) * distance);
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
			mat4 rotation = glm::rotate(glm::mat4(1), this->angleH, glm::vec3(0, 1, 0))
				*glm::rotate(glm::mat4(1), this->angleL, glm::vec3(0, 0, 1))
				*glm::rotate(glm::mat4(1), this->angleU, glm::vec3(1, 0, 0));
			this->position += vec3(rotation * this->rotation * vec4(0, 1, 0, 1) * distance);
			moved = true;
		}
		else if (token.at(0) == '[') {
			SavePoint point;
			point.position = position;
			point.angleH = this->angleH;
			point.angleL = this->angleL;
			point.angleU = this->angleU;
			point.distance = this->distance;
			point.rotation = this->rotation;
			point.angle = this->angle;
			this->level++;
			stack.push_back(point);
		}
		else if (token.at(0) == ']') {
			SavePoint point = stack.at(stack.size() - 1);
			stack.pop_back();
			this->position = point.position;
			this->angleH = point.angleH;
			this->angleL = point.angleL;
			this->angleU = point.angleU;
			this->rotation = point.rotation;
			this->distance = point.distance;
			this->angle = point.angle;
			this->level--;
			moved = true;
		}
		else if (token.at(0) == '+') {
			this->angleL = angle;
		}
		else if (token.at(0) == '-') {
			this->angleL = -angle;
		}
		else if (token.at(0) == '&') {//pitch down
			this->angleU = -angle;
		}
		else if (token.at(0) == '^') {//pitch up
			this->angleU = angle;
		}
		else if (token.at(0) == '\\') {
			this->angleH = angle;
		}
		else if (token.at(0) == '/') {
			this->angleH = -angle;
		}
		else if (token.at(0) == '|') {
			this->angleL = pi<float>();
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
			this->distance = parseExpression(token).evaluate(randomNumberGenerator);
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
			this->angle = parseExpression(token).evaluate(randomNumberGenerator);
		}
		else if (token.at(0) == 'R') {
		i += 2;
		int j = 0;
		while (true) {
			if (command.at(i) == ')') {
				if (j == 0) {
					break;
				}
				i++;
				j--;
			}
			else {
				if (command.at(i) == '(') {
					j++;
				}
				i++;
			}
		}
		}
	}
}

Expression Turtle::parseExpression(std::string token) {
	for (int i = 0; i < token.size(); i++) {
		if (token.at(i) == '(') {
			int d = 1;
			int j = i;
			for (i++; d > 0; i++) {
				if (token.at(i) == '(') {
					d++;
				}
				if (token.at(i) == ')') {
					d--;
				}
			}
			i--;
			if (i == 1) {
				return Expression(); //empty brackets: ()
			}
			if (i != token.size() - 1) {
				//s.substr(pos, length) NOT s.substr(pos1, pos2)
				//if length > s.size() - pos, length is shrunk to fit string
				return Expression(parseExpression(token.substr(j + 1, i - 1 - j)), token.at(i + 1), parseExpression(token.substr(i + 2, token.size())));
			}
			else {
				if (j != 0) {
					return  Expression(parseExpression(token.substr(0, j - 1)), token.at(j - 1),parseExpression(token.substr(j + 1, token.size() - 2 - j)));
				}
				return parseExpression(token.substr(1, token.size() - 2));
			}
		}
	}
	for (int i = 0; i < token.size(); i++) {
		if (token.at(i) == '+') {
			return Expression(parseExpression(token.substr(0, i)), '+', parseExpression(token.substr(i + 1, token.size())));
		}
		if (token.at(i) == '-') {
			return Expression(parseExpression(token.substr(0, i)), '-', parseExpression(token.substr(i + 1, token.size())));
		}
	}
	for (int i = 0; i < token.size(); i++) {
		if (token.at(i) == '*') {
			return Expression(parseExpression(token.substr(0, i)),'*', parseExpression(token.substr(i + 1, token.size())));
		}
		if (token.at(i) == '/') {
			return Expression(parseExpression(token.substr(0, i)), '/', parseExpression(token.substr(i + 1, token.size())));
		}
		if (token.at(i) == '>') {
			return Expression(parseExpression(token.substr(0, i)), '>', parseExpression(token.substr(i + 1, token.size())));
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
	if (token.at(0) == 'r') {
		return Expression(this->radius);
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

std::vector<Branch> Turtle::createBranches(string command, int resolution, mat4 groundAngle, std::mt19937 randomNumberGenerator)
{
	std::vector<Branch> branches;

	for (int i = 0; i < command.size(); i++) {
		std::string token;
		token.push_back(command.at(i));
		if (token.at(0) == 'F') {
			glm::vec3 startPosition = position;
			mat4 rotation = glm::rotate(glm::mat4(1), this->angleH, glm::vec3(0, 1, 0))
				*glm::rotate(glm::mat4(1), this->angleL, glm::vec3(0, 0, 1))
				*glm::rotate(glm::mat4(1), this->angleU, glm::vec3(1, 0, 0));
			this->position += vec3(this->rotation * rotation * vec4(0, 1, 0, 1) * distance);
			Circle end(position, radius, resolution, this->rotation * rotation);
			this->rotation = this->rotation*rotation;
			std::shared_ptr<Branch> start = make_shared<Branch>(Branch());
			if (branches.size() == 0) {
				start->base = Circle(startPosition, radius, resolution, mat4(1));//ground angle to ba taken into account here
			}
			else if(moved){
				start = branch;
			}
			else {
				start = lastBranch;
			}
			Branch b = Branch(end);
			start->addBranch(b);
			branches.push_back(b);
			moved = false;
		}
		else if (token.at(0) == 'f') {
			//this would make an odd tree
			mat4 rotation = glm::rotate(glm::mat4(1), this->angleH, glm::vec3(0, 1, 0))
				*glm::rotate(glm::mat4(1), this->angleL, glm::vec3(0, 0, 1))
				*glm::rotate(glm::mat4(1), this->angleU, glm::vec3(1, 0, 0));
			this->position += vec3(rotation * vec4(0, 1, 0, 1) * distance);
			branch = make_shared<Branch>(Branch(Circle(position, radius, resolution, mat4(1))));
			moved = true;
		}
		else if (token.at(0) == '[') {
			SavePoint point;
			point.position = position;
			point.angleH = this->angleH;
			point.angleL = this->angleL;
			point.angleU = this->angleU;
			point.rotation = this->rotation;
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
			this->angleH = point.angleH;
			this->angleL = point.angleL;
			this->angleU = point.angleU;
			this->distance = point.distance;
			this->rotation = point.rotation;
			this->angle = point.angle;
			this->radius = point.radius;
			this->branch = point.branch;
			this->level--;
			moved = true;
		}
		else if (token.at(0) == '+') {
			this->angleL = angle;
		}
		else if (token.at(0) == '-') {
			this->angleL = -angle;
		}
		else if (token.at(0) == '&') {//pitch down
			this->angleU = -angle;
		}
		else if (token.at(0) == '^') {//pitch up
			this->angleU = angle;
		}
		else if (token.at(0) == '\\') {
			this->angleH = angle;
		}
		else if (token.at(0) == '/') {
			this->angleH = -angle;
		}
		else if (token.at(0) == '|') {
			this->angleL = pi<float>();
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
			this->distance = parseExpression(token).evaluate(randomNumberGenerator);
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
			this->angle = parseExpression(token).evaluate(randomNumberGenerator);
		}
		else if (token.at(0) == 'R') {
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
		this->radius = parseExpression(token).evaluate(randomNumberGenerator);
		}
	}
	return branches;
}

std::vector<cgra::gl_mesh> Turtle::cylinders(std::string command, int resolution, std::mt19937 randomNumberGenerator)
{
	std::vector<cgra::gl_mesh> cylinders;

	for (int i = 0; i < command.size(); i++) {
		std::string token;
		token.push_back(command.at(i));
		if (token.at(0) == 'F') {
			glm::vec3 startPosition = position;
			mat4 rotation = glm::rotate(glm::mat4(1), this->angleH, glm::vec3(0, 1, 0))
				*glm::rotate(glm::mat4(1), this->angleL, glm::vec3(0, 0, 1))
				*glm::rotate(glm::mat4(1), this->angleU, glm::vec3(1, 0, 0));
			this->position += vec3(this->rotation * rotation * vec4(0, 1, 0, 1) * distance);
			 //vec3(0,1,0) is the initial normal of the circle
			Circle end(position, radius, resolution, this->rotation * rotation);
			this->rotation = this->rotation*rotation;
			std::shared_ptr<Circle> start = make_shared<Circle>(Circle());
			if (cylinders.size() == 0) {
				start = make_shared<Circle>(Circle(startPosition, radius, resolution, mat4(1)));
			}
			else if (moved) {
				start = circle;
			}
			else {
				start = lastCircle;
			}
			cgra::gl_mesh cylinder = this->cylinder(*start, end);
			cylinders.push_back(cylinder);
			lastCircle = make_shared<Circle>(end);
			circle = lastCircle;
			moved = false;
		}
		else if (token.at(0) == 'f') {
			mat4 rotation = glm::rotate(glm::mat4(1), this->angleH, glm::vec3(0, 1, 0))
				*glm::rotate(glm::mat4(1), this->angleL, glm::vec3(0, 0, 1))
				*glm::rotate(glm::mat4(1), this->angleU, glm::vec3(1, 0, 0));
			this->position += vec3(rotation * vec4(0, 1, 0, 1) * distance);
			circle = make_shared<Circle>(Circle(position, radius, resolution, mat4(1)));
			moved = true;
		}
		else if (token.at(0) == '[') {
			SavePoint point;
			point.position = position;
			point.angleH = this->angleH;
			point.angleL = this->angleL;
			point.angleU = this->angleU;
			point.rotation = this->rotation;
			point.distance = this->distance;
			point.angle = this->angle;
			point.radius = this->radius;
			point.circle = this->circle;
			this->level++;
			stack.push_back(point);
		}
		else if (token.at(0) == ']') {
			SavePoint point = stack.at(stack.size() - 1);
			stack.pop_back();
			this->position = point.position;
			this->angleH = point.angleH;
			this->angleL = point.angleL;
			this->angleU = point.angleU;
			this->rotation = point.rotation;
			this->distance = point.distance;
			this->angle = point.angle;
			this->radius = point.radius;
			this->circle = point.circle;
			this->level--;
			moved = true;
		}
		else if (token.at(0) == '+') {
			this->angleL = angle;
		}
		else if (token.at(0) == '-') {
			this->angleL = -angle;
		}
		else if (token.at(0) == '&') {//pitch down
			this->angleU = -angle;
		}
		else if (token.at(0) == '^') {//pitch up
			this->angleU = angle;
		}
		else if (token.at(0) == '\\') {
			this->angleH = angle;
		}
		else if (token.at(0) == '/') {
			this->angleH = -angle;
		}
		else if (token.at(0) == '|') {
			angleL = glm::pi<float>();
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
			this->distance = parseExpression(token).evaluate(randomNumberGenerator);
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
			this->angle = parseExpression(token).evaluate(randomNumberGenerator);
		}
		else if (token.at(0) == 'R') {
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
		this->radius = parseExpression(token).evaluate(randomNumberGenerator);
		}
	}
	return cylinders;
}

cgra::gl_mesh Turtle::cylinder(Circle base, Circle branch)
{
	std::vector<cgra::mesh_vertex> vertices;
	std::vector<unsigned int> indices;
	vec3 center = (branch.center + base.center) / 2.0f;

	cgra::mesh_vertex vertex;
	vertex.pos = base.center;
	vertex.norm = base.center - center;
	vertices.push_back(vertex);
	for (int i = 0; i < base.points.size(); i++) {
		Vertex point = base.getPoints().at(i);
		point.id = id++;
		cgra::mesh_vertex vertex;
		vertex.pos = point.pos;
		vertex.norm = point.pos - center;
		vertices.push_back(vertex);
	}

	vertex;
	vertex.pos = branch.center;
	vertex.norm = branch.center - center;
	vertices.push_back(vertex);
	for (int i = 0; i < branch.points.size(); i++) {
		Vertex point = branch.getPoints().at(i);
		point.id = id++;
		cgra::mesh_vertex vertex;
		vertex.pos = point.pos;
		vertex.norm = point.pos - center;
		vertices.push_back(vertex);
	}

	for (int i = 1; i <= base.points.size(); i++) {
		//link base
		indices.push_back(0);
		indices.push_back(i);
		if (i == base.points.size()) {
			indices.push_back(1);
		}
		else {
			indices.push_back(i + 1);
		}
		//link sides
		int j = i + base.points.size() + 1;
		int k = i + base.points.size() + 2;
		if (k == vertices.size()) {
			k = base.points.size() + 2;
		}
		indices.push_back(i);
		indices.push_back(j);
		indices.push_back(k);
		j = i + 1;
		if (i == base.points.size()) {
			j = 1;
		}
		indices.push_back(i);
		indices.push_back(j);
		indices.push_back(k);
	}
	for (int i = base.points.size() + 2; i < vertices.size(); i++) {
		//link branch
		indices.push_back(base.points.size() + 1);
		indices.push_back(i);
		if (i == vertices.size() - 1) {
			indices.push_back(base.points.size() + 2);
		}
		else {
			indices.push_back(i + 1);
		}
	}

	cgra::mesh_builder builder;
	builder.indices = indices;
	builder.vertices = vertices;
	builder.mode = GL_TRIANGLES;
	return builder.build();
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
		if (input != "") {
			this->ruleList.push_back(Rule(input, output));
		}
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
