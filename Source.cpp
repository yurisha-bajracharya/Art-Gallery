#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "camera.h"
#include "stb_image.h"
#include "Model.h"
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

//settings
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

unsigned int groundVAO;

// Adjust camera position
Camera camera(glm::vec3(0.0f, 0.0f, -10.0f)); // Camera further back
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f; // stores the previous mouse position for smooth camera movement
bool firstMouse = true;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	std::cerr << "RESIze!!" << std::endl;
	glViewport(0, 0, width, height);
}

// Handle keyboard inputs
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::DOWN, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

unsigned int createGroundPlane() {
	float groundVertices[] = {
		// Positions              // Texture Coords
		-50.0f, -1.0f,  50.0f,   0.0f,  1.0f,  // Bottom-left
		 50.0f, -1.0f,  50.0f,   1.0f,  1.0f,  // Bottom-right
		 50.0f, -1.0f, -50.0f,   1.0f,  0.0f,  // Top-right
		-50.0f, -1.0f, -50.0f,   0.0f,  0.0f   // Top-left
	};


	unsigned int groundIndices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	return VAO;
}
unsigned int backgroundVAO, backgroundVBO;

// Function to create the background plane
unsigned int createBackground() {
	float backgroundVertices[] = {
		// Positions        // Colors (R, G, B)
		-1.0f,  1.0f, 0.0f, 0.5f, 0.8f, 1.0f, // Top-left (sky blue)
		 1.0f,  1.0f, 0.0f, 0.5f, 0.8f, 1.0f, // Top-right (sky blue)
		 1.0f, -1.0f, 0.0f, 0.3f, 0.5f, 0.2f, // Bottom-right (greenish brown)
		-1.0f, -1.0f, 0.0f, 0.3f, 0.5f, 0.2f  // Bottom-left (greenish brown)
	};


	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertices), backgroundVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	return VAO;
}

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Art Gallery", NULL, NULL);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	Shader backgroundShader("Shaders/background.vs", "Shaders/background.fs");
	Model ourModel("assets/models/artGallery.obj");
	Shader ourShader("Shaders/model_loading.vs", "Shaders/model_loading.fs");

	glViewport(0, 0, 800, 600);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST);

	backgroundVAO = createBackground();
	unsigned int backgroundTexture = ourModel.TextureFromFile("ground.png", "assets/textures");

	// Enable blending for PNG transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	unsigned int groundTexture = ourModel.TextureFromFile("ground.png", "assets/textures");
	groundVAO = createGroundPlane();

	// Define light properties
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 objectColor(1.0f, 1.0f, 1.0f);

	// Enable depth testing globally
	glEnable(GL_DEPTH_TEST);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		// Clear buffers before rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render the background first (disable depth testing to ensure it's always drawn)
		glDisable(GL_DEPTH_TEST);
		backgroundShader.Activate();
		glm::mat4 bgProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
		glm::mat4 bgView = glm::mat4(1.0f);
		backgroundShader.setMat4("projection", bgProjection);
		backgroundShader.setMat4("view", bgView);
		glBindVertexArray(backgroundVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST); // Re-enable depth testing for 3D rendering

		// Set up the main shader
		ourShader.Activate();

		// Set up view and projection for the main scene
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// Render the ground
		glm::mat4 groundModel = glm::mat4(1.0f);
		groundModel = glm::translate(groundModel, glm::vec3(0.0f, 0.08f, 0.0f));
		ourShader.setMat4("model", groundModel);
		glBindTexture(GL_TEXTURE_2D, groundTexture);
		glBindVertexArray(groundVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Set lighting uniforms (before rendering the model)
		ourShader.setVec3("lightPos", lightPos);
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setVec3("lightColor", lightColor);
		ourShader.setVec3("objectColor", objectColor);

		// Render the model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f)); // Model at z = 5.0f
		model = glm::rotate(model, glm::radians(00.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		ourShader.setMat4("model", model);

		ourModel.Draw(ourShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glDisable(GL_DEPTH_TEST);
	glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;  // Inverted y-axis

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

