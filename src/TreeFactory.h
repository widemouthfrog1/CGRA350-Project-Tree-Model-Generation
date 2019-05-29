#pragma once
#include "Include.h"
#include "Variable.h"
#include "Rule.h"

using namespace std;
using namespace glm;
using namespace cgra;

class Variable;
class Rule;

class TreeFactory {
	vector<Variable> alphabet;
public:
	TreeFactory(vector<Variable> alphabet, vector<Rule> rules, Variable start);
	~TreeFactory();
	gl_mesh createTree();
	void next();
};