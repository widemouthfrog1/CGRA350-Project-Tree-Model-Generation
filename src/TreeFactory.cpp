#include "TreeFactory.h"

TreeFactory::TreeFactory(vector<Variable> alphabet, vector<Rule> rules, Variable start) {
	this->alphabet = alphabet;
	this->state.push_back(start);
}
TreeFactory::~TreeFactory() {

}

//creates a tree mesh from the current state
gl_mesh TreeFactory::createTree() {
	int id = 0;
	vector<mesh_vertex> vertices;
	vector<unsigned int> indices;
	vector<Vertex> myVertices;
	for (int i = 0; i < state.size(); i++) {
		//-----Give points a position in the index vector-----\\

		Circle base = state.at(i).getBranch().base;
		//vector<Vertex> base_vertices;
		/*for (int j = 0; j < base.points.size(); j++) {
			Vertex vertex = Vertex();
			vertex.pos = base.points.at(j);
			vertex.id = id++;
			myVertices.push_back(vertex);
			base_vertices.push_back(vertex);
		}*/

		vector<Circle> branches = state.at(i).getBranch().branches;
		//vector<vector<Vertex>> branch_vertices_vector;
		/*for (int j = 0; j < branches.size(); j++) {
			vector<Vertex> branch_vertices;
			Math::Circle branch = branches.at(j);
			for (int k = 0; k < branch.points.size(); k++) {
				Vertex vertex = Vertex();
				vertex.pos = branch.points.at(k);
				vertex.id = id++;
				vertex.branch = j;
				myVertices.push_back(vertex);
				branch_vertices.push_back(vertex);
			}
			branch_vertices_vector.push_back(branch_vertices);
		}*/
		//----------------------------------------------------\\

		//-----Link bottom of each branch (circle) to the closest point on the base-----\\

		/*for (int j = 0; j < branch_vertices_vector.size(); j++) {
			vector<Vertex> branch = branch_vertices_vector.at(j);
			//this is likely making a copy and I will need to replace it with pointers
			Vertex closest = Math::closestToBasePlane(base_vertices, branch);
			Vertex closestBase = Math::closestBasePoint(base_vertices, closest);
			closestBase.connections.push_back(closest.id);
			closest.connections.push_back(closestBase.id);
		}*/

		for (int j = 0; j < branches.size(); j++) {
			Vertex basePoint = base.getClosestPoint(branches.at(j).center);
			basePoint.pos += base.center;
			basePoint.id = id++;
			Vertex branchPoint = branches.at(j).getBranchPoint(base, basePoint.pos);
			branchPoint.pos += branches.at(j).center;
			branchPoint.id = id++;
			branchPoint.branch = j;
			basePoint.link(branchPoint);
			base.addPoint(basePoint);
			branches.at(j).addPoint(branchPoint);

			myVertices.push_back(basePoint);
			myVertices.push_back(branchPoint);
		}
		//----------------------------------------------------\\
		
		//-----Find midpoint(s) of links on base-----\\
		
		vector<Vertex> basePoints = base.getPoints();
		for (int j = 0; j < basePoints.size(); j++) {
			Vertex vertex = basePoints.at(j);
				
			int k = j + 1;
			if (k == basePoints.size()) {
				k = 0;
			}
			Vertex neighbour = base.getPoints().at(k);
			base.addMidPoint(j, k);
			vector<Vertex> newBasePoints = base.getPoints();
			vec3 middleVector = newBasePoints.at(newBasePoints.size()-1).pos - base.center;//position relative to the centre of the circle
			Circle branch = branches.at(myVertices.at(vertex.getConnection(0)).branch);
			float startAngle = branch.getAngle(branch.getPoints().at(0).pos);
			if (startAngle < 0) {
				startAngle += 2 * pi<float>();
			}
			float plusPiOver2 = startAngle + pi<float>() / 2;
			if (plusPiOver2 >= 2 * pi<float>()) {
				plusPiOver2 -= 2 * pi<float>();
			}
			float minusPiOver2 = startAngle - pi<float>() / 2;
			if (minusPiOver2 < 0) {
				minusPiOver2 += 2 * pi<float>();
			}
			Vertex vertexBranchPoint;
			Vertex bp1 = branch.getPoint(minusPiOver2);
			Vertex bp2 = branch.getPoint(plusPiOver2);
			vec3 rbp1 = bp1.pos - branch.center;
			vec3 rbp2 = bp2.pos - branch.center;
			vec3 middlePoint = middleVector + base.center;
			if (length(Math::projection(rbp1,middleVector) + middleVector) > length(middleVector)) {
				if (length(Math::projection(rbp2, middleVector) + middleVector) > length(middleVector)) {
					if (length(Math::projection(rbp1, middleVector)) > length(Math::projection(rbp2, middleVector))) {
						vertexBranchPoint = bp1;
					}
					else {
						vertexBranchPoint = bp2;
					}
				}
				else {
					vertexBranchPoint = bp1;
				}
			}
			else {
				if (length(Math::projection(rbp2, middleVector) + middleVector) > length(middleVector)) {
					vertexBranchPoint = bp2;
				}
				else {
					if (length(Math::projection(rbp1, middleVector)) < length(Math::projection(rbp2, middleVector))) {
						vertexBranchPoint = bp1;
					}
					else {
						vertexBranchPoint = bp2;
					}
				}
			}
			vertexBranchPoint.id = id++;
			vec3 vertexBranchPos = vertexBranchPoint.pos;

			Circle neighbourBranch = branches.at(myVertices.at(neighbour.getConnection(0)).branch);
			startAngle = neighbourBranch.getAngle(neighbourBranch.getPoints().at(0).pos);
			plusPiOver2 = startAngle + pi<float>() / 2;
			if (plusPiOver2 >= 2 * pi<float>()) {
				plusPiOver2 -= 2 * pi<float>();
			}
			minusPiOver2 = startAngle - pi<float>() / 2;
			if (minusPiOver2 < 0) {
				minusPiOver2 += 2 * pi<float>();
			}
			Vertex neighbourBranchPoint;
			Vertex nbp1 = neighbourBranch.getPoint(minusPiOver2);
			Vertex nbp2 = neighbourBranch.getPoint(plusPiOver2);
			if (length(nbp1.pos - middlePoint) < length(nbp2.pos - middlePoint)) {
				neighbourBranchPoint = nbp1;
			}
			else {
				neighbourBranchPoint = nbp2;
			}
			neighbourBranchPoint.id = id++;
			vec3 neighbourBranchPos = neighbourBranchPoint.pos;
			vec3 average = (middlePoint + vertexBranchPos + neighbourBranchPos)/3.0f;

			Vertex middleVertex;
			middleVertex.id = id++;
			middleVertex.pos = middlePoint;
			Vertex averageVertex;
			averageVertex.id = id++;
			averageVertex.pos = average;
			averageVertex.link(middleVertex);
			averageVertex.link(vertexBranchPoint);
			averageVertex.link(neighbourBranchPoint);
			myVertices.push_back(vertexBranchPoint);
			myVertices.push_back(neighbourBranchPoint);
			myVertices.push_back(middleVertex);
			myVertices.push_back(averageVertex);
			
		}
	}

	for (int i = 0; i < myVertices.size(); i++) {
		Vertex vertex = myVertices.at(i);
		for (int j = 0; j < vertex.connectionsSize(); j++) {
			int connectionIndex = vertex.getConnection(j);
			if (vertex.useConnection(myVertices.at(connectionIndex))) {
				indices.push_back(i);
				indices.push_back(connectionIndex);
			}
		}
	}
	for (int i = 0; i < myVertices.size(); i++) {
		mesh_vertex vert;
		vert.pos = myVertices.at(i).pos;
		vertices.push_back(vert);
	}
	mesh_builder builder;
	builder.vertices = vertices;
	builder.indices = indices;
	builder.mode = GL_LINES; //for now
	return builder.build();
}

void TreeFactory::next() {

}