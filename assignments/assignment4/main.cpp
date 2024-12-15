//Thomas Flavin
// Credited For Helping me use his code:: Joseph Isaacs
// 10/21/24

#include <stdio.h>
#include <iostream>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "thomasLib/Shaders.h"
#include "../../core/thomasLib/camera.h"
#include "thomasLib/camera.h"
#include <thomasLib/textures.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ew/external/stb_image.h>


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

enum ProjectionType {
	ORTHOGRAPHIC = 0,
	PERSPECTIVE = 1
};
//struct Light {
//	glm::vec3 position;
//	glm::vec3 color;
//};
//
//Light light = { glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f) }; // Default light properties


float deltaTime = 0.0f;
float lastFrame = 0.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
const float nearPlane = 0.1f, farPlane = 1000.0f; //- Near plane of 0.1, Far plane of 1000
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);    // White color
float ambientK = 0.5f;
float specularK = 0.5f;
float diffuseK = 0.5f;
float ambientStrength = 0.1f;
float shininess = 100.0f;
//cel shading 
float rimcut = 0.1f;
float threshold = 5.4f;
//end of cell shading
const unsigned int CUBENUM = 20;
glm::vec3 cubePositions[CUBENUM];
glm::vec3 cubeScales[CUBENUM];
glm::vec3 cubeRotations[CUBENUM];
glm::vec3 cubeRotationDirs[CUBENUM];
float cubeRotationAngles[CUBENUM];
float cubeRotationSpeed = 0.7f;

const float cubeVertices[] = {
	//-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f,
	// 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, -1.0f, 1.0f,
	// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 1.0f,
	// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 1.0f,
	//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,
	//-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f,

	//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,
	// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f,
	// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
	// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
	//-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f,
	//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,

	//-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
	//-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f,
	//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f,
	//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f,
	//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,
	//-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,

	// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
	// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f,
	// 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
	// 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
	// 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,
	// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,

	//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f,
	// 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f,
	// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
	// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
	//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,

	//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f,
	// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f,
	// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
	// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
	//-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,
	//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f
	-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};


float planeVertices[] = {
	// positions            // normals         // texcoords
	 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
	-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
	-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

	 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
	-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
	 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
};
const float speed = 2.5f, sprintSpeedMultiplier = 2.0f;
void movementInput(GLFWwindow* window) {
	float cameraSpeed = speed * deltaTime; //- All movement must be scaled by deltaTime to be framerate independent

	//- Left Shift to Sprint(double base movement speed if held)*
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed *= sprintSpeedMultiplier;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, UP)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, UP)) * cameraSpeed;

	//- Q, E to move down / up along local Y axis, respectively*
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(glm::cross(cameraFront, UP), cameraFront)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(glm::cross(cameraFront, UP), cameraFront)) * cameraSpeed;
}

//- Bonus +2: Add an option to toggle between Perspective and Orthographic projection.
// press [TAB] 
ProjectionType cameraMode = ProjectionType::PERSPECTIVE;
bool holdingCameraModeToggle = false;
void cameraProjectionInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		if (holdingCameraModeToggle == false) {
			cameraMode = (ProjectionType)((cameraMode + 1) % 2);
		}
		holdingCameraModeToggle = true;
	}
	else {
		holdingCameraModeToggle = false;
	}
}

void processInput(GLFWwindow* window)
{
	movementInput(window);
	cameraProjectionInput(window);
}

//- Aiming with mouse
const float sensitivity = 0.1f;
float pitch, yaw = -90.0f; //- Camera starts looking looking along world Z axis.
float lastX = 0.0f, lastY = 0.0f;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	//- Pitch is clamped to - 89 and 89 degrees
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

//- Mouse scroll to change FOV
float fov = 60.0f; //- Perspective projection with a default FOV of 60 degrees
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;

	//- Clamp FOV between 1 and 120 degrees
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 120.0f)
		fov = 120.0f;
}
// Load texture function
unsigned int loadTexture(const char* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1) format = GL_RED;
		else if (nrChannels == 3) format = GL_RGB;
		else if (nrChannels == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

int main() {
#pragma region Initialization
	printf("Initializing...");
	GLFWwindow* window;
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
#pragma endregion
#pragma region Geometry data
	// Background
	unsigned int VAO, VBO, EBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 5));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
#pragma endregion

	//- Cubes at random positions, rotations, and scales
	for (int i = 0; i < CUBENUM; i++) {
		float cubeBounds = 6.0f;
		float fieldStartsAt = -4.0f;
		cubePositions[i].x = ew::RandomRange(-cubeBounds, cubeBounds);
		cubePositions[i].y = ew::RandomRange(-cubeBounds, cubeBounds);
		cubePositions[i].z = ew::RandomRange(fieldStartsAt, fieldStartsAt - cubeBounds * 2);

		float cubeMinScale = 0.25f;
		float cubeMaxScale = 1.2f;
		cubeScales[i] = glm::vec3(ew::RandomRange(cubeMinScale, cubeMaxScale));

		float cubeRotationAngle = ew::RandomRange(0.0f, 3.0f);
		cubeRotations[i] = cubeRotationAngle * glm::vec3(ew::RandomRange(-1.0f, 1.0f), ew::RandomRange(-1.0f, 1.0f), ew::RandomRange(-1.0f, 1.0f));
		cubeRotationDirs[i] = glm::vec3(ew::RandomRange(-1.0f, 1.0f), ew::RandomRange(-1.0f, 1.0f), ew::RandomRange(-1.0f, 1.0f));
		cubeRotationAngles[i] = 0.0f;
	}
	// configure global opengl state
   // -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shaders shader("assets/shader/vertexShader.vert", "assets/shader/fragmentShader.frag");
	unsigned int slime = loadTexture("assets/textures/tophatslimeboy.png");
	unsigned int floorTexture = loadTexture("assets/textures/wood.png");
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Initialize ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();  // Access ImGui's I/O if needed

	// Setup ImGui style
	ImGui::StyleColorsDark();

	// Initialize ImGui for GLFW and OpenGL3
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	while (!glfwWindowShouldClose(window)) {
		// Inputs
		glfwPollEvents();
		processInput(window);

		// Update
		float time = glfwGetTime();
		deltaTime = time - lastFrame;
		lastFrame = time;

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, UP);

		glm::mat4 projection = glm::mat4(1.0);
		if (cameraMode == ProjectionType::PERSPECTIVE)
			projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / SCREEN_HEIGHT, nearPlane, farPlane);  //- FOV accounts for the screen's aspect ratio
		else
			projection = glm::ortho(-SCREEN_WIDTH / 300.0f, SCREEN_WIDTH / 300.0f, -SCREEN_HEIGHT / 300.0f, SCREEN_HEIGHT / 300.0f, nearPlane, farPlane);

		GLuint press = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2); //Returns GLFW_PRESS or GLFW_RELEASE
		if (press == GLFW_PRESS) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Locks



		}
		else {

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //Unlocks
		}


		// background
		glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// brick cubes
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);

		// ... (set other uniforms as needed)

		// Render your scene
		//renderScene();
		//Its comprompising the fragmentShader.frag into the main.cpp
		shader.use();
		//shader.setInt("floor", 0);
		shader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
		shader.setVec3("lightColor", lightColor);
		shader.setVec3("lightPos", lightPos);
		shader.setFloat("shininess", shininess);
		shader.setFloat("ambientStrength", ambientK);
		shader.setFloat("specularStrength", specularK);
		shader.setFloat("diffuseStrength", diffuseK);
		//cel shading
		shader.setFloat("rimcut", rimcut);
		shader.setFloat("rimThreshold", threshold);
		//end of cell shading
		//brick.Bind(GL_TEXTURE0);
		glBindVertexArray(VAO);
		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	//	glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		shader.setMat4("model", model);
		glBindVertexArray(planeVAO);

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		shader.setMat4("model", model);
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// render the cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// also draw the lamp object
		//shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		shader.setMat4("model", model);

		bool useBlinn = false; // Set this based on your logic
		shader.setInt("blinn", useBlinn);


		// render the cube
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// also draw the lamp object
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, slime);
		//Start Drawing ImGUI
		//shader.setMat4("projection", projection);
		//shader.setMat4("view", view);
		/*model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); */// a smaller cube
		//shader.setMat4("model", model);
		//shader.setMat4("projection", projection);
		//shader.setMat4("view", view);
		for (unsigned int i = 0; i < CUBENUM; i++)
		{
			// BONUS +2: Animate your cubes by changing position, rotation, and/or scale every frame. This movement must be scaled with deltaTime.
			float angle = 7.0f * (i + 1) * time;
			cubeRotationAngles[i] += cubeRotationSpeed * deltaTime;

			glm::mat4 Cubemodel = glm::mat4(1.0f);
			Cubemodel = glm::translate(Cubemodel, cubePositions[i]);
			Cubemodel = glm::rotate(Cubemodel, cubeRotationAngles[i], cubeRotations[i]);
			Cubemodel = glm::scale(Cubemodel, cubeScales[i]);

			shader.setMat4("model", Cubemodel);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// Start ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Begin an ImGui window
		ImGui::Begin("Settings");
		ImGui::Text("Adjust Lighting Parameters:");
		ImGui::DragFloat3("Light Position", &lightPos.x, 0.1f);
		ImGui::ColorEdit3("Light Color", &lightColor.r);
		ImGui::SliderFloat("Ambient K", &ambientK, 0.0f, 1.0f);
		ImGui::SliderFloat("DiffuseK", &diffuseK, 0.0f, 1.0f);
		ImGui::SliderFloat("SpecularK", &specularK, 0.0f, 1.0f);
		ImGui::SliderFloat("shininess", &shininess, 0.0, 255.0f);
		ImGui::End();

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");

}
