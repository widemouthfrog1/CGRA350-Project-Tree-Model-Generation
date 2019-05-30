#include "Branch.h"

Branch::Branch(Circle base, vector<Circle> branches) {
	this->base = base;
	this->branches = branches;
}

Branch::~Branch() {

}