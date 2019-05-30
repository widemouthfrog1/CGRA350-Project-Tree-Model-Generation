#pragma once
#include "Include.h"

using namespace std;
using namespace glm;

class Circle;

class Branch {
public:
	Circle base;
	std::vector<Circle> branches;
	Branch() {};
	Branch(Circle base, std::vector<Circle> branches);
	~Branch();
};
