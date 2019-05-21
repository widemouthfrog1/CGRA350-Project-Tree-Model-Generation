#pragma once
#include "Include.h"

using namespace std;
using namespace glm;

struct Spline {
	vector<vec3> controlPoints;
	vector<vec3> interpolatedPoints;
};

Spline spline(const vector<vec3> controlPoints, int divisions);
std::vector<glm::vec3> circle(int numberOfPoints, int radius);
bool pointIsAbove(vec3 branchPoint, vector<vec3> base, int baseIndex);