//Standard Libs
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <time.h>

//Extension wrangler (glad) opengl loading library + Window/Context creation (glfw) opengl library framework
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//GLM (OpenGL Mathematics)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//World Libs
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "tile.h"
#include "world.h"

//GSR lib
#include "gsr.h"

//Texture Lib
#define STB_IMAGE_IMPLEMENTATION	//Required for stb.
#include "stb_image.h"

//Audio Lib
#include <irrklang/irrKlang.h>
using namespace irrklang;
ISoundEngine *SoundEngine = createIrrKlangDevice();
ISound* snd = SoundEngine->play2D("footsteps.wav", true, false, true);

#define TIME_PER_FRAME_MICRO_SECONDS 1000 / 60
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//World Variables//
World world;
Tile t = Tile(NORTH);
glm::vec2 currentPosition = glm::vec2(0, 0);
int tileCounter = 0;

//Camera Variables//
Camera camera(glm::vec3(0.0f, 0.0f, -1.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
float deltaTime = 1.0f;
float yaw = 0, pitch = 0, fov = 90;

//Misc Variables//
bool firstMouseMovement = true;
float mixRate = 0;
float control = 0;
int success = true;

//Window + Context//
GLFWwindow* windowInit();

//World Building Functions//
void placeTile(Tile tile, glm::vec3 translation, int numberTiles, glm::mat4* modelMatrix, int VBO, Shader shaderProgram, unsigned int texture);
void placeTile(Tile tile, glm::vec3 translation, int numberTiles, glm::mat4* modelMatrix, int VBO, Shader shaderProgram, unsigned int floor, unsigned int walls, unsigned int ceiling);
void drawPanel(Tile tile, unsigned int texture, int vertice);

//Callbacks//
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);

int main(int argc, char* argv[])
{
	//Setup Serial Connection for gsr logging.
	SerialConnection sc;
	char str[20];
	sprintf(str, "\\\\.\\COM%s", argv[1]);
	//if (!setupSerial(str, &sc))
	//	return -1;
	/////////////////////////////////////////////////////

	//Initalize window.
	GLFWwindow* window = windowInit();
	if (!window)
		return -1;

	//Initialize GLAD.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Error: Failed to initialize GLAD." << std::endl;
		return -1;
	}

	//Set size of rendering window. The View port.
	glViewport(0, 0, 800, 600);
	//Enable depth testing.
	glEnable(GL_DEPTH_TEST);

	//Set callbacks for window resize, keys, cursor. (GLFW)
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	///////////////////////////////////////////////////////

	//Vertex object + vertex array buffers setup.
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Set vertex attribute pointers. (How vertices are read into glsl)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//A third attribute pointer if need. Before used for indices I think.
	/*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);*/
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	////////////////////////////////////////////

	//Load images into textures.
	//Texture 1
	//unsigned int texture1 = generate2DTextureProcedural();
	//Texture 2
	//unsigned int texture2 = generate2DTexture("textures/creepy_texture.jpg");

	//Create + compile vertex shader.
	Shader shaderProgram("shaders/vsOriginal.txt", "shaders/fsOriginal.txt");
	shaderProgram.use();	//Finding uniform location not require shader to be running but updating its value does.
	////////////////////////////////

	Shader shaderProgram2("shaders/vsOriginal.txt", "shaders/fsProcedural.txt");

	Shader shaderProgram3("shaders/vsOriginal.txt", "shaders/fsProceduralTime.txt");

	Shader shaderProgram4("shaders/vsOriginal.txt", "shaders/fsProceduralTimeFast.txt");

	//unsigned int texture1 = generateBrickTexture(1920, 1080);
	unsigned int texture1 = generateLatticeNoise(200, 200);
	unsigned int texture2 = generateBrickTexture(200, 200);
	unsigned int texture3 = generateCloudTexture(200, 200);
	//shaderProgram.setInt("ourTexture01", 0);
	//shaderProgram.setInt("ourTexture02", 1);

	//A random value for shaders if needed.
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	shaderProgram.setFloat("randomPassed", r);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//The render loop.
	while (!glfwWindowShouldClose(window))
	{
		//Record time before rendering.
		clock_t beginFrame = clock();

		//Input...
		processInput(window);
		currentPosition = glm::vec2(0, 0);
		tileCounter = 0;

		if((camera.Position.z >= -30.6 && camera.Position.z <= -30.5) && (camera.Position.x >= 3.0 && camera.Position.x <= 4.0))
			SoundEngine->play2D("a.wav", GL_FALSE);

		//Transformations//
		glm::mat4 modelMatrix;
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		//
		glm::mat4 viewMatrix;
		viewMatrix = camera.getViewMatrix();
		//
		glm::mat4 projectionMatrix;
		projectionMatrix = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
		//////////////////////

		shaderProgram.updateMatrices(modelMatrix, viewMatrix, projectionMatrix);
		shaderProgram2.updateMatrices(modelMatrix, viewMatrix, projectionMatrix);
		shaderProgram3.updateMatrices(modelMatrix, viewMatrix, projectionMatrix);
		shaderProgram4.updateMatrices(modelMatrix, viewMatrix, projectionMatrix);


		//Rendering commands...
		glClearColor(0.2f, 0.3f, 0.3f, 0.0f);	//A state setting function.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			//A state using function. ^^

		//shaderProgram.setFloat("time", glfwGetTime());

		//float noisy = vnoise(100.0, 789.0, 138.0);
		//printf("%f\n", noisy);
		//shaderProgram.setFloat("randomPassed", noisy);

		//Bind texture.
		//glad_glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);	
		//glad_glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);
		//Then shader used on vertices.
		//glBindVertexArray(VAO);
		//First cube.
		//glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float) / 5);	//Used to draw an array of vertices.
		//glDrawElements(GL_TRIANGLES, (sizeof(indices) / sizeof(int)), GL_UNSIGNED_INT, 0);	//Draw using indices. Draw triangles referencing existing vertices.

		//glDrawArrays(GL_TRIANGLES, 0, sizeof(t.vertices) / sizeof(float) / 5);	//Used to draw an array of vertices.

		//////////////////
		//WORLD CREATION//
		glBindVertexArray(VAO);	

		//////////////////
		//Background sky//
		//////////////////
		//If you modifiy vertex shader to not apply view matrix so banner of sky stays on screen irrelevant of
		//view. But modifiy position coordinates passed to fragment shader which procedurally generates from these points so as view chnages the colours change.
		// Pos = vec4(aPos, 1.0) * viewMatrix; (Requires Pos be a vec 4)
		/*float sky[] = {
			20.5f, 0.5f, -10.5f, 0.0f, 1.0f,
			20.5f, 10.5f, -10.5f, 1.0f, 1.0f,
			-20.5f, 10.5f, -10.5f, 1.0f, 0.0f,
			-20.5f, 10.5f, -10.5f, 1.0f, 0.0f,
			-20.5f, 0.5f, -10.5f, 0.0f, 0.0f,
			20.5f, 0.5f, -10.5f, 0.0f, 1.0f
		};
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sky), sky, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(sky) / sizeof(float) / 5);*/

		t = Tile(EAST | WEST);
		placeTile(t, glm::vec3(0.0, 0.0, -1.0), 7, &modelMatrix, VBO, shaderProgram, texture1, texture2, texture3);

		t = Tile(SOUTH | WEST);
		placeTile(t, glm::vec3(0.0, 0.0, -1.0), 1, &modelMatrix, VBO, shaderProgram, texture1, texture2, texture3);

		t = Tile(NORTH | SOUTH);
		placeTile(t, glm::vec3(1.0, 0.0, 0.0), 7, &modelMatrix, VBO, shaderProgram, texture1, texture2, texture3);

		t = Tile(NORTH | EAST);
		placeTile(t, glm::vec3(1.0, 0.0, 0.0), 1, &modelMatrix, VBO, shaderProgram, texture1, texture2, texture3);

		t = Tile(WEST | EAST);
		placeTile(t, glm::vec3(0.0, 0.0, -1.0), 2, &modelMatrix, VBO, shaderProgram, texture1, texture2, texture3);

		t = Tile(SOUTH);
		placeTile(t, glm::vec3(0.0, 0.0, -1.0), 1, &modelMatrix, VBO, shaderProgram, texture1, texture2, texture3);

		t = Tile(NORTH | SOUTH);
		placeTile(t, glm::vec3(-1.0, 0.0, 0.0), 5, &modelMatrix, VBO, shaderProgram, texture1, texture2, texture3);

		t = Tile(NORTH | WEST | SOUTH);
		placeTile(t, glm::vec3(-1.0, 0.0, 0.0), 1, &modelMatrix, VBO, shaderProgram, texture1, texture2, texture3);

		modelMatrix = glm::translate(modelMatrix, glm::vec3(6.0f, 0.0f, 0.0f));
		//For movements outside placeTile function need this.
		currentPosition += glm::vec2(glm::vec3(6.0f, 0.0f, 0.0f).x, glm::vec3(6.0f, 0.0f, 0.0f).z);
		world.grid[tileCounter].position = currentPosition;

		t = Tile(NORTH | SOUTH);
		placeTile(t, glm::vec3(1.0, 0.0, 0.0), 5, &modelMatrix, VBO, shaderProgram, texture1, texture2, texture3);

		t = Tile(EAST);
		placeTile(t, glm::vec3(1.0, 0.0, 0.0), 1, &modelMatrix, VBO, shaderProgram, texture1, texture2, texture3);

		t = Tile(WEST | EAST);
		placeTile(t, glm::vec3(0.0, 0.0, 1.0), 2, &modelMatrix, VBO, shaderProgram, texture1, texture2, texture3);

		t = Tile(WEST | EAST | NORTH);
		placeTile(t, glm::vec3(0.0, 0.0, 1.0), 1, &modelMatrix, VBO, shaderProgram, texture1, texture2, texture3);

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
		//For movements outside placeTile function need this.
		currentPosition += glm::vec2(glm::vec3(0.0f, 0.0f, -3.0f).x, glm::vec3(0.0f, 0.0f, -3.0f).z);
		world.grid[tileCounter].position = currentPosition;

		t = Tile(WEST | EAST);
		placeTile(t, glm::vec3(0.0, 0.0, -1.0), 3, &modelMatrix, VBO, shaderProgram, texture1, texture2, texture3);

		t = Tile(EAST | SOUTH);
		placeTile(t, glm::vec3(0.0, 0.0, -1.0), 1, &modelMatrix, VBO, shaderProgram, texture1, texture2, texture3);

		t = Tile(NORTH | SOUTH);
		placeTile(t, glm::vec3(-1.0, 0.0, 0.0), 10, &modelMatrix, VBO, shaderProgram2, texture1, texture2, texture3);

		t = Tile(NORTH | WEST);
		placeTile(t, glm::vec3(-1.0, 0.0, 0.0), 1, &modelMatrix, VBO, shaderProgram2, texture1, texture2, texture3);

		t = Tile(EAST | WEST);
		placeTile(t, glm::vec3(0.0, 0.0, -1.0), 2, &modelMatrix, VBO, shaderProgram2, texture1, texture2, texture3);

		t = Tile(SOUTH);
		placeTile(t, glm::vec3(0.0, 0.0, -1.0), 1, &modelMatrix, VBO, shaderProgram3, texture1, texture2, texture3);

		t = Tile(NORTH | WEST);
		placeTile(t, glm::vec3(-1.0, 0.0, 0.0), 1, &modelMatrix, VBO, shaderProgram3, texture1, texture2, texture3);

		t = Tile(EAST | WEST);
		placeTile(t, glm::vec3(0.0, 0.0, -1.0), 2, &modelMatrix, VBO, shaderProgram3, texture1, texture2, texture3);

		t = Tile(SOUTH | WEST);
		placeTile(t, glm::vec3(0.0, 0.0, -1.0), 1, &modelMatrix, VBO, shaderProgram3, texture1, texture2, texture3);

		modelMatrix = glm::translate(modelMatrix, glm::vec3(1.0f, 0.0f, 3.0f));
		//For movements outside placeTile function need this.
		currentPosition += glm::vec2(glm::vec3(1.0f, 0.0f, 3.0f).x, glm::vec3(1.0f, 0.0f, 3.0f).z);
		world.grid[tileCounter].position = currentPosition;

		t = Tile(NORTH | EAST);
		placeTile(t, glm::vec3(1.0, 0.0, 0.0), 1, &modelMatrix, VBO, shaderProgram3, texture1, texture2, texture3);

		t = Tile(WEST | EAST);
		placeTile(t, glm::vec3(0.0, 0.0, -1.0), 2, &modelMatrix, VBO, shaderProgram3, texture1, texture2, texture3);

		t = Tile(SOUTH | EAST);
		placeTile(t, glm::vec3(0.0, 0.0, -1.0), 1, &modelMatrix, VBO, shaderProgram3, texture1, texture2, texture3);

		t = Tile(NORTH);
		placeTile(t, glm::vec3(-1.0, 0.0, 0.0), 1, &modelMatrix, VBO, shaderProgram3, texture1, texture2, texture3);

		t = Tile(EAST | WEST);
		placeTile(t, glm::vec3(0.0, 0.0, -1.0), 200, &modelMatrix, VBO, shaderProgram4, texture1, texture2, texture3);

		t = Tile(SOUTH | EAST | WEST);
		placeTile(t, glm::vec3(0.0, 0.0, -1.0), 1, &modelMatrix, VBO, shaderProgram4, texture1, texture2, texture3);
		////////////////////

		//Check call back events then swap buffers.
		glfwPollEvents();
		glfwSwapBuffers(window);

		//Caps framerate to 60 if higher by sleeping the time difference in rendering to actual render.
		clock_t endFrame = clock();
		clock_t renderFrame = endFrame - beginFrame;
		if (renderFrame < TIME_PER_FRAME_MICRO_SECONDS)
			Sleep(TIME_PER_FRAME_MICRO_SECONDS - renderFrame);

		//printGSR(sc);
	}

	//closeSerial(sc);
	//shaderProgram.setFloat("mixRate", mixRate);

	//Clearup.
	glfwTerminate();
	return 0;
}

GLFWwindow* windowInit()
{
	//Initialize then configure GLFW.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	//Mac OS X related.

	//Create the window we will be using.
	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Error: Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return window;
	}
	glfwMakeContextCurrent(window);		//Make window main context of current thread.
	return window;
}

//GLFW Callbacks.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Q)
	{
		if (action == GLFW_PRESS)
			fov = 30.0f;
		if (action == GLFW_RELEASE)
			fov = 90.0f;
	}
}
static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouseMovement)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouseMovement = false;
	}
	float xOffset = xPos - lastX;
	float yOffset = yPos - lastY;
	lastX = xPos;
	lastY = yPos;

	camera.processMouseMovement(xOffset, yOffset);
}

bool collision()
{
	bool safe = false;
	for (int i = 0; i != tileCounter; ++i)
	{
		if (camera.Position.x > world.grid[i].position.x - 0.5 && camera.Position.x < world.grid[i].position.x + 0.5)
		{
			if (camera.Position.z > world.grid[i].position.y - 0.5 && camera.Position.z < world.grid[i].position.y + 0.5)
			{
				safe = true;
			}
		}
	}
	return !safe;
}

void processMovement(GLFWwindow* window)
{
	bool moving = false;
	float moveSpeed = 0.01f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.processKeyboard(FORWARD, moveSpeed);
		if (collision())
			camera.processKeyboard(FORWARD, -moveSpeed);
		moving = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.processKeyboard(BACKWARD, moveSpeed);
		if (collision())
			camera.processKeyboard(BACKWARD, -moveSpeed);
		moving = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.processKeyboard(LEFT, moveSpeed);
		if (collision())
			camera.processKeyboard(LEFT, -moveSpeed);
		moving = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.processKeyboard(RIGHT, moveSpeed);
		if (collision())
			camera.processKeyboard(RIGHT, -moveSpeed);
		moving = true;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		camera.Position = glm::vec3(0.0, 0.0, -1.0);
	}
	if (moving)
		snd->setIsPaused(false);
	else
		snd->setIsPaused(true);
}

void processInput(GLFWwindow* window)
{
	int multi = 1;
	//Escape pressed.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		multi = 1000;
	}
	//Texture mixture.
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (mixRate <= 1.0f)
			mixRate += 0.01f;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			mixRate = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (mixRate >= 0.0f)
			mixRate -= 0.01f;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			mixRate = 0.0f;
	}
	processMovement(window);
}

//Draws same texture to all panels.
//Modifies the model matrix passed to it. So moves next tile from one just placed.
void placeTile(Tile tile, glm::vec3 translation, int numberTiles, glm::mat4* modelMatrix, int VBO, Shader shaderProgram, unsigned int texture)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tile.vertices), tile.vertices, GL_STATIC_DRAW);
	for (int i = 0; i != numberTiles; i++)
	{
		shaderProgram.setInt("tileID", tileCounter);
		glBindTexture(GL_TEXTURE_2D, texture);	//Bind the exisiting textures to be used when next drawn. (A state)
		*modelMatrix = glm::translate(*modelMatrix, translation);
		shaderProgram.setMatrix4X4("modelMatrix", *modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(tile.vertices) / sizeof(float) / 5);	//Used to draw an array of vertices.
		currentPosition += glm::vec2(translation.x, translation.z);
		world.grid[tileCounter].position = currentPosition;
		++tileCounter;
	}
}

//Draws textures to each panel.
void placeTile(Tile tile, glm::vec3 translation, int numberTiles, glm::mat4* modelMatrix, int VBO, Shader shaderProgram, unsigned int floor, unsigned int walls, unsigned int ceiling)
{
	shaderProgram.use();
	shaderProgram.setFloat("time", glfwGetTime());
	for (int j = 0; j != numberTiles; ++j)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		*modelMatrix = glm::translate(*modelMatrix, translation);
		shaderProgram.setMatrix4X4("modelMatrix", *modelMatrix);
		shaderProgram.setInt("tileID", tileCounter);

		drawPanel(tile, ceiling, FLOOR);
		drawPanel(tile, floor, CEILING);
		drawPanel(tile, walls, WALL1);
		drawPanel(tile, walls, WALL2);
		drawPanel(tile, walls, WALL3);
		drawPanel(tile, walls, WALL4);

		currentPosition += glm::vec2(translation.x, translation.z);
		world.grid[tileCounter].position = currentPosition;
		++tileCounter;
	}
}

//Draw passed texture on individual panel of tile. The chosen panel defined by starting at the given vertice.
void drawPanel(Tile tile, unsigned int texture, int vertice)
{
	float panel[30];
	for (int i = 0; i != 30; ++i)
	{
		panel[i] = tile.vertices[vertice + i];
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(panel), panel, GL_STATIC_DRAW);
	glBindTexture(GL_TEXTURE_2D, texture);	//Bind the exisiting textures to be used when next drawn. (A state)
	glDrawArrays(GL_TRIANGLES, 0, sizeof(panel) / sizeof(float) / 5);
}