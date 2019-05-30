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

void Circle::addPoint(Math::Vertex point)//fix with pointers
{
	points.push_back(point);
}

Math::Vertex Circle::getPoint(float angle)
{
	vec3 point = vec3(sin(angle), 0, cos(angle));
	mat4 rotationMatrix = glm::rotate(mat4(1), rotation.x, vec3(1, 0, 0))*
		glm::rotate(mat4(1), rotation.y, vec3(0, 1, 0))*
		glm::rotate(mat4(1), rotation.z, vec3(0, 0, 1));
	mat4 translateToOrigin = glm::translate(mat4(1), -center);
	mat4 translateBack = glm::translate(mat4(1), center);
	Math::Vertex vertex;
	vertex.pos = translateBack * rotationMatrix * translateToOrigin * vec4(point, 1);
	return vertex;
}

float Circle::getAngle(glm::vec3 point)
{
	//transform point back from rotation
	mat4 rotationMatrix = glm::rotate(mat4(1), -rotation.x, vec3(1, 0, 0))*
		glm::rotate(mat4(1), -rotation.y, vec3(0, 1, 0))*
		glm::rotate(mat4(1), -rotation.z, vec3(0, 0, 1));
	mat4 translateToOrigin = glm::translate(mat4(1), -center);
	mat4 translateBack = glm::translate(mat4(1), center);
	vec3 transformedPoint = translateBack * rotationMatrix * translateToOrigin * vec4(point, 1);
	return acos(transformedPoint.z);
}

std::vector<Math::Vertex> Circle::createCircle(int startingID)
{
	vector<int> completed;
	std::vector<Math::Vertex> vertices;
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
		vertices.at(i).connections.push_back(j);
	}
	return vertices;
}

std::vector<Math::Vertex> Circle::getPoints()
{
	std::vector<Math::Vertex> readOnly;
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

void Circle::connectPoint(Math::Vertex vertex, int id)//fix with pointers
{
	for (int i = 0; i < points.size(); i++) {
		if (points.at(i).id == id) {
			Math::link(vertex, points.at(i));
			return;
		}
	}
}
