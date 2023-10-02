#include <stdio.h>
#include <math.h>

#include <am/texture.h>
#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>

struct Vertex {
	float x, y, z;
	float u, v;
};

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;

Vertex vertices[4] = {
	{-1.0, -1.0, 0.0, 0.0, 0.0},
	{1.0, -1.0, 0.0, 1.0, 0.0},
	{1.0, 1.0, 0.0, 1.0, 1.0},
	{-1.0, 1.0, 0.0, 0.0, 1.0}
};
unsigned short indices[6] = {
	0, 1, 2,
	2, 3, 0
};


int main() {

	float time;
	float zoomSpeed = 1;
	float distortSpeed = 1;
	float catSpeed = 1;
	float maxDistortion = 0.5;

	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures", NULL, NULL);
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	ew::Shader backgroundShader("assets/background.vert", "assets/background.frag");
	ew::Shader characterShader("assets/character.vert", "assets/character.frag");
	unsigned int quadVAO = createVAO(vertices, 4, indices, 6);
	unsigned int textureA = loadTexture("assets/brick.png", GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	unsigned int textureB = loadTexture("assets/noise.png", GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	unsigned int textureC = loadTexture("assets/cat.png", GL_CLAMP_TO_BORDER, GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST);

	glBindVertexArray(quadVAO);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		time = (float)glfwGetTime();

		//Setup for background shader
		backgroundShader.use();
		//place textureA in unit 0
		glActiveTexture(GL_TEXTURE0);
 		glBindTexture(GL_TEXTURE_2D, textureA);
		// place textureB in unit 1
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureB);
		
		// make sampler 2d _BrickTexture sample from unit 0
		backgroundShader.setInt("_BrickTexture", 0);
		// make sampler 2d _NoiseTexture sample from unit 1
		backgroundShader.setInt("_NoiseTexture", 1);

		// these variables affect the speed that the background zooms and distorts and limits how much it is affected by the noise.png
		backgroundShader.setFloat("zoomSpeed", zoomSpeed);
		backgroundShader.setFloat("distortSpeed", distortSpeed);
		backgroundShader.setFloat("maxDistortion", maxDistortion);

		backgroundShader.setFloat("time", time);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

		//setup for character
		characterShader.use();
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureC);

		characterShader.setInt("_CatTexture", 2);
		characterShader.setFloat("time", time);
		characterShader.setFloat("catSpeed", catSpeed);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
		


		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::SliderFloat("Zoom Speed", &zoomSpeed, 0.0, 8.0);
			ImGui::SliderFloat("Distortion Speed", &distortSpeed, 0.0, 8.0);
			ImGui::SliderFloat("Maximum Distortion", &maxDistortion, 0.0, 1.0);
			ImGui::SliderFloat("Cat Movement Speed", &catSpeed, 0.0, 8.0);
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Vertex Buffer Object 
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, vertexData, GL_STATIC_DRAW);

	//Element Buffer Object
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * numIndices, indicesData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,x));
	glEnableVertexAttribArray(0);

	//UV attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

