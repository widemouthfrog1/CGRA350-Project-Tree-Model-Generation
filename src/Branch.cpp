#include "Branch.h"

Branch::Branch(vector<vec3> base, vector<vector<vec3>> branches) {
	this->base = base;
	this->branches = branches;
}

Branch::~Branch() {

}