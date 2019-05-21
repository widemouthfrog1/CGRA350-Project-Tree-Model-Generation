#include "Variable.h"

Variable::Variable(char letter, Branch branch) {
	this->letter = letter;
	this->branch = branch;
}
Variable::~Variable() {

}
char Variable::getLetter() {
	return letter;
}

Branch Variable::getBranch() {
	return this->branch;
}