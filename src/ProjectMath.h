#pragma once
#include "Include.h"

using namespace std;
using namespace glm;

namespace Math {
	struct Spline {
		std::vector<glm::vec3> controlPoints;
		std::vector<glm::vec3> interpolatedPoints;
	};
	struct Vertex {
		int id;
		glm::vec3 pos;
		std::vector<int> connections;
		Link link;
	};
	struct Link {
		int id = -1;
		bool linked = false;//Has the link already been joined by adding it to the index array
	};

	Spline spline(const std::vector<glm::vec3> controlPoints, int divisions);
	std::vector<glm::vec3> circle(int numberOfPoints, int radius);
	Vertex closestToBasePlane(std::vector<Vertex> base, std::vector<Vertex> branch);
	Vertex closestBasePoint(std::vector<Vertex> base, Vertex closest);
	std::vector<int> midPoint(int circleSize, int point1, int point2);
	int getNeighbour(int circleSize, int point, bool leftNeighbour);
	vec3 projection(vec3 a, vec3 b);
}