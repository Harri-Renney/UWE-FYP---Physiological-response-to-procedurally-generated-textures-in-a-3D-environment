//GLM (OpenGL Mathematics)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <random>

#include "texture.h"
#include "noise.h"

void whiteNoise(Texture texData);
void latticeNoise(Texture texData);
void fractalsum(Texture texData);
void cloudTexture(Texture texData);
void brickTexture(Texture texData);

//Generates a 2D texture ready to image loaded into.
int generate2DTexture()
{
	//Generating texture.
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	//Texture parameter setup.
	//Set wrapping parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Set filtering parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return textureID;
}

//Generate the array of ints that will make up a generated image.
int generate2DTextureProcedural(void(*texture)(Texture), int xResolution, int yResolution)
{
	Texture texData;
	texData.xResolution = xResolution;
	texData.yResolution = yResolution;
	texData.xElement = 3 * xResolution;	//An element on x axis considered 3 bytes as represents RGB.
	texData.data = new unsigned char[texData.xElement * texData.xResolution];	//Heap Allocation of texture data.

	unsigned int textureID = generate2DTexture();

	int width = xResolution, height = yResolution, nrChannels;

	(*texture)(texData);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	delete[] texData.data;
	return textureID;
}

////////////////
//White Noise //
////////////////
int generateWhiteNoise(int xResolution, int yResolution)
{
	return generate2DTextureProcedural(whiteNoise, xResolution, yResolution);
}
void whiteNoise(Texture texData)
{
	srand(1);
	for (int i = 0; i != texData.yResolution; ++i)
	{
		for (int j = 0; j != texData.xElement; ++j)
		{
			texData.data[i * texData.xElement + j] = rand();
			texData.data[i * texData.xElement + ++j] = rand();
			texData.data[i * texData.xElement + ++j] = rand();
		}
	}
}

//////////////////
//Lattice Noise //
//////////////////
int generateLatticeNoise(int xResolution, int yResolution)
{
	return generate2DTextureProcedural(latticeNoise, xResolution, yResolution);
}
void latticeNoise(Texture texData)
{
	for (int i = 0; i != texData.yResolution; ++i)
	{
		for (int j = 0; j != texData.xElement; ++j)
		{
			//Unsure whetehr to take the texture coords [0.0 - 1.0]
			//float latticeNoise = vnoise(j / (float)texData.xElement, i / (float)texData.yResolution, -0.5);
			//... or the world coordinates, integers.
			//If different resoltuon divide by difference. eg. 1920(original tex) / 16(This tex) = 120 (The devisor)
			float latticeNoise = vnoise2D(j, i) * 255;	//These are not actually world coordinates but still integers.
			texData.data[i * texData.xElement + j] = latticeNoise;
			texData.data[i * texData.xElement + ++j] = latticeNoise;
			texData.data[i * texData.xElement + ++j] = latticeNoise;
		}
	}
}

////////////////
//FRACTAL SUM //
////////////////
int generateFractalsum(int xResolution, int yResolution)
{
	return generate2DTextureProcedural(fractalsum, xResolution, yResolution);
}
void fractalsum(Texture texData)
{
	for (int i = 0; i != texData.yResolution; ++i)
	{
		for (int j = 0; j != texData.xElement; ++j)
		{
			//Several calls to noise function to build stochastic spectral function.
			float value = 0;
			for (int f = 1; f < 320; f *= 2)
			{
				value += vnoise2D((j / (float)texData.xElement)*f, (i / (float)texData.yResolution)*f) / f;
			}
			////////////////////////////////////////////////////////////////////////

			//Crazyness of it.
			int amplitude = 2;
			value *= amplitude;

			value *= 255;
			texData.data[i * texData.xElement + j] = value;
			texData.data[i * texData.xElement + ++j] = value;
			texData.data[i * texData.xElement + ++j] = value;
		}
	}
}

///////////
//BRICK //
//////////
int generateBrickTexture(int xResolution, int yResolution)
{
	return generate2DTextureProcedural(brickTexture, xResolution, yResolution);
}
void brickTexture(Texture texData)
{
	const float BRICKWIDTH = 0.25;
	const float BRICKHEIGHT = 0.08;
	const float MORTARTHICKNESS = 0.01;

	const float BMWIDTH = (BRICKWIDTH + MORTARTHICKNESS);
	const float BMHEIGHT = (BRICKHEIGHT + MORTARTHICKNESS);
	const float MWF = (MORTARTHICKNESS * 0.5 / BMWIDTH);
	const float MHF = (MORTARTHICKNESS * 0.5 / BMHEIGHT);


	const float ka = 1;
	const float kd = 1;
	const glm::vec3 Cbrick = glm::vec3(0.59, 0.09, 0.04);
	const glm::vec3 Cmortar = glm::vec3(0.5, 0.5, 0.5);

	for (int i = 0; i != texData.yResolution; ++i)
	{
		for (int j = 0; j != texData.xElement; ++j)
		{

			glm::vec3 Ct;
			//vec2 Nf;
			float ss, tt, sbrick, tbrick, w, h;
			float scoord = i / (float)texData.yResolution;
			float tcoord = j / (float)texData.xElement;

			//Nf = normalize(faceforward(N, I, 1);

			ss = scoord / BMWIDTH;
			tt = tcoord / BMHEIGHT;

			if (glm::mod(tt * 0.5, 1.0) > 0.5)
			{
				ss += 0.5;
			}

			sbrick = floor(ss);
			tbrick = floor(tt);
			ss -= sbrick;
			tt -= tbrick;

			w = step(MWF, ss) - step((float)(1.0 - MWF), ss);
			h = step(MHF, tt) - step((float)(1.0 - MHF), tt);

			Ct = glm::mix(Cmortar, Cbrick, w*h);

			//If mixing, comment this out.
			/*texData.data[i * texData.xElement + j] = Ct.r * 255;
			texData.data[i * texData.xElement + ++j] = Ct.g * 255;
			texData.data[i * texData.xElement + ++j] = Ct.b * 255;*/

			////////////////////////////////////////////////////////
			//Mix the texture already in data with the new values.//
			////////////////////////////////////////////////////////
			float temp = texData.data[i * texData.xElement + j] / 255.0;
			float temp2 = Ct.r;
			texData.data[i * texData.xElement + j] = glm::mix(temp, temp2, 0.85) * 255;
			temp = texData.data[i * texData.xElement + ++j] / 255.0;
			temp2 = Ct.g;
			texData.data[i * texData.xElement + j] = glm::mix(temp, temp2, 0.85) * 255;
			temp = texData.data[i * texData.xElement + ++j] / 255.0;
			temp2 = Ct.b;
			texData.data[i * texData.xElement + j] = glm::mix(temp, temp2, 0.85) * 255;
		}
	}

}

/////////
//CLOUD//
/////////
int generateCloudTexture(int xResolution, int yResolution)
{
	return generate2DTextureProcedural(cloudTexture, xResolution, yResolution);
}
void cloudTexture(Texture texData)
{
	const float PI = 3.1415926535897932384626433832795;
	const float NTERMS = 5.0;

	glm::vec3 Colour = glm::vec3(1.0, 1.0, 1.0);
	glm::vec3 Ct;
	glm::vec3 Psh;
	float i, amplitude, f;
	float x, fx, xfreq, xphase;
	float y, fy, yfreq, yphase;
	float offset = 0.5;
	float xoffset = 7.0;
	float yoffset = 96.0;

	//Something to do with alasing?
	//Psh = transform("shader", P);
	//x = xcomp(Psh) + xoffset;
	//y = ycomp(Psh) + yoffset;


	for (int i = 0; i != texData.yResolution; ++i)
	{
		for (int j = 0; j != texData.xElement; ++j)
		{
			x = j / (float)texData.xElement + xoffset;
			y = i / (float)texData.yResolution;

			xphase = 0.9; /* arbitrary */
			yphase = 0.7; /* arbitrary */
			xfreq = 2.0 * PI * 0.023;
			yfreq = 2.0 * PI * 0.051;
			amplitude = 0.3;
			f = 0.0;
			for (float i = 0.0; i < NTERMS; i += 1.0) {
				fx = amplitude *
					(offset + cos(xfreq * (x + xphase)));
				fy = amplitude *
					(offset + cos(yfreq * (y + yphase)));
				f += fx * fy;
				xphase = PI / 2.0 * 0.9 * cos(yfreq * y);
				yphase = PI / 2.0 * 1.1 * cos(xfreq * x);

				xfreq *= 1.9 + i * 0.1; /* approximately 2 */
				yfreq *= 2.2 - i * 0.08; /* approximately 2 */
				amplitude *= 0.907;
			}
			f = clamp(f, 0.0, 1.0);

			glm::vec3 Cs = glm::vec3(0.00784313725, 0.031372549, 0.356862745);
			Ct = mix(Cs, Colour, f);

			texData.data[i * texData.xElement + j] = Ct.r * 255;
			texData.data[i * texData.xElement + ++j] = Ct.g * 255;
			texData.data[i * texData.xElement + ++j] = Ct.b * 255;
		}

	}

}