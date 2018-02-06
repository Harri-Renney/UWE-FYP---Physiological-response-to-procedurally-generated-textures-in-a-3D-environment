#include "tile.h"

float maxTexCoord = 1.0;
const float minTexCoord = 0.0;

Tile::Tile(int walls)
{
	int vertexCounter = 0;
	for (int i = 0; i != 30; i++)
	{
		vertices[i] = Ceiling::vertices[i];
	}
	vertexCounter += 30;
	for (int i = 0; i != 30; i++)
	{
		vertices[i + vertexCounter] = Floor::vertices[i];
	}
	vertexCounter += 30;
	if (walls & WEST)
	{
		for (int i = 0; i != 30; i++)
		{
			vertices[i + vertexCounter] = West_Wall::vertices[i];
		}
		vertexCounter += 30;
	}
	if (walls & EAST)
	{
		for (int i = 0; i != 30; i++)
		{
			vertices[i + vertexCounter] = East_Wall::vertices[i];
		}
		vertexCounter += 30;
	}
	if (walls & NORTH)
	{
		for (int i = 0; i != 30; i++)
		{
			vertices[i + vertexCounter] = North_Wall::vertices[i];
		}
		vertexCounter += 30;
	}
	if (walls & SOUTH)
	{
		for (int i = 0; i != 30; i++)
		{
			vertices[i + vertexCounter] = South_Wall::vertices[i];
		}
		vertexCounter += 30;
	}

}

float North_Wall::vertices[30] = { 0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
0.5f, -0.5f, 0.5f, 0.0f, 1.0f };

float South_Wall::vertices[30] = { 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
-0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
-0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
0.5f, -0.5f, -0.5f, 0.0f, 1.0f };

float East_Wall::vertices[30] = { 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
0.5f, 0.5f, 0.5f, 1.0f, 0.0f };

float West_Wall::vertices[30] = { -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
-0.5f, 0.5f, 0.5f, 1.0f, 0.0f };

float Floor::vertices[30] = { -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f };

float Ceiling::vertices[30] = { -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f };