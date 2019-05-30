#include "Vertex.h"
void Vertex::link(Vertex &vertex)
{
	connections.push_back(vertex.id);
	used.push_back(false);
	vertex.connections.push_back(id);
	vertex.used.push_back(false);
}

int Vertex::getConnection(int index)
{
	return connections.at(index);
}

int Vertex::getIndex(int id)
{
	for (int i = 0; i < connections.size(); i++) {
		if (connections.at(i) == id) {
			return i;
		}
	}
	return -1;
}

bool Vertex::useConnection(Vertex &vertex)
{
	if (used.at(getIndex(vertex.id)) == true) {
		return false;
	}
	used.at(getIndex(vertex.id)) = true;
	vertex.used.at(vertex.getIndex(this->id)) = true;
	return true;
}

void Vertex::resetConnections()
{
	for (int i = 0; i < used.size(); i++) {
		used.at(i) = false;
	}
}

int Vertex::connectionsSize()
{
	return connections.size();
}
