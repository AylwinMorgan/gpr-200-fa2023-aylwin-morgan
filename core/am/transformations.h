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

	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 up) {
		ew::Vec3 f1 = eye - target;
		float f2 = ew::Magnitude(f1);
		ew::Vec3 f = f1 / f2;
		ew::Vec3 r1 = ew::Cross(up, f);
		float r2 = ew::Magnitude(r1);
		ew::Vec3 r = r1 / r2;
		ew::Vec3 u1 = ew::Cross(f, r);
		float u2 = ew::Magnitude(u1);
		ew::Vec3 u = u1 / u2;

		ew::Mat4 matrix = Identity();
		matrix[0][0] = r.x;
		matrix[1][0] = r.y;
		matrix[2][0] = r.z;

		matrix[0][1] = u.x;
		matrix[1][1] = u.y;
		matrix[2][1] = u.z;

		matrix[0][2] = f.x;
		matrix[1][2] = f.y;
		matrix[2][2] = f.z;

		matrix[3][0] = -(ew::Dot(r, eye));
		matrix[3][1] = -(ew::Dot(u, eye));
		matrix[3][2] = -(ew::Dot(f, eye));
		return matrix;
	};

	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far) {
		float width = aspect * height;

		ew::Mat4 matrix = Identity();
		matrix[0][0] = 2 / width;
		matrix[1][1] = 2 / height;
		matrix[2][2] = -(2 / far - near);

		return matrix;
	};

	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far) {
		ew::Mat4 matrix = Identity();
		fov = ew::Radians(fov);
		matrix[0][0] = (1 / tan(fov / 2)*aspect);
		matrix[1][1] = (1 / tan(fov / 2));
		matrix[2][2] = ((near + far) / (near - far));
		matrix[2][3] = -1;
		matrix[3][2] = (2 * far * near / (near - far));
		matrix[3][3] = 0;
		return matrix;
	};

	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0, 0.0, 0.0);
		ew::Vec3 rotation = ew::Vec3(0.0, 0.0, 0.0);
		ew::Vec3 scale = ew::Vec3(1.0, 1.0, 1.0);
		ew::Mat4 getModelMatrix() const {
			ew::Mat4 scaleMatrix = am::Scale(scale);
			ew::Mat4 rotationMatrixZ = RotateZ(ew::Radians(rotation.z));
			ew::Mat4 rotationMatrixX = RotateX(ew::Radians(rotation.x));
			ew::Mat4 rotationMatrixY = RotateY(ew::Radians(rotation.y));
			ew::Mat4 translationMatrix = am::Translate(position);
			
			ew::Mat4 matrix = translationMatrix * rotationMatrixY * rotationMatrixX * rotationMatrixZ * scaleMatrix;
			return matrix;
		}
	};
}