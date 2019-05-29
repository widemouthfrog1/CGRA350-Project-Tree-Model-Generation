#pragma once
#include "Include.h"

using namespace std;
using namespace glm;


class Branch {
public:
	Math::Circle base;
	vector<Math::Circle> branches;
	Branch() {};
	Branch(Math::Circle base, vector<Math::Circle> branches);
	~Branch();
};
