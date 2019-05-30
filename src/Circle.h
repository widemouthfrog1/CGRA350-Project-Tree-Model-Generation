#pragma once
#include "Include.h"
#include "Vertex.h"
using namespace std;
using namespace glm;

struct Vertex;

class Circle {
	std::vector<Vertex> points;
public:
	glm::vec3 center;
	glm::vec3 rotation;
	float radius;
	Circle() {};
	Circle(glm::vec3 center, float radius, glm::vec3 rotation);
	void addPoint(float angle);
	void addPoint(Vertex &point);//use with caution
	Vertex getPoint(float angle);
	float getAngle(glm::vec3 point);
	std::vector<Vertex> createCircle(int startingID);
	std::vector<Vertex> createFullCircle(int resolution);
	std::vector<Vertex> getPoints();
	void addMidPoint(int point1, int point2);
	void connectPoint(Vertex &vertex, int id);
};
