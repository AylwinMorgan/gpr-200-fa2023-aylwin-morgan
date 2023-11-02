#pragma once
#include "../ew/mesh.h"
namespace am {
	//ew::MeshData createSpere(float radius, int numSegments);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createPlane(float width, float height, int subdivisions);
}