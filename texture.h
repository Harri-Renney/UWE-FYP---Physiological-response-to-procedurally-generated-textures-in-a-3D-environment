#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>


struct Texture {
	unsigned char* data;
	int xElement;
	int xResolution;
	int yResolution;
};

int generate2DTextureProcedural(void(*brickTexture)(Texture), int xResolution, int yResolution);
int generateWhiteNoise(int xResolution, int yResolution);
int generateLatticeNoise(int xResolution, int yResolution);
int generateFractalsum(int xResolution, int yResolution);
int generateBrickTexture(int xResolution, int yResolution);
int generateCloudTexture(int xResolution, int yResolution);

#endif