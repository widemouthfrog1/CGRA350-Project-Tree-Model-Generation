#pragma once
#include "Letter.h"
#include "Branch.h"
class Letter;
class Branch;

class Variable : public Letter {
	char letter;
	Branch branch;
public:
	Variable(char letter, Branch branch);
	~Variable();
	char getLetter();
	Branch getBranch();

};