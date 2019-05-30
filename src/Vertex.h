#pragma once
#include "Include.h"

struct Vertex {

	int id;
	int branch;
	glm::vec3 pos;
	void link(Vertex &vertex);
	int getConnection(int index);
	int getIndex(int id);
	bool useConnection(Vertex &vertex);
	void resetConnections();
	int connectionsSize();
private:
	std::vector<int> connections;
	std::vector<bool> used;
};