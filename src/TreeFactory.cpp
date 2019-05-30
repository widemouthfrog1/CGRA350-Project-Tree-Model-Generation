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

		Circle base = alphabet.at(i).getBranch().base;
		//vector<Math::Vertex> base_vertices;
		/*for (int j = 0; j < base.points.size(); j++) {
			Math::Vertex vertex = Math::Vertex();
			vertex.pos = base.points.at(j);
			vertex.id = id++;
			myVertices.push_back(vertex);
			base_vertices.push_back(vertex);
		}*/

		vector<Circle> branches = alphabet.at(i).getBranch().branches;
		//vector<vector<Math::Vertex>> branch_vertices_vector;
		/*for (int j = 0; j < branches.size(); j++) {
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
		}*/
		//----------------------------------------------------\\

		//-----Link bottom of each branch (circle) to the closest point on the base-----\\

		/*for (int j = 0; j < branch_vertices_vector.size(); j++) {
			vector<Math::Vertex> branch = branch_vertices_vector.at(j);
			//this is likely making a copy and I will need to replace it with pointers
			Math::Vertex closest = Math::closestToBasePlane(base_vertices, branch);
			Math::Vertex closestBase = Math::closestBasePoint(base_vertices, closest);
			closestBase.connections.push_back(closest.id);
			closest.connections.push_back(closestBase.id);
		}*/
		for (int j = 0; j < branches.size(); j++) {
			vector<float> angles = Math::angleOfClosestPointsOnTwoCircles(base, branches.at(j));
			Math::Vertex basePoint = base.getPoint(angles.at(0));
			basePoint.id = id++;
			Math::Vertex branchPoint = base.getPoint(angles.at(0));
			branchPoint.id = id++;
			branchPoint.branch = j;
			basePoint.link(&branchPoint);
			base.addPoint(basePoint);
			branches.at(j).addPoint(branchPoint);

			myVertices.push_back(basePoint);
			myVertices.push_back(branchPoint);
		}
		//----------------------------------------------------\\
		
		//-----Find midpoint(s) of links on base-----\\
		
		vector<Math::Vertex> basePoints = base.getPoints();
		for (int j = 0; j < basePoints.size(); j++) {
			Math::Vertex vertex = basePoints.at(j);
				
			int k = j + 1;
			if (k == basePoints.size()) {
				k == 0;
			}
			Math::Vertex neighbour = base.getPoints.at(k);
			base.addMidPoint(j, k);
			vector<Math::Vertex> newBasePoints = base.getPoints();
			vec3 middleVector = newBasePoints.at(newBasePoints.size()-1).pos - base.center;//position relative to the centre of the circle
			Circle branch = branches.at(myVertices.at(vertex.connections.at(0)).branch);
			float startAngle = branch.getAngle(branch.getPoints().at(0).pos);
			float plusPiOver2 = startAngle + pi<float>() / 2;
			if (plusPiOver2 >= 2 * pi<float>()) {
				plusPiOver2 -= 2 * pi<float>();
			}
			float minusPiOver2 = startAngle - pi<float>() / 2;
			if (minusPiOver2 < 0) {
				minusPiOver2 += 2 * pi<float>();
			}
			Math::Vertex vertexBranchPoint;
			Math::Vertex bp1 = branch.getPoint(minusPiOver2);
			Math::Vertex bp2 = branch.getPoint(plusPiOver2);
			vec3 middlePoint = middleVector + base.center;
			if (length(bp1.pos - middlePoint) < length(bp2.pos - middlePoint)) {
				vertexBranchPoint = bp1;
			}
			else {
				vertexBranchPoint = bp2;
			}
			vertexBranchPoint.id = id++;
			vec3 vertexBranchPos = vertexBranchPoint.pos;

			Circle neighbourBranch = branches.at(myVertices.at(neighbour.connections.at(0)).branch);
			Math::Vertex neighbourBranchPoint;
			Math::Vertex nbp1 = neighbourBranch.getPoint(minusPiOver2);
			Math::Vertex nbp2 = neighbourBranch.getPoint(plusPiOver2);
			if (length(nbp1.pos - middlePoint) < length(nbp2.pos - middlePoint)) {
				neighbourBranchPoint = nbp1;
			}
			else {
				neighbourBranchPoint = nbp2;
			}
			neighbourBranchPoint.id = id++;
			vec3 neighbourBranchPos = neighbourBranchPoint.pos;
			vec3 average = (vertex.pos + vertexBranchPos + neighbourBranchPos)/3.0f;

			Math::Vertex averageVertex;
			averageVertex.id = id++;
			averageVertex.link(&vertex);
			averageVertex.link(&vertexBranchPoint);
			averageVertex.link(&neighbourBranchPoint);
			myVertices.push_back(averageVertex);
			
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