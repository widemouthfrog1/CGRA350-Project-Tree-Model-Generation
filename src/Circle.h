#pragma once
#include "Include.h"
#include "Vertex.h"
using namespace std;
using namespace glm;

struct Vertex;

class Circle {
	
public:
	std::vector<Vertex> points;
	glm::vec3 center;
	glm::vec3 rotation;
	glm::mat4 matrixRotation = mat4(1);
	float radius;
	Circle() {};
	Circle(glm::vec3 center, float radius, int resolution, glm::mat4 rotation);
	Circle(glm::vec3 center, float radius, glm::vec3 rotation);
	void increaseResolution(int point1, int point2, int resolution);
	void addPoint(float angle);
	void addPoint(Vertex &point);//use with caution
	Vertex getPoint(float angle);
	float getAngle(glm::vec3 point);
	std::vector<Vertex> createCircle(int startingID);
	std::vector<Vertex> createFullCircle(int resolution);
	std::vector<Vertex> getPoints();
	std::vector<Vertex> getMidPoints(int point1, int point2);
	void addMidPoint(int point1, int point2);
	void connectPoint(Vertex &vertex, int id);
	Vertex getClosestPoint(glm::vec3 point);
	Vertex getBranchPoint(Circle base/*, glm::vec3 closestBasePoint*/);
	glm::vec3 normal();
};
