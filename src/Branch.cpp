#include "Branch.h"

Branch::Branch(Math::Circle base, vector<Math::Circle> branches) {
	this->base = base;
	this->branches = branches;
}

Branch::~Branch() {

}