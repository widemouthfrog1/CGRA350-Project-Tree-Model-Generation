#pragma once
#include "Include.h"

using namespace std;
using namespace glm;

struct Spline {
	std::vector<glm::vec3> controlPoints;
	std::vector<glm::vec3> interpolatedPoints;
};
struct Vertex {
	int id;
	glm::vec3 pos;
};
struct Link {
	int id1;
	int id2;
};

Spline spline(const std::vector<glm::vec3> controlPoints, int divisions);
std::vector<glm::vec3> circle(int numberOfPoints, int radius);
Vertex closestToBasePlane(std::vector<Vertex> base, std::vector<Vertex> branch);
Vertex closestBasePoint(std::vector<Vertex> base, Vertex closest);