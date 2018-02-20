#ifndef TILE_H
#define TILE_H

enum Walls {
	NORTH = 1,
	EAST = 2,
	SOUTH = 4,
	WEST = 8
};

enum Panels {
	FLOOR = 0,
	CEILING = 30,
	WALL1 = 60,
	WALL2 = 90,
	WALL3 = 120,
	WALL4 = 150
};


class Tile{
public:
	float vertices[180];
	Tile();
	Tile(int walls);
private:
};

class North_Wall{
public:
	static float vertices[30];
};

class South_Wall{
public:
	static float vertices[30];
};

class East_Wall{
public:
	static float vertices[30];
};

class West_Wall{
public:
	static float vertices[30];
};

class Ceiling{
public:
	static float vertices[30];
};

class Floor{
public:
	static float vertices[30];
};


/*
float vertices[] = {

	-0.5f, 0.5f, 0.5f, maxTexCoord, 0.0f,
	-0.5f, 0.5f, -0.5f, maxTexCoord, maxTexCoord,
	-0.5f, -0.5f, -0.5f, 0.0f, maxTexCoord,
	-0.5f, -0.5f, -0.5f, 0.0f, maxTexCoord,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, maxTexCoord, 0.0f,

	0.5f, 0.5f, 0.5f, maxTexCoord, 0.0f,
	0.5f, 0.5f, -0.5f, maxTexCoord, maxTexCoord,
	0.5f, -0.5f, -0.5f, 0.0f, maxTexCoord,
	0.5f, -0.5f, -0.5f, 0.0f, maxTexCoord,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, maxTexCoord, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, maxTexCoord,
	0.5f, -0.5f, -0.5f, maxTexCoord, maxTexCoord,
	0.5f, -0.5f, 0.5f, maxTexCoord, 0.0f,
	0.5f, -0.5f, 0.5f, maxTexCoord, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, maxTexCoord,

	-0.5f, 0.5f, -0.5f, 0.0f, maxTexCoord,
	0.5f, 0.5f, -0.5f, maxTexCoord, maxTexCoord,
	0.5f, 0.5f, 0.5f, maxTexCoord, 0.0f,
	0.5f, 0.5f, 0.5f, maxTexCoord, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, maxTexCoord
};
*/

#endif