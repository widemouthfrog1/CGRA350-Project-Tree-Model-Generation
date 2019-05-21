#include "TreeFactory.h"

TreeFactory::TreeFactory(vector<Variable> alphabet, vector<Rule> rules) {
	this->alphabet = alphabet;
}
TreeFactory::~TreeFactory() {

}

gl_mesh TreeFactory::createTree() {
	for (int i = 0; i < alphabet.size(); i++) {
		vector<vec3> base = alphabet.at(i).getBranch().base;
		vector<vector<vec3>> branches = alphabet.at(i).getBranch().branches;
		//find base points with a branch point above them
		for (int j = 0; j < base.size(); j++) {
			for (int k = 0; k < branches.size(); k++) {
				vector<vec3> branch = branches.at(k);
				for (int l = 0; l < branch.size(); l++) {
					if (pointIsAbove(branch.at(l), base, j)
				}
			}
		}
	}
}

void TreeFactory::next() {

}