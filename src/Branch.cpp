#include "Branch.h"

Branch::Branch(Circle base) {
	this->base = base;
}

Branch::~Branch() {

}

void Branch::addBranch(Branch branch)
{
	branches.push_back(branch);
}
