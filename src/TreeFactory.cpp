#include "TreeFactory.h"

TreeFactory::TreeFactory(vector<Branch> branches) {
	this->branches = branches;
}
TreeFactory::~TreeFactory() {

}

//creates a tree mesh from the current state
gl_mesh TreeFactory::createTree() {
	int id = 0;
	vector<mesh_vertex> vertices;
	vector<unsigned int> indices;
	vector<Vertex> myVertices;
	for (int i = 0; i < branches.size(); i++) {
		//-----Give points a position in the index vector-----\\
		
		Circle base = branches.at(i).base;
		if (i == 0) {
			for (int j = 0; j < branches.at(0).base.getPoints().size(); j++) {
				branches.at(0).base.points.at(j).id = id++;
			}
		}
		//vector<Vertex> base_vertices;
		/*for (int j = 0; j < base.points.size(); j++) {
			Vertex vertex = Vertex();
			vertex.pos = base.points.at(j);
			vertex.id = id++;
			myVertices.push_back(vertex);
			base_vertices.push_back(vertex);
		}*/

		vector<Branch> branches = branches.at(i).branches;
		for (int j = 0; j < branches.size(); j++) {
			for (int k = 0; k < branches.at(j).base.points.size(); k++) {
				branches.at(j).base.points.at(k).id = id++;
			}
		}
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
			Vertex basePoint = base.getClosestPoint(branches.at(j).base.center);
			basePoint.pos += base.center;
			basePoint.id = id++;
			Vertex branchPoint = branches.at(j).base.getBranchPoint(base);
			branchPoint.pos += branches.at(j).base.center;
			branchPoint.id = id++;
			branchPoint.branch = j;
			basePoint.link(branchPoint);
			base.addPoint(basePoint);
			branches.at(j).base.addPoint(branchPoint);

			myVertices.push_back(basePoint);
			myVertices.push_back(branchPoint);
		}

		//----------------------------------------------------\\

		if (branches.size() == 1) {
			branches.at(0).base.points.clear();
			for (int j = 0; j < base.points.size(); j++) {
				float angle = base.getAngle(base.points.at(j).pos);
				branches.at(0).base.addPoint(angle);
				branches.at(0).base.points.at(j).link(base.points.at(j));
			}
		}
		else {

			//-----Find midpoint(s) of links on base-----\\
			
			vector<Vertex> basePoints = base.getPoints();
			for (int j = 0; j < basePoints.size(); j++) {
				Vertex vertex = basePoints.at(j);
				if (vertex.connectionsSize() == 0) {
					continue;
				}
				int k = j + 1;
				while (basePoints.at(k).connectionsSize() == 0) {
					k++;
					if (k == basePoints.size()) {
						k = 0;
					}
				}
				Vertex neighbour = base.getPoints().at(k);
				//base.addMidPoint(j, k);
				vector<Vertex> midPoints = base.getMidPoints(j, k);
				vec3 middleVector;
				for (int l = 0; l < midPoints.size(); l++) {
					middleVector += midPoints.at(l).pos;
				}
				middleVector /= midPoints.size();
				//vector<Vertex> newBasePoints = base.getPoints();
				//vec3 middleVector = newBasePoints.at(newBasePoints.size()-1).pos - base.center;//position relative to the centre of the circle
				Circle branch = branches.at(myVertices.at(vertex.getConnection(0)).branch).base;
				int l;
				for (int m = 0; m < branch.getPoints().size(); m++) {
					if (branch.getPoints().at(m).connectionsSize() != 0) {
						l = m;
					}
				}
				/*
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
				*/
				int plusPiOver2 = l + branch.getPoints().size() / 4; //at this point all the branches should have an even distribution of resolution
				if (plusPiOver2 >= branch.getPoints().size()) {
					plusPiOver2 -= branch.getPoints().size();
				}
				int minusPiOver2 = l - branch.getPoints().size() / 4;
				if (minusPiOver2 < 0) {
					minusPiOver2 += branch.getPoints().size();
				}
				Vertex vertexBranchPoint;
				/*Vertex bp1 = branch.getPoint(minusPiOver2);
				Vertex bp2 = branch.getPoint(plusPiOver2);*/
				Vertex bp1 = branch.getPoints().at(minusPiOver2);
				Vertex bp2 = branch.getPoints().at(plusPiOver2);

				vec3 rbp1 = bp1.pos - branch.center;
				vec3 rbp2 = bp2.pos - branch.center;
				if (length(Math::projection(rbp1, middleVector) + middleVector) > length(middleVector)) {
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
				Circle neighbourBranch = branches.at(myVertices.at(neighbour.getConnection(0)).branch).base;
				for (int m = 0; m < neighbourBranch.getPoints().size(); m++) {
					if (neighbourBranch.getPoints().at(m).connectionsSize() != 0) {
						l = m;
					}
				}
				plusPiOver2 = l + neighbourBranch.getPoints().size() / 4;
				if (plusPiOver2 >= neighbourBranch.getPoints().size()) {
					plusPiOver2 -= neighbourBranch.getPoints().size();
				}
				minusPiOver2 = l - neighbourBranch.getPoints().size() / 4;
				if (minusPiOver2 < 0) {
					minusPiOver2 += neighbourBranch.getPoints().size();
				}
				Vertex neighbourBranchPoint;
				Vertex nbp1 = neighbourBranch.getPoints().at(minusPiOver2);
				Vertex nbp2 = neighbourBranch.getPoints().at(plusPiOver2);
				vec3 rnbp1 = nbp1.pos - neighbourBranch.center;
				vec3 rnbp2 = nbp2.pos - neighbourBranch.center;
				if (length(Math::projection(rnbp1, middleVector) + middleVector) > length(middleVector)) {
					if (length(Math::projection(rnbp2, middleVector) + middleVector) > length(middleVector)) {
						if (length(Math::projection(rnbp1, middleVector)) > length(Math::projection(rnbp2, middleVector))) {
							neighbourBranchPoint = nbp1;
						}
						else {
							neighbourBranchPoint = nbp2;
						}
					}
					else {
						neighbourBranchPoint = nbp1;
					}
				}
				else {
					if (length(Math::projection(rnbp2, middleVector) + middleVector) > length(middleVector)) {
						neighbourBranchPoint = nbp2;
					}
					else {
						if (length(Math::projection(rnbp1, middleVector)) < length(Math::projection(rnbp2, middleVector))) {
							neighbourBranchPoint = nbp1;
						}
						else {
							neighbourBranchPoint = bp2;
						}
					}
				}
				vec3 neighbourBranchPos = neighbourBranchPoint.pos;
				vec3 average;
				if (midPoints.size() == 1) {
					average = (midPoints.at(0).pos + vertexBranchPos + neighbourBranchPos) / 3.0f;
					Vertex averageVertex;
					averageVertex.pos = average;
					averageVertex.link(midPoints.at(0));
					averageVertex.link(vertexBranchPoint);
					averageVertex.link(neighbourBranchPoint);
					myVertices.push_back(vertexBranchPoint);
					myVertices.push_back(neighbourBranchPoint);
					myVertices.push_back(midPoints.at(0));
					myVertices.push_back(averageVertex);
				}
				else {
					average = (midPoints.at(0).pos + midPoints.at(1).pos + vertexBranchPos + neighbourBranchPos) / 4.0f;
					Vertex averageVertex;
					averageVertex.pos = average;
					averageVertex.link(midPoints.at(0));
					averageVertex.link(midPoints.at(1));
					averageVertex.link(vertexBranchPoint);
					averageVertex.link(neighbourBranchPoint);
					myVertices.push_back(vertexBranchPoint);
					myVertices.push_back(neighbourBranchPoint);
					myVertices.push_back(midPoints.at(0));
					averageVertex.link(midPoints.at(1));
					myVertices.push_back(averageVertex);
				}
			}



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