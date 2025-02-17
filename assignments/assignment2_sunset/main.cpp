#include <stdio.h>
#include <math.h>

#include <am/shader.h>
#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace std;


struct Vertex {
	float x, y, z;
	float u, v;
};
Vertex vertices[4] = {
	//x     y      z     u     v
	{-1.0, -1.0,  0.0,  -1.0,  -1.0}, //bottom left 
	{ 1.0, -1.0,  0.0,  1.0,  -1.0}, // bottom right
	{ 1.0,  1.0,  0.0,  1.0,  1.0}, // top right
	{-1.0,  1.0,  0.0,  -1.0,  1.0}  // top left
};

/*
float vertices[12] = {
	//x    y    z
	-0.5, -0.5, 0.0, // bottom left
	 0.5, -0.5, 0.0, // bottom right
	 0.5,  0.5, 0.0, // top right
	-0.5,  0.5, 0.0  // top left
};
*/

unsigned int indices[6] = {
	0, 1, 2, //triangle 1
	2, 3, 0 // triangle 2
};


// colors
float mountainColor[3] = { 0.2, 0.3, 0.2 };

float skyColor1[3] = { 0.3, 0.6, 0.7 };
float skyColor2[3] = { 1.0, 0.45, 0.3 };

float skyColor3[3] = { 0.2, 0.4, 0.6 };
float skyColor4[3] = {0.3, 0.2, 0.15};

float sunColor1[3] = {1.0, 0.6, 0.0};
float sunColor2[3] = { 1.0, 1.0, 0.0 };

float sunSpeed = 1.0;
bool showImGUIDemoWindow = true;

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 960;


int main() {
	float time;
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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

	std::string vertexShaderSource = am::loadShaderSourceFromFile("assets/vertexShader.vert");
	std::string fragmentShaderSource = am::loadShaderSourceFromFile("assets/fragmentShader.frag");
	am::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shader.use();
	unsigned int vao = createVAO(vertices, 4, indices, 6);

	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		
		//Wireframe
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		time = (float)glfwGetTime();

		glClearColor(0.7f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms
		shader.setFloat("iTime", time);
		shader.setFloat("sunSpeed", sunSpeed);

		shader.setVec3("skyColor1", skyColor1[0], skyColor1[1], skyColor1[2]);
		shader.setVec3("skyColor2", skyColor2[0], skyColor2[1], skyColor2[2]);
		shader.setVec3("skyColor3", skyColor3[0], skyColor3[1], skyColor3[2]);
		shader.setVec3("skyColor4", skyColor4[0], skyColor4[1], skyColor4[2]);

		shader.setVec3("sunColor1", sunColor1[0], sunColor1[1], sunColor1[2]);
		shader.setVec3("sunColor2", sunColor2[0], sunColor2[1], sunColor2[2]);

		shader.setVec3("mountainColor", mountainColor[0], mountainColor[1], mountainColor[2]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,NULL);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Day Sky 1", skyColor1);
			ImGui::ColorEdit3("Day Sky 2", skyColor2);
			ImGui::ColorEdit3("Night Sky 1", skyColor3);
			ImGui::ColorEdit3("Night Sky 2", skyColor4);
			ImGui::ColorEdit3("Sun Color 1", sunColor1);
			ImGui::ColorEdit3("Sun Color 2", sunColor2);
			ImGui::ColorEdit3("Mountain Color", mountainColor);
			
			ImGui::SliderFloat("Sun Speed", &sunSpeed, 0.0f, 10.0f);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}


unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	// element buffer object
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);
	
	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 5, vertexData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,x));
	glEnableVertexAttribArray(0);

	// UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);
	
	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

