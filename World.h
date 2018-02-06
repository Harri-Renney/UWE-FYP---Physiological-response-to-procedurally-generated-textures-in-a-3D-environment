#ifndef WORLD_H
#define WORLD_H

//Extension wrangler (glad) opengl loading library + Window/Context creation (glfw) opengl library framework
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//GLM (OpenGL Mathematics)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Square
{
	glm::vec2 position;
};

struct World
{
	//We need like a grid of coordinates in here.
	Square grid[10000];
};

#endif