#include "ProjectMath.h"

using namespace Math;

float getT(float t, vec3 point1, vec3 point2)
{
	float a = pow((point2.x - point1.x), 2.0f) + pow((point2.y - point1.y), 2.0f) + pow((point2.z - point1.z), 2.0f);
	float b = pow(a, 0.5f);
	float c = pow(b, 0.5);

	return (c + t);
}

Spline Math::spline(const vector<vec3> points, int divisions) {
	vector<vec3> controlPoints;
	vector<vec3> interpolatedPoints;
	vec3 p0 = points.at(0);
	vec3 p1 = points.at(1);
	vec3 p2 = points.at(2);
	vec3 p3 = points.at(3);

	float t0 = 0;
	float t1 = getT(t0, p0, p1);
	float t2 = getT(t1, p1, p2);
	float t3 = getT(t2, p2, p3);
	int ind = 0;
	for (float t = t1; t < t2; t += ((t2 - t1) / divisions))
	{
		vec3 a1 = (t1 - t) / (t1 - t0)*p0 + (t - t0) / (t1 - t0)*p1;
		vec3 a2 = (t2 - t) / (t2 - t1)*p1 + (t - t1) / (t2 - t1)*p2;
		vec3 a3 = (t3 - t) / (t3 - t2)*p2 + (t - t2) / (t3 - t2)*p3;

		vec3 b1 = (t2 - t) / (t2 - t0)*a1 + (t - t0) / (t2 - t0)*a2;
		vec3 b2 = (t3 - t) / (t3 - t1)*a2 + (t - t1) / (t3 - t1)*a3;

		vec3 c = (t2 - t) / (t2 - t1)*b1 + (t - t1) / (t2 - t1)*b2;
		interpolatedPoints.push_back(c);
	}
	for (int i = 0; i < points.size(); i++) {
		controlPoints.push_back(points.at(i));
	}


	Spline spline;
	spline.controlPoints = controlPoints;
	spline.interpolatedPoints = interpolatedPoints;
	return spline;
}

std::vector<float> Math::angleOfBaseAndBranchConnectors(Circle base, Circle branch)
{
	std::vector<float> angles;
	vec3 circle0to1 = branch.center - base.center;
	vec3 normal0 = cross(base.getPoint(0.0f).pos - base.center, base.getPoint(1.0f).pos - base.center);
	vec3 projection0 = projection(circle0to1, normal0);
	vec3 planeProjection0 = normalize(circle0to1 - projection0)*base.radius;
	angles.push_back(base.getAngle(translate(mat4(1), base.center)*vec4(planeProjection0, 1)));

	vec3 normal1 = cross(branch.getPoint(0.0f).pos - branch.center, branch.getPoint(1.0f).pos - branch.center);
	vec3 centerToNormal0 = normal0 - branch.center;
	vec3 projectionOnNormal1 = projection(centerToNormal0, normal1);
	vec3 planeProjection1 = normalize(centerToNormal0 - projectionOnNormal1)*branch.radius;
	angles.push_back(branch.getAngle(translate(mat4(1), branch.center)*vec4(planeProjection1, 1)));
	angles.push_back(branch.getAngle(translate(mat4(1), branch.center)*vec4(planeProjection1, 1)) + pi<float>());
	return angles;
}


vector<vec3> Math::circle(int numberOfPoints, int radius) {
	vector<vec3> circle;
	float step = 2 * pi<float>()/numberOfPoints;
	for (int i = 0; i < 2*pi<float>(); i += step) {
		circle.push_back(vec3(sin(i), 0, cos(i)));
	}
	return circle;
}

/*
Vertex Math::closestToBasePlane(vector<Vertex> base, vector<Vertex> branch) {
	//provides vertical normal due to anti-clockwise circle
	vec3 normal = glm::cross(base.at(1).pos - base.at(0).pos, base.at(2).pos - base.at(0).pos);//vector from A -> B = B-A therefore, cross(0 -> 1, 0 -> 2). Base should have at least 4 vertices
	float minLength = -1;
	Vertex minPoint;
	for (int i = 0; i < branch.size(); i++) {
		float length = glm::length(projection(branch.at(i).pos - base.at(0).pos, normal));
		if (minLength == -1 || minLength > length) {
			minLength = length;
			minPoint = branch.at(i);
		}
	}
	return minPoint;
}

Vertex Math::closestBasePoint(vector<Vertex> base, Vertex closest) {
	float minLength = -1;
	Vertex minPoint;
	for (int i = 0; i < base.size(); i++) {
		float length = glm::length(base.at(i).pos - closest.pos);
		if (minLength == -1 || minLength > length) {
			minLength = length;
			minPoint = base.at(i);
		}
	}
	return minPoint;
}


//circle must have a number of points divisible by 4
int Math::ninetyDegreeRotation(std::vector<Vertex> circle, int point, vec3 direction)
{
	int rotatedPoint;
	int leftNeighbour = getNeighbour(circle.size(), point, true);
	int rightNeighbour = getNeighbour(circle.size(), point, false);
	vec3 leftProjection = projection(circle.at(leftNeighbour).pos, direction);
	vec3 rightProjection = projection(circle.at(rightNeighbour).pos, direction);
	if (acos(dot(direction, leftProjection)) == 1){
		if (acos(dot(direction, rightProjection)) == 1) {
			//find shortest, rotate in that direction
			if (length(leftProjection) < length(rightProjection)) {
				rotatedPoint = point - circle.size() / 4;
			}
			else {
				rotatedPoint = point + circle.size() / 4;
			}
		}
		else {
			//rotate left
			rotatedPoint = point - circle.size() / 4;
		}
	}
	else {
		if (acos(dot(direction, rightProjection)) == -1) {
			//find largest, rotate in that direction
			if (length(leftProjection) > length(rightProjection)) {
				rotatedPoint = point - circle.size() / 4;
			}
			else {
				rotatedPoint = point + circle.size() / 4;
			}
		}
		else {
			//rotate right
			rotatedPoint = point + circle.size() / 4;
		}
	}
	if (rotatedPoint < 0) {
		rotatedPoint = circle.size() + rotatedPoint;
	}
	else if (rotatedPoint >= circle.size()) {
		rotatedPoint = rotatedPoint - circle.size();
	}
	return rotatedPoint;
}

int Math::getNeighbour(int circleSize, int point, bool leftNeighbour)
{
	int neighbour;
	if (leftNeighbour) {
		if (point - 1 == 0) {
			neighbour = circleSize - 1;
		}
		else {
			neighbour = point - 1;
		}
	}
	else {
		if (point + 1 == circleSize) {
			neighbour = 0;
		}
		else {
			neighbour = point + 1;
		}
	}
	return neighbour;
}*/


vec3 Math::projection(vec3 a, vec3 b) {
	return (
		dot(a, b)
		/(length(b)*length(b)
			)
		)*b;
}


