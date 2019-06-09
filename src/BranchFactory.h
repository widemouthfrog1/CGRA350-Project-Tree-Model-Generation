#pragma once

#include "Include.h"
#include "Variable.h"
#include "Rule.h"
#include "ProjectMath.h"
#include "Circle.h"
#include "Branch.h"

class BranchFactory {
	BranchFactory() {}
	BranchFactory(std::vector<float> anglesToBranches, std::vector<float> minAngleVariences, std::vector<float> maxAngleVariences, );
	Branch createBranch(Circle baseCircle, long seed);
};