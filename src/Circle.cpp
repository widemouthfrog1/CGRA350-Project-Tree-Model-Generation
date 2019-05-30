#include "Circle.h"

Circle::Circle(vec3 center, float radius, vec3 rotation)
{
	this->center = center;
	this->radius = radius;
	this->rotation = rotation;
}

void Circle::addPoint(float angle)
{
	points.push_back(getPoint(angle));
}

void Circle::addPoint(Vertex &point)
{
	points.push_back(point);
}

Vertex Circle::getPoint(float angle)
{
	vec3 point = vec3(sin(angle), 0, cos(angle));
	mat4 rotationMatrix = glm::rotate(mat4(1), rotation.x, vec3(1, 0, 0))*
		glm::rotate(mat4(1), rotation.y, vec3(0, 1, 0))*
		glm::rotate(mat4(1), rotation.z, vec3(0, 0, 1));
	mat4 translateToPosition = glm::translate(mat4(1), center);
	Vertex vertex;
	vertex.pos = translateToPosition * rotationMatrix * vec4(point, 1);
	return vertex;
}

float Circle::getAngle(glm::vec3 point)
{
	//transform point back from rotation
	mat4 rotationMatrix = glm::rotate(mat4(1), -rotation.x, vec3(1, 0, 0))*
		glm::rotate(mat4(1), -rotation.y, vec3(0, 1, 0))*
		glm::rotate(mat4(1), -rotation.z, vec3(0, 0, 1));
	mat4 translateToOrigin = glm::translate(mat4(1), -center);
	vec3 transformedPoint = rotationMatrix * translateToOrigin * vec4(point, 1);
	if (!(transformedPoint.z < 0.001 && transformedPoint.z > -0.001)) {
		return acos(transformedPoint.z);
	}
	else {
		return asin(transformedPoint.x);
	}
	
}

std::vector<Vertex> Circle::createCircle(int startingID)
{
	vector<int> completed;
	std::vector<Vertex> vertices;
	while (completed.size() < points.size()) {
		int smallest;
		for (int i = 0; i < points.size(); i++) {
			bool completedContains;
			for (int j = 0; j < completed.size(); j++) {
				if (completed.at(j) == i) {
					completedContains = true;
				}
			}
			if (!completedContains) {
				if (getAngle(this->points.at(smallest).pos) > getAngle(this->points.at(i).pos)) {
					smallest = i;
				}
			}
		}
		vertices.push_back(points.at(smallest));
		completed.push_back(smallest);
	}
	for (int i = 0; i < vertices.size(); i++) {
		int j = i + 1;
		if (j == vertices.size()) {
			j = 0;
		}
		vertices.at(i).link(vertices.at(j));
	}
	return vertices;
}

std::vector<Vertex> Circle::createFullCircle(int resolution)
{
	vector<Vertex> circle;
	float step = 2 * pi<float>() / resolution;
	int count = 0;
	for (float i = 0; i < 2 * pi<float>(); i += step) {
		Vertex vertex;
		mat4 rotationMatrix = glm::rotate(mat4(1), rotation.x, vec3(1, 0, 0))*
			glm::rotate(mat4(1), rotation.y, vec3(0, 1, 0))*
			glm::rotate(mat4(1), rotation.z, vec3(0, 0, 1));
		mat4 translateToPosition = glm::translate(mat4(1), center);
		vec3 transformedPoint = translateToPosition * rotationMatrix  * vec4(vec3(sin(i), 0, cos(i)), 1);

		vertex.id = count;
		vertex.pos = transformedPoint;
		circle.push_back(vertex);
		count++;
	}
	for (int i = 0; i < count; i++) {
		int j = i + 1;
		if (j == count) {
			j = 0;
		}
		circle.at(i).link(circle.at(j));
	}
	return circle;
}

std::vector<Vertex> Circle::getPoints()
{
	std::vector<Vertex> readOnly;
	for (int i = 0; i < points.size(); i++) {
		readOnly.push_back(points.at(i));
	}
	return readOnly;
}

void Circle::addMidPoint(int point1, int point2)
{
	float angle1 = getAngle(getPoints().at(point1).pos);
	float angle2 = getAngle(getPoints().at(point2).pos);
	if (angle2 > angle1) {
		addPoint((angle2 + angle1) / 2.0f);
	}
	else {
		float angle = (angle2 + 2 * pi<float>() + angle1) / 2.0f;
		if (angle >= 2 * pi<float>()) {
			angle -= 2 * pi<float>();
		}
		addPoint(angle);
	}
}

void Circle::connectPoint(Vertex &vertex, int id)
{
	for (int i = 0; i < points.size(); i++) {
		if (points.at(i).id == id) {
			vertex.link(points.at(i));
			return;
		}
	}
}
