#pragma once
#include "Include.h"

using namespace std;
using namespace glm;

class Branch {

	

public:
	vector<vec3> base;
	vector<vector<vec3>> branches;
	Branch() {};
	Branch(vector<vec3> base, vector<vector<vec3>> branches);
	~Branch();
};
