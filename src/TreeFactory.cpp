#include "TreeFactory.h"

TreeFactory::TreeFactory(vector<Variable> alphabet, vector<Rule> rules) {
	this->alphabet = alphabet;
}
TreeFactory::~TreeFactory() {

}

gl_mesh TreeFactory::createTree() {
	int id = 0;
	vector<Link> links;
	vector<mesh_vertex> vertices;
	vector<unsigned int> indices;
	for (int i = 0; i < alphabet.size(); i++) {
		//-----Give points a position in the index vector-----\\

		vector<vec3> base = alphabet.at(i).getBranch().base;
		vector<Vertex> base_vertices;
		for (int j = 0; j < base.size(); j++) {
			Vertex vertex = Vertex();
			vertex.pos = base.at(j);
			vertex.id = id++;
			base_vertices.push_back(vertex);
		}

		vector<vector<vec3>> branches = alphabet.at(i).getBranch().branches;
		vector<vector<Vertex>> branch_vertices_vector;
		for (int j = 0; j < branches.size(); j++) {
			vector<Vertex> branch_vertices;
			vector<vec3> branch = branches.at(j);
			for (int k = 0; k < branch.size(); k++) {
				Vertex vertex = Vertex();
				vertex.pos = branch.at(k);
				vertex.id = id++;
				branch_vertices.push_back(vertex);
			}
			branch_vertices_vector.push_back(branch_vertices);
		}
		//----------------------------------------------------\\

		//-----Link bottom of each branch (circle) to the closest point on the base-----\\

		for (int j = 0; j < branch_vertices_vector.size(); j++) {
			vector<Vertex> branch = branch_vertices_vector.at(j);
			Vertex closest = closestToBasePlane(base_vertices, branch);
			Vertex closestBase = closestBasePoint(base_vertices, closest);
			Link link = Link();
			link.id1 = closest.id;
			link.id2 = closestBase.id;
			links.push_back(link);
		}
		//----------------------------------------------------\\
		

	}


	mesh_builder builder;
	builder.vertices = vertices;
	builder.indices = indices;
	builder.mode = GL_LINES; //for now
	return builder.build();
}

void TreeFactory::next() {

}