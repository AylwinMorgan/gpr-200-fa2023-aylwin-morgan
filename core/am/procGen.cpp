#include "procGen.h"

namespace am {
	ew::MeshData createPlane(float width, float height, int subdivisions) {
		ew::MeshData mesh;
		ew::Vertex v;
		for (float i = 0; i <= subdivisions; i++) {
			for (float j = 0; j <= subdivisions; j++) {
				v.pos.x = width * (j / subdivisions);
				v.pos.z = -height * (i / subdivisions);
				v.normal = ew::Vec3(0, 1, 0);
				v.uv = ew::Vec2(i/subdivisions,j/subdivisions);
				mesh.vertices.push_back(v);
			}
		}

		int columns = subdivisions + 1;
		int start;
		for (int i = 0; i < subdivisions; i++) {
			for (int j = 0; j < subdivisions; j++) {
				start = i * columns + j;
				//bottom right triangle
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns + 1);
				//top left triangle
				mesh.indices.push_back(start + columns + 1);
				mesh.indices.push_back(start + columns);
				mesh.indices.push_back(start);
			}

		
		}
		return mesh;
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData mesh;
		ew::Vertex v;
		float topY = height / 2;
		float bottomY = -topY;
		// top center
		mesh.vertices.push_back({ew::Vec3(0, topY, 0)});
		// top ring
		float thetaStep = ew::PI * 2 / numSegments;
		float theta;
		for (int i = 0; i <= numSegments; i++) {
			theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY;
			v.normal = ew::Vec3(0, 1, 0);
			v.uv = ew::Vec2(v.pos.x / radius, v.pos.z / radius);
			mesh.vertices.push_back(v);
		}
		for (int i = 0; i <= numSegments; i++) {
			theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY;
			v.uv = ew::Vec2(v.pos.x / radius, v.pos.z / radius);
			v.normal = ew::Normalize(ew::Vec3(v.pos.x, 0, v.pos.z));
			mesh.vertices.push_back(v);
		}
		//bottom ring
		for (int i = 0; i <= numSegments; i++) {
			theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = bottomY;
			v.uv = ew::Vec2(v.pos.x/radius,v.pos.z/radius);
			v.normal = ew::Normalize(ew::Vec3(v.pos.x, 0, v.pos.z));
			mesh.vertices.push_back(v);
		}
		for (int i = 0; i <= numSegments; i++) {
			theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = bottomY;
			v.uv = ew::Vec2(v.pos.x/radius,v.pos.z/radius);
			v.normal = ew::Vec3(0, -1, 0);
			mesh.vertices.push_back(v);
		}
		//bottom center
		mesh.vertices.push_back({ew::Vec3(0,bottomY,0)});
		

		// top ring indices
		int start = 1;
		int center = 0;
		for (int i = 0; i <= numSegments; i++) {
			mesh.indices.push_back(start + i);
			mesh.indices.push_back(center);
			mesh.indices.push_back(start + i + 1);
		}

		// bottom ring indices
		start = numSegments*3 + 4;
		center = mesh.vertices.size()-1;
		for (int i = 0; i < numSegments; i++) {
			mesh.indices.push_back(start + i);
			mesh.indices.push_back(center);
			mesh.indices.push_back(start + i + 1);
		}

		int sideStart = 1;
		int columns = numSegments + 1;
		for (int i = 0; i < columns; i++) {
			start = sideStart + i;
			// first triangle
			mesh.indices.push_back(start);
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + (2*columns));
			// second triangle
			mesh.indices.push_back(start + (2*columns));
			mesh.indices.push_back(start + (2*columns) + 1);
			mesh.indices.push_back(start);
		}

		return mesh;
	}
}