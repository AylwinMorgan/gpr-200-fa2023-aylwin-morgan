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
		v.pos.x = 0;
		v.pos.y = topY;
		v.pos.z = 0;
		v.normal = (0, 1, 0);
		v.uv = ew::Vec2(0.5, 0.5);
		mesh.vertices.push_back(v);
		// top ring
		float thetaStep = ew::PI * 2 / numSegments;
		float theta;
		for (float i = 0; i <= numSegments; i++) {
			theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY;
			v.normal = ew::Vec3(0, 1, 0);
			v.uv = ew::Vec2(v.pos.x / radius / 2 + 0.5, v.pos.z / radius / 2 + 0.5);
			mesh.vertices.push_back(v);
		}
		for (float i = 0; i <= numSegments; i++) {
			theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY;
			v.uv = ew::Vec2(i/numSegments, 1);
			v.normal = ew::Normalize(ew::Vec3(v.pos.x, 0, v.pos.z));
			mesh.vertices.push_back(v);
		}
		//bottom ring
		for (float i = 0; i <= numSegments; i++) {
			theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = bottomY;
			v.uv = ew::Vec2(i / numSegments, 0);
			v.normal = ew::Normalize(ew::Vec3(v.pos.x, 0, v.pos.z));
			mesh.vertices.push_back(v);
		}
		for (float i = 0; i <= numSegments; i++) {
			theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = bottomY;
			v.uv = ew::Vec2(v.pos.x / radius / 2 + 0.5, v.pos.z / radius / 2 + 0.5);
			v.normal = ew::Vec3(0, -1, 0);
			mesh.vertices.push_back(v);
		}
		//bottom center
		v.pos.x = 0;
		v.pos.y = bottomY;
		v.pos.z = 0;
		v.normal = (0, -1, 0);
		v.uv = ew::Vec2(0.5, 0.5);
		mesh.vertices.push_back(v);

		// top ring indices
		int start = 1;
		int center = 0;
		for (int i = 0; i <= numSegments; i++) {
			mesh.indices.push_back(start + i);
			mesh.indices.push_back(center);
			mesh.indices.push_back(start + i + 1);
		}

		int sideStart = numSegments + 1;
		int columns = numSegments + 1;
		for (int i = 0; i < columns; i++) {
			start = sideStart + i;
			// first triangle
			mesh.indices.push_back(start);
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns);
			// second triangle
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns + 1);
			mesh.indices.push_back(start + columns);
		}

		// bottom ring indices
		start = numSegments * 3 + 4;
		center = mesh.vertices.size() - 1;
		for (int i = 0; i < numSegments; i++) {
			mesh.indices.push_back(center);
			mesh.indices.push_back(start + i);
			mesh.indices.push_back(start + i + 1);
		}

		return mesh;
	}

	ew::MeshData createSphere(float radius, int numSegments) {
		ew::MeshData mesh;
		ew::Vertex v;
		float thetaStep = 2 * ew::PI / numSegments;
		float phiStep = ew::PI / numSegments;
		for (float i = 0; i <= numSegments; i++) {
			// first and last row converge at poles
			float phi = i * phiStep;
			for (float j = 0; j <= numSegments; j++) {
				float theta = j * thetaStep;
				v.pos.x = radius * cos(theta) * sin(phi);
				v.pos.y = radius * cos(phi);
				v.pos.z = radius * sin(theta) * sin(phi);
				v.normal = ew::Normalize(ew::Vec3(v.pos.x, v.pos.y, v.pos.z));
				v.uv = ew::Vec2(j/numSegments,i/numSegments);
				mesh.vertices.push_back(v);
			}
		}

		// indices 
		int poleStart = 0;
		int sideStart = numSegments + 1;
		for (int i = 0; i < numSegments; i++) {
			mesh.indices.push_back(sideStart + i);
			mesh.indices.push_back(poleStart + i);
			mesh.indices.push_back(sideStart + i + 1);
		}
		
		int columns = numSegments + 1;
		for (int i = 1; i < numSegments - 1; i++) {
			for (int j = 0; j < numSegments; j++) {
				int start = i * columns + j;
				//triangle 1
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns);
				// triangle 2
				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns + 1);
				mesh.indices.push_back(start + columns);
				
			}
		}

		poleStart = mesh.vertices.size() - 1;
		sideStart = poleStart - numSegments - 1;
		for (int i = 0; i < numSegments; i++) {
			mesh.indices.push_back(sideStart - i);
			mesh.indices.push_back(poleStart - i);
			mesh.indices.push_back(sideStart - i - 1);
		}

		return mesh;
	}
}