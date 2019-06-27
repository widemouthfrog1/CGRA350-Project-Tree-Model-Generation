#include "Circle.h"
#include "ProjectMath.h"

Circle::Circle(glm::vec3 center, float radius, int resolution, glm::mat4 rotation)
{
	this->center = center;
	this->radius = radius;
	this->rotation = vec3(0, 0, 0);
	this->matrixRotation = rotation;
	for (int i = 0; i < resolution*4; i++) {//this resolution is multiplied by 4 to ensure the resolution is divisible by 4
		addPoint((i * 2 * pi<float>())/(resolution *4.0f));
	}
}

Circle::Circle(vec3 center, float radius, vec3 rotation)
{
	this->center = center;
	this->radius = radius;
	this->rotation = rotation;
}

void Circle::increaseResolution(int point1, int point2, int resolution)
{
	float angle1 = getAngle(this->points.at(point1).pos);
	float angle2 = getAngle(this->points.at(point2).pos);
	float step = (angle2 - angle1) / (resolution + 1);
	for (float i = angle1 + step; i < angle2; i += step) {
		this->addPoint(i);
	}
}

void Circle::addPoint(float angle)
{
	points.push_back(getPoint(angle));
}

void Circle::addPoint(Vertex &point)
{
	points.push_back(point);
}

Vertex Circle::getPoint(float angle)
{
	vec3 point = vec3(radius*sin(angle), 0, radius*cos(angle));
	mat4 rotationMatrix = glm::rotate(mat4(1), rotation.x, vec3(1, 0, 0))*
		glm::rotate(mat4(1), rotation.y, vec3(0, 1, 0))*
		glm::rotate(mat4(1), rotation.z, vec3(0, 0, 1));
	mat4 translateToPosition = glm::translate(mat4(1), center);
	Vertex vertex;
	vertex.pos = translateToPosition * rotationMatrix * this->matrixRotation * vec4(point, 1);
	return vertex;
}

float Circle::getAngle(glm::vec3 point)
{
	//transform point back from rotation
	mat4 rotationMatrix = glm::rotate(mat4(1), -rotation.x, vec3(1, 0, 0))*
		glm::rotate(mat4(1), -rotation.y, vec3(0, 1, 0))*
		glm::rotate(mat4(1), -rotation.z, vec3(0, 0, 1));
	mat4 translateToOrigin = glm::translate(mat4(1), -center);
	vec3 transformedPoint = glm::inverse(this->matrixRotation)*rotationMatrix * translateToOrigin * vec4(point, 1);
	if (transformedPoint.z > 0 ) {
		if (transformedPoint.x > 0) {
			return acos(transformedPoint.z / radius);
		}
		else {
			return -acos(transformedPoint.z / radius);
		}
	}
	else {
		if (transformedPoint.x > 0) {
			return acos(transformedPoint.z / radius);
		}
		else {
			return -acos(transformedPoint.z / radius);
		}
	}
}

std::vector<Vertex> Circle::createCircle(int startingID)
{
	vector<int> completed;
	std::vector<Vertex> vertices;
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
		vertices.at(i).link(vertices.at(j));
	}
	return vertices;
}

std::vector<Vertex> Circle::createFullCircle(int resolution)
{
	vector<Vertex> circle;
	float step = 2 * pi<float>() / resolution;
	int count = 0;
	for (float i = 0; i < 2 * pi<float>(); i += step) {
		Vertex vertex;
		mat4 rotationMatrix = glm::rotate(mat4(1), rotation.x, vec3(1, 0, 0))*
			glm::rotate(mat4(1), rotation.y, vec3(0, 1, 0))*
			glm::rotate(mat4(1), rotation.z, vec3(0, 0, 1));
		mat4 translateToPosition = glm::translate(mat4(1), center);
		vec3 transformedPoint = translateToPosition * rotationMatrix  * vec4(vec3(radius*sin(i), 0, radius*cos(i)), 1);

		vertex.id = count;
		vertex.pos = transformedPoint;
		circle.push_back(vertex);
		count++;
	}
	for (int i = 0; i < count; i++) {
		int j = i + 1;
		if (j == count) {
			j = 0;
		}
		circle.at(i).link(circle.at(j));
	}
	return circle;
}

std::vector<Vertex> Circle::getPoints()
{
	std::vector<Vertex> readOnly;
	for (int i = 0; i < points.size(); i++) {
		readOnly.push_back(points.at(i));
	}
	return readOnly;
}

std::vector<Vertex> Circle::getMidPoints(int point1, int point2)
{
	std::vector<Vertex> midPoints;
	float angle1 = getAngle(getPoints().at(point1).pos);
	float angle2 = getAngle(getPoints().at(point2).pos);
	float angle;
	if (angle2 > angle1) {
		angle = (angle2 + angle1) / 2.0f;
	}
	else {
		float angle = (angle2 + 2 * pi<float>() + angle1) / 2.0f;
		if (angle >= 2 * pi<float>()) {
			angle -= 2 * pi<float>();
		}
	}
	float pointAngle;
	int angleCheck = 0;
	for (int i = 0; i < points.size(); i++) {
		pointAngle = getAngle(getPoints().at(i).pos);
		if (pointAngle - angle < 0.0001 && pointAngle - angle > -0.0001) {
			midPoints.push_back(getPoints().at(i));
			break;
		}
		if (pointAngle < angle) {
			if (angleCheck == 0) {
				angleCheck = -1;
			}
			else if (pointAngle > angle) {
				midPoints.push_back(getPoints().at(i));
				midPoints.push_back(getPoints().at(i-1));
				break;
			}
		}
		else {
			if (angleCheck == 0) {
				angleCheck = 1;
			}
			else if (pointAngle < angle) {
				midPoints.push_back(getPoints().at(i));
				midPoints.push_back(getPoints().at(i - 1));
				break;
			}
		}
	}

	return midPoints;
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

void Circle::connectPoint(Vertex &vertex, int id)
{
	for (int i = 0; i < points.size(); i++) {
		if (points.at(i).id == id) {
			vertex.link(points.at(i));
			return;
		}
	}
}

Vertex Circle::getClosestPoint(vec3 point)
{
	Vertex closestPoint = this->points.at(0);
	float smallestDistance = glm::length(closestPoint.pos - point);
	
	for (int i = 1; i < this->points.size(); i++) {
		Vertex newPoint = points.at(i);
		float distance = length(newPoint.pos - point);
		if (distance < smallestDistance) {
			closestPoint = newPoint;
			smallestDistance = distance;
		}
	}
	//general solution
	/*
	vec3 normal = this->normal();
	vec3 position = point - center;
	vec3 projOnNorm = Math::projection(position, normal);
	vec3 closestPoint = normalize(position - projOnNorm)*radius;
	Vertex vertex;
	vertex.pos = closestPoint;
	*/
	return closestPoint;
}

Vertex Circle::getBranchPoint(Circle base/*, glm::vec3 closestBasePoint*/)
{
	vec3 baseNormal = base.normal();
	float smallestLength = -1;
	int index;
	for (int i = 0; i < this->points.size(); i++) {
		vec3 relativePosition = this->points.at(i).pos - base.center;
		vec3 projection = Math::projection(relativePosition, baseNormal);
		float length = glm::length(projection);
		if (length < smallestLength || smallestLength == -1) {
			index = i;
			smallestLength = length;
		}
	}
	return this->points.at(index);
	/*
	Vertex branchPoint = getClosestPoint(closestBasePoint);
	vec3 normal = this->normal();
	vec3 baseNormal = base.normal();
	vec3 planeNormal = cross(baseNormal, closestBasePoint);
	vec3 normalOnPlaneNormal = Math::projection(normal, planeNormal);
	vec3 normalOnPlane;
	if (normalOnPlaneNormal.x < 0.0001 && normalOnPlaneNormal.x > -0.0001 && normalOnPlaneNormal.y < 0.0001 && normalOnPlaneNormal.y > -0.0001 && normalOnPlaneNormal.z < 0.0001 && normalOnPlaneNormal.z > -0.0001) {
		return branchPoint; //normal is perpendicular to baseNormal
	}
	else {
		normalOnPlane = normalOnPlaneNormal - normal;
	}
	vec3 normalOnClosest = Math::projection(normal, closestBasePoint);
	float angle = acos(dot(normalOnPlane, normal));
	if (angle == 0 || angle == pi<float>() || length(normalOnClosest + closestBasePoint) > length(closestBasePoint)) {
		//return far point
		return getPoint(getAngle(branchPoint.pos) + pi<float>());
	}
	else {
		//return close point
		return branchPoint;
	}
	*/
	
}



glm::vec3 Circle::normal()
{
	return cross(getPoint(0).pos - center, getPoint(1).pos - center);
}
