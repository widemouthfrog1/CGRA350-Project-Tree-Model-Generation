#include "TreeFactory.h"

TreeFactory::TreeFactory(vector<Variable> alphabet, vector<Rule> rules, Variable start) {
	this->alphabet = alphabet;
}
TreeFactory::~TreeFactory() {

}

gl_mesh TreeFactory::createTree() {
	int id = 0;
	vector<mesh_vertex> vertices;
	vector<unsigned int> indices;
	vector<Math::Vertex> myVertices;
	for (int i = 0; i < alphabet.size(); i++) {
		//-----Give points a position in the index vector-----\\

		Math::Circle base = alphabet.at(i).getBranch().base;
		vector<Math::Vertex> base_vertices;
		for (int j = 0; j < base.points.size(); j++) {
			Math::Vertex vertex = Math::Vertex();
			vertex.pos = base.points.at(j);
			vertex.id = id++;
			myVertices.push_back(vertex);
			base_vertices.push_back(vertex);
		}

		vector<Math::Circle> branches = alphabet.at(i).getBranch().branches;
		vector<vector<Math::Vertex>> branch_vertices_vector;
		for (int j = 0; j < branches.size(); j++) {
			vector<Math::Vertex> branch_vertices;
			Math::Circle branch = branches.at(j);
			for (int k = 0; k < branch.points.size(); k++) {
				Math::Vertex vertex = Math::Vertex();
				vertex.pos = branch.points.at(k);
				vertex.id = id++;
				vertex.branch = j;
				myVertices.push_back(vertex);
				branch_vertices.push_back(vertex);
			}
			branch_vertices_vector.push_back(branch_vertices);
		}
		//----------------------------------------------------\\

		//-----Link bottom of each branch (circle) to the closest point on the base-----\\

		for (int j = 0; j < branch_vertices_vector.size(); j++) {
			vector<Math::Vertex> branch = branch_vertices_vector.at(j);
			//this is likely making a copy and I will need to replace it with pointers
			Math::Vertex closest = Math::closestToBasePlane(base_vertices, branch);
			Math::Vertex closestBase = Math::closestBasePoint(base_vertices, closest);
			closestBase.connections.push_back(closest.id);
			closest.connections.push_back(closestBase.id);
		}
		//----------------------------------------------------\\
		
		//-----Find midpoint(s) of links on base-----\\

		for (int j = 0; j < base_vertices.size(); j++) {
			Math::Vertex vertex = base_vertices.at(j);
			if (vertex.connections.size() != 0) {//if it has a link
				int	k = j + 1;
				if (k >= base_vertices.size()) {
					k = 0;
				}
				Math::Vertex neighbour = base_vertices.at(k);
				while (neighbour.connections.size() == 0) {
					k++;
					if (k >= base_vertices.size()) {//this is fine because k should never loop twice
						k = 0;
					}
					neighbour = base_vertices.at(k);
				}
				vector<int> midPoints = Math::midPoint(base_vertices.size(), j, k);
				vec3 middleVector = base.points.at(midPoints.at(0)) - base.center;//position relative to the centre of the circle
				Math::Vertex vertexBranch = branch_vertices_vector.at(vertex.branch).at(Math::ninetyDegreeRotation(branch_vertices_vector.at(vertex.branch), vertex.connections.at(0), middleVector));
				vec3 vertexBranchPos = vertexBranch.pos;
				Math::Vertex neighbourBranch = branch_vertices_vector.at(neighbour.branch).at(Math::ninetyDegreeRotation(branch_vertices_vector.at(neighbour.branch), neighbour.connections.at(0), middleVector));
				vec3 neighbourBranchPos = neighbourBranch.pos;
				vec3 average = (vertex.pos + vertexBranchPos + neighbourBranchPos)/3.0f;

				Math::Vertex averageVertex;
				averageVertex.id = id++;
				averageVertex.connections.push_back(vertex.id);
				averageVertex.connections.push_back(vertexBranch.id);
				averageVertex.connections.push_back(neighbourBranch.id);
				myVertices.push_back(averageVertex);

				vertex.connections.push_back(averageVertex.id);
				vertexBranch.connections.push_back(averageVertex.id);
				neighbourBranch.connections.push_back(averageVertex.id);

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