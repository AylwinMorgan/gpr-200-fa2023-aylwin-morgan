#pragma once

#include "../ew/ewMath/ewMath.h"

namespace am {
	// Identity matrix
	inline ew::Mat4 Identity() {
		return ew::Mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};

	// scaling on the x, y, and z axes
	inline ew::Mat4 Scale(ew::Vec3 s) {
		ew::Mat4 matrix = Identity();
		matrix[0] *= s.x;
		matrix[1] *= s.y;
		matrix[2] *= s.z;
		return matrix;
	};

	// rotating around the x axis in radians
	inline ew::Mat4 RotateX(float rad) {
		ew::Mat4 matrix = Identity();
		matrix[1][1] = cos(rad);
		matrix[1][2] = sin(rad);
		matrix[2][1] = -sin(rad);
		matrix[2][2] = cos(rad);
		return matrix;
	};

	// rotate around y axis
	inline ew::Mat4 RotateY(float rad) {
		ew::Mat4 matrix = Identity();
		matrix[0][0] = cos(rad);
		matrix[2][0] = sin(rad);
		matrix[0][2] = -sin(rad);
		matrix[2][2] = cos(rad);
		return matrix;
	};

	// rotate around z axis
	inline ew::Mat4 RotateZ(float rad) {
		ew::Mat4 matrix = Identity();
		matrix[0][0] = cos(rad);
		matrix[1][0] = -sin(rad);
		matrix[0][1] = sin(rad);
		matrix[1][1] = cos(rad);
		return matrix;
	};

	// translate x,y,z
	inline ew::Mat4 Translate(ew::Vec3 t) {
		ew::Mat4 matrix = Identity();
		matrix[3][0] += t.x;
		matrix[3][1] += t.y;
		matrix[3][2] += t.z;
		return matrix;
	};

	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0, 0.0, 0.0);
		ew::Vec3 rotation = ew::Vec3(0.0, 0.0, 0.0);
		ew::Vec3 scale = ew::Vec3(1.0, 1.0, 1.0);
		ew::Mat4 getModelMatrix() const {
			ew::Mat4 scaleMatrix = Scale(scale);
			ew::Mat4 rotationMatrixZ = RotateZ(ew::Radians(rotation.z));
			ew::Mat4 rotationMatrixX = RotateX(ew::Radians(rotation.x));
			ew::Mat4 rotationMatrixY = RotateY(ew::Radians(rotation.y));
			ew::Mat4 translationMatrix = Translate(position);
			
			ew::Mat4 matrix = translationMatrix * rotationMatrixY * rotationMatrixX * rotationMatrixZ * scaleMatrix;
			return matrix;
		}
	};
}