#pragma once
#include "transformations.h"

namespace am {
	struct  Camera{
		ew::Vec3 position;
		ew::Vec3 target;
		float fov;
		float aspectRatio;
		float nearPlane;
		float farPlane;
		bool orthographic;
		float orthoSize;
		ew::Vec3 up = ew::Vec3(0, 1, 0);
		ew::Mat4 ViewMatrix() {
			return LookAt(position, target, up);
		};
		ew::Mat4 ProjectionMatrix() {
			if (orthographic) {
				return Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
			}
			else {
				return Perspective(fov, aspectRatio, nearPlane, farPlane);
			}
		}
	};
}