#pragma once
#include "Include.h"
#include "Letter.h"

using namespace std;

class Letter;

class Rule {
	Letter input;
	vector<Letter> output;

	Rule(Letter input, vector<Letter> output);
	~Rule();

	vector<Letter> checkRule(vector<Letter> tree);
};