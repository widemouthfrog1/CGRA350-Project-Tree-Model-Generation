#pragma once
#include "Include.h"
#include "Circle.h"

using namespace std;
using namespace glm;

class Circle;

class Branch {
public:
	Circle base;
	std::vector<Branch> branches;
	Branch() {};
	Branch(Circle base);
	~Branch();
	void addBranch(Branch branch);
};

