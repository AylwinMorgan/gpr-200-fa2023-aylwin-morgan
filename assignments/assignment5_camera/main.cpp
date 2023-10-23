#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <am/camera.h>
#include <ew/shader.h>
#include <ew/procGen.h>
#include <ew/transform.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void moveCamera(GLFWwindow* window, am::Camera* camera, am::CameraControls* control, float deltaTime);

//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
ew::Transform cubeTransforms[NUM_CUBES];

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	// camera
	am::Camera camera;
	am::CameraControls cameraControls;
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0, 0, 0);
	camera.aspectRatio = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera.fov = 60;
	camera.orthoSize = 6;
	camera.nearPlane = 0.1;
	camera.farPlane = 100;
	camera.orthographic = false;

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;
	}

	float prevTime = 0;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//Calculate deltaTime
		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;
		moveCamera(window, &camera, &cameraControls,deltaTime);

		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();
		shader.setFloat("_Height", SCREEN_HEIGHT);
		shader.setFloat("_Width", SCREEN_WIDTH);
		shader.setMat4("_View", camera.ViewMatrix());
		shader.setMat4("_Projection", camera.ProjectionMatrix());
		//TODO: Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{

			//Construct model matrix

			shader.setMat4("_Model", cubeTransforms[i].getModelMatrix());
			cubeMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			if (ImGui::CollapsingHeader("Cubes")) {
				for (size_t i = 0; i < NUM_CUBES; i++)
				{
					ImGui::PushID(i);
					if (ImGui::CollapsingHeader("Transform")) {
						ImGui::DragFloat3("Position", &cubeTransforms[i].position.x, 0.05f);
						ImGui::DragFloat3("Rotation", &cubeTransforms[i].rotation.x, 1.0f);
						ImGui::DragFloat3("Scale", &cubeTransforms[i].scale.x, 0.5f);
					}
					ImGui::PopID();
				}
			}
			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat3("Position", &camera.position.x, 0.05);
				ImGui::DragFloat3("Target", &camera.target.x, 0.05);
				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.05, 0, 10);
				ImGui::DragFloat("Far Plane", &camera.farPlane, 0.05, 0, 100);
				ImGui::Checkbox("Orthographic", &camera.orthographic);
				if (camera.orthographic) {
					ImGui::DragFloat("Ortho Height", &camera.orthoSize);
				}
				else {
					ImGui::DragFloat("FOV", &camera.fov, 0.05, 0);
				}
				if (ImGui::Button("Reset")) {
					camera.position = ew::Vec3(0, 0, 5);
					camera.target = ew::Vec3(0, 0, 0);
					camera.fov = 60;
					camera.orthoSize = 6;
					camera.nearPlane = 0.1;
					camera.farPlane = 100;
				}
			}
			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void moveCamera(GLFWwindow* window, am::Camera* camera, am::CameraControls* controls, float deltaTime) {
	// if right mouse button is not held, release cursor and return early
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		controls->firstMouse = true;
		return;
	}
	//GLFW_CURSOR_DISABLED hides the cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// get screen mouse position this frame
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	//if a right click just started, set prevMouse values to current position
	if (controls->firstMouse) {
		controls->firstMouse = false;
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;
	}

	// get mouse position delta
	float mouseDeltaX = mouseX - controls->prevMouseX;
	float mouseDeltaY = mouseY - controls->prevMouseY;

	// add to yaw and pitch
	controls->yaw += mouseDeltaX * controls->mouseSensitivity;
	controls->pitch += mouseDeltaY * controls->mouseSensitivity;

	// clamp pitch between -89 and 89 degrees
	if (controls->pitch > 89)
		controls->pitch = 89;
	else if (controls->pitch < -89)
		controls->pitch = -89;

	controls->prevMouseX = mouseX;
	controls->prevMouseY = mouseY;

	// Graham-Schmidt process
	ew::Vec3 forward = ew::Vec3(ew::Radians(controls->yaw), -ew::Radians(controls->pitch), -camera->position.z);
	ew::Vec3 up = ew::Vec3(0, 1, 0);
	ew::Vec3 right = (ew::Cross(up, forward) / ew::Magnitude(ew::Cross(up, forward)));
	up = (ew::Cross(forward, right) / ew::Magnitude(ew::Cross(forward, right)));

	if (glfwGetKey(window, GLFW_KEY_W)) {
		camera->position += forward * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		camera->position -= forward * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		camera->position += right * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		camera->position -= right * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_Q)) {
		camera->position -= up * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_E)) {
		camera->position += up * controls->moveSpeed * deltaTime;
	}

	camera->target = camera->position + forward;
}