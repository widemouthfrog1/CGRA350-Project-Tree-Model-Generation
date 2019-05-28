#include "TreeFactory.h"

TreeFactory::TreeFactory(vector<Variable> alphabet, vector<Rule> rules) {
	this->alphabet = alphabet;
}
TreeFactory::~TreeFactory() {

}

gl_mesh TreeFactory::createTree() {
	int id = 0;
	vector<mesh_vertex> vertices;
	vector<unsigned int> indices;
	for (int i = 0; i < alphabet.size(); i++) {
		//-----Give points a position in the index vector-----\\

		vector<vec3> base = alphabet.at(i).getBranch().base;
		vector<Math::Vertex> base_vertices;
		for (int j = 0; j < base.size(); j++) {
			Math::Vertex vertex = Math::Vertex();
			vertex.pos = base.at(j);
			vertex.id = id++;
			base_vertices.push_back(vertex);
		}

		vector<vector<vec3>> branches = alphabet.at(i).getBranch().branches;
		vector<vector<Math::Vertex>> branch_vertices_vector;
		for (int j = 0; j < branches.size(); j++) {
			vector<Math::Vertex> branch_vertices;
			vector<vec3> branch = branches.at(j);
			for (int k = 0; k < branch.size(); k++) {
				Math::Vertex vertex = Math::Vertex();
				vertex.pos = branch.at(k);
				vertex.id = id++;
				branch_vertices.push_back(vertex);
			}
			branch_vertices_vector.push_back(branch_vertices);
		}
		//----------------------------------------------------\\

		//-----Link bottom of each branch (circle) to the closest point on the base-----\\

		for (int j = 0; j < branch_vertices_vector.size(); j++) {
			vector<Math::Vertex> branch = branch_vertices_vector.at(j);
			Math::Vertex closest = Math::closestToBasePlane(base_vertices, branch);
			Math::Vertex closestBase = Math::closestBasePoint(base_vertices, closest);
			closestBase.link.id = closest.id;
			closest.link.id = closestBase.id;
		}
		//----------------------------------------------------\\
		
		//-----Find midpoint(s) of links on base-----\\

		for (int j = 0; j < base_vertices.size(); j++) {
			Math::Vertex vertex = base_vertices.at(j);
			if (vertex.link.id != -1) {//if it has a link
				int	k = j + 1;
				if (k >= base_vertices.size()) {
					k = 0;
				}
				Math::Vertex neighbour = base_vertices.at(k);
				while (neighbour.link.id == -1) {
					k++;
					if (k >= base_vertices.size()) {//this is fine because k should never loop twice
						k = 0;
					}
					neighbour = base_vertices.at(k);
				}
				vector<int> midPoints = Math::midPoint(base_vertices.size(), j, k);
				int leftNeighbour = Math::getNeighbour(base_vertices.size(), midPoints.at(0), true);
				int rightNeighbour = Math::getNeighbour(base_vertices.size(), midPoints.at(0), false);
			}
		}
	}


	mesh_builder builder;
	builder.vertices = vertices;
	builder.indices = indices;
	builder.mode = GL_LINES; //for now
	return builder.build();
}

void TreeFactory::next() {

}