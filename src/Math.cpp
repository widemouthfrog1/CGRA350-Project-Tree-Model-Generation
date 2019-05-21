#include "Math.h"

float getT(float t, vec3 point1, vec3 point2)
{
	float a = pow((point2.x - point1.x), 2.0f) + pow((point2.y - point1.y), 2.0f) + pow((point2.z - point1.z), 2.0f);
	float b = pow(a, 0.5f);
	float c = pow(b, 0.5);

	return (c + t);
}

Spline spline(const vector<vec3> points, int divisions) {
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

vector<vec3> circle(int numberOfPoints, int radius) {
	vector<vec3> circle;
	float step = 2 * pi<float>()/numberOfPoints;
	for (int i = 0; i < 2*pi<float>(); i += step) {
		circle.push_back(vec3(sin(i), 0, cos(i)));
	}
	return circle;
}

bool pointIsAbove(vec3 branchPoint, vector<vec3> base, int baseIndex) {
	int leftNeighbour = j - 1;
	int rightNeighbour = j + 1;
	if (leftNeighbour < 0) {
		leftNeighbour = base.size() - 1;
	}
	if (rightNeighbour == base.size()) {
		rightNeighbour == 0;
	}
	vec3 left = base.at(leftNeighbour);
	vec3 right = base.at(rightNeighbour);
	int rotation = base.size() / 4; //base.size must always be divisible by 4
	int hleftIndex = baseIndex - rotation;
	int hrightIndex = baseIndex + rotation;
	if (hleftIndex < 0) {
		hleftIndex = base.size() + hleftIndex;
	}
	if (hrightIndex >= base.size()) {
		hleftIndex = hleftIndex - base.size();
	}

	vec3 halfVector = base.at(hleftIndex) - base.at(hrightIndex);//defines a line intersecting the base through the middle as far from the point at baseIndex as possible

}

