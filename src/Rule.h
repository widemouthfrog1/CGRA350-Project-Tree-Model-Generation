#pragma once
#include "Include.h"
#include "Letter.h"

using namespace std;

class Letter;

struct Rule {
	std::string input;
	std::string output;

	Rule(std::string input,std::string output);
	~Rule();

	std::string applyRule(std::string command);
};