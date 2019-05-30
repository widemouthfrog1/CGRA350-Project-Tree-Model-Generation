#pragma once
#include "Include.h"
#include "ProjectMath.h"
using namespace std;
using namespace glm;

class Circle {
	std::vector<Math::Vertex> points;
public:
	glm::vec3 center;
	glm::vec3 rotation;
	float radius;
	Circle() {};
	Circle(glm::vec3 center, float radius, glm::vec3 rotation);
	void addPoint(float angle);
	void addPoint(Math::Vertex point);//use with caution
	Math::Vertex getPoint(float angle);
	float getAngle(glm::vec3 point);
	std::vector<Math::Vertex> createCircle(int startingID);
	std::vector<Math::Vertex> getPoints();
	void addMidPoint(int point1, int point2);
	void connectPoint(Math::Vertex vertex, int id);
};
