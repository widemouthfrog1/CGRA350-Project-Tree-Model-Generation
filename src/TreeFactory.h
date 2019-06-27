#pragma once
#include "Include.h"
#include "Variable.h"
#include "Rule.h"
#include "ProjectMath.h"
#include "Circle.h"

using namespace std;
using namespace glm;
using namespace cgra;

class Variable;
class Rule;

class TreeFactory {
	vector<Branch> branches;
public:
	TreeFactory(vector<Branch> branches);
	~TreeFactory();
	gl_mesh createTree();
	void next();
};