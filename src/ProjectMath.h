#pragma once
#include "Include.h"
#include "Circle.h"

using namespace std;
using namespace glm;

class Circle;

namespace Math {
	struct Spline {
		std::vector<glm::vec3> controlPoints;
		std::vector<glm::vec3> interpolatedPoints;
	};

	Spline spline(const std::vector<glm::vec3> controlPoints, int divisions);

	std::vector<float> angleOfClosestPointsOnTwoCircles(Circle circle0, Circle circle1);

	std::vector<glm::vec3> circle(int numberOfPoints, int radius);
	/*Vertex closestToBasePlane(std::vector<Vertex> base, std::vector<Vertex> branch);
	Vertex closestBasePoint(std::vector<Vertex> base, Vertex closest);
	int ninetyDegreeRotation(std::vector<Vertex> circle, int point, glm::vec3 direction);
	int getNeighbour(int circleSize, int point, bool leftNeighbour);*/
	vec3 projection(vec3 a, vec3 b);
}