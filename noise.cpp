#include "noise.h"

#include <random>

static void valueTabInit(int seed);
static float vlattice3D(int ix, int iy, int iz);
static float vlattice2D(int ix, int iy);

static float valueTab[TABSIZE];
unsigned char perm[TABSIZE] = {
	225,155,210,108,175,199,221,144,203,116, 70,213, 69,158, 33,252,
	5, 82,173,133,222,139,174, 27,  9, 71, 90,246, 75,130, 91,191,
	169,138,  2,151,194,235, 81,  7, 25,113,228,159,205,253,134,142,
	248, 65,224,217, 22,121,229, 63, 89,103, 96,104,156, 17,201,129,
	36,  8,165,110,237,117,231, 56,132,211,152, 20,181,111,239,218,
	170,163, 51,172,157, 47, 80,212,176,250, 87, 49, 99,242,136,189,
	162,115, 44, 43,124, 94,150, 16,141,247, 32, 10,198,223,255, 72,
	53,131, 84, 57,220,197, 58, 50,208, 11,241, 28,  3,192, 62,202,
	18,215,153, 24, 76, 41, 15,179, 39, 46, 55,  6,128,167, 23,188,
	106, 34,187,140,164, 73,112,182,244,195,227, 13, 35, 77,196,185,
	26,200,226,119, 31,123,168,125,249, 68,183,230,177,135,160,180,
	12,  1,243,148,102,166, 38,238,251, 37,240,126, 64, 74,161, 40,
	184,149,171,178,101, 66, 29, 59,146, 61,254,107, 42, 86,154,  4,
	236,232,120, 21,233,209, 45, 98,193,114, 78, 19,206, 14,118,127,
	48, 79,147, 85, 30,207,219, 54, 88,234,190,122, 95, 67,143,109,
	137,214,145, 93, 92,100,245,  0,216,186, 60, 83,105, 97,204, 52
};

///////////////////////
//Universal Functions//
///////////////////////
float step(float a, float x)
{
	return (float)(x >= a);
}

float clamp(float x, float a, float b)
{
	return (x < a ? a : (x > b ? b : x));
}

float spline(float x, int nknots, float *knot)
{
	int span;
	int nspans = nknots - 3;
	float c0, c1, c2, c3;	/* coefficients of the cubic.*/

	if (nspans < 1) {  /* illegal */
		fprintf(stderr, "Spline has too few knots.\n");
		return 0;
	}

	/* Find the appropriate 4-point span of the spline. */
	x = clamp(x, 0, 1) * nspans;
	span = (int)x;
	if (span >= nknots - 3)
		span = nknots - 3;
	x -= span;
	knot += span;

	/* Evaluate the span cubic at x using Horner's rule. */
	c3 = CR00*knot[0] + CR01*knot[1]
		+ CR02*knot[2] + CR03*knot[3];
	c2 = CR10*knot[0] + CR11*knot[1]
		+ CR12*knot[2] + CR13*knot[3];
	c1 = CR20*knot[0] + CR21*knot[1]
		+ CR22*knot[2] + CR23*knot[3];
	c0 = CR30*knot[0] + CR31*knot[1]
		+ CR32*knot[2] + CR33*knot[3];

	return ((c3*x + c2)*x + c1)*x + c0;
}


/////////
//Noise//
/////////
float vnoise3D(float x, float y, float z)
{
	int ix, iy, iz;
	int i, j, k;
	float fx, fy, fz;
	float xknots[4], yknots[4], zknots[4];
	static int initialized = 0;

	if (!initialized) {
		valueTabInit(665);
		initialized = 1;
	}

	ix = FLOOR(x);
	fx = x - ix;

	iy = FLOOR(y);
	fy = y - iy;

	iz = FLOOR(z);
	fz = z - iz;

	for (k = -1; k <= 2; k++) {
		for (j = -1; j <= 2; j++) {
			for (i = -1; i <= 2; i++)
				xknots[i + 1] = vlattice3D(ix + i, iy + j, iz + k);
			yknots[j + 1] = spline(fx, 4, xknots);
		}
		zknots[k + 1] = spline(fy, 4, yknots);
	}
	return spline(fz, 4, zknots);
}

float vnoise2D(float x, float y)
{
	int ix, iy;
	int i, j, k;
	float fx, fy;
	float xknots[4], yknots[4];
	static int initialized = 0;

	if (!initialized) {
		valueTabInit(665);
		initialized = 1;
	}

	ix = FLOOR(x);
	fx = x - ix;

	iy = FLOOR(y);
	fy = y - iy;

	for (k = -1; k <= 2; k++) {
		for (j = -1; j <= 2; j++) {
			for (i = -1; i <= 2; i++)
				xknots[i + 1] = vlattice2D(ix + i, iy + j);
			yknots[j + 1] = spline(fx, 4, xknots);
		}
	}
	return spline(fy, 4, yknots);
}

//float perlinNoise(glm::vec3 P)
//{
//	glm::vec3 Pi0 = floor(P); // Integer part for indexing
//	glm::vec3 Pi1 = Pi0 + glm::vec3(1.0); // Integer part + 1
//	Pi0 = mod289(Pi0);
//	Pi1 = mod289(Pi1);
//	glm::vec3 Pf0 = fract(P); // Fractional part for interpolation
//	glm::vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
//	glm::vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
//	glm::vec4 iy = vec4(Pi0.yy, Pi1.yy);
//	glm::vec4 iz0 = Pi0.zzzz;
//	glm::vec4 iz1 = Pi1.zzzz;
//
//	vec4 ixy = permute(permute(ix) + iy);
//	vec4 ixy0 = permute(ixy + iz0);
//	vec4 ixy1 = permute(ixy + iz1);
//
//	vec4 gx0 = ixy0 * (1.0 / 7.0);
//	vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
//	gx0 = fract(gx0);
//	vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
//	vec4 sz0 = step(gz0, vec4(0.0));
//	gx0 -= sz0 * (step(0.0, gx0) - 0.5);
//	gy0 -= sz0 * (step(0.0, gy0) - 0.5);
//
//	vec4 gx1 = ixy1 * (1.0 / 7.0);
//	vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
//	gx1 = fract(gx1);
//	vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
//	vec4 sz1 = step(gz1, vec4(0.0));
//	gx1 -= sz1 * (step(0.0, gx1) - 0.5);
//	gy1 -= sz1 * (step(0.0, gy1) - 0.5);
//
//	vec3 g000 = vec3(gx0.x, gy0.x, gz0.x);
//	vec3 g100 = vec3(gx0.y, gy0.y, gz0.y);
//	vec3 g010 = vec3(gx0.z, gy0.z, gz0.z);
//	vec3 g110 = vec3(gx0.w, gy0.w, gz0.w);
//	vec3 g001 = vec3(gx1.x, gy1.x, gz1.x);
//	vec3 g101 = vec3(gx1.y, gy1.y, gz1.y);
//	vec3 g011 = vec3(gx1.z, gy1.z, gz1.z);
//	vec3 g111 = vec3(gx1.w, gy1.w, gz1.w);
//
//	vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
//	g000 *= norm0.x;
//	g010 *= norm0.y;
//	g100 *= norm0.z;
//	g110 *= norm0.w;
//	vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
//	g001 *= norm1.x;
//	g011 *= norm1.y;
//	g101 *= norm1.z;
//	g111 *= norm1.w;
//
//	float n000 = dot(g000, Pf0);
//	float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
//	float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
//	float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
//	float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
//	float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
//	float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
//	float n111 = dot(g111, Pf1);
//
//	vec3 fade_xyz = fade(Pf0);
//	vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
//	vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
//	float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
//	return 2.2 * n_xyz;
//}

///////////////////
//Local Functions//
///////////////////
static void valueTabInit(int seed)
{
	float *table = valueTab;
	int i;

	srand(seed);
	for (i = 0; i < TABSIZE; i++)
	{
		*table++ = 1. - 2.*RANDNBR;
	}
}

static float vlattice3D(int ix, int iy, int iz)
{
	return valueTab[INDEX3D(ix, iy, iz)];
}

static float vlattice2D(int ix, int iy)
{
	return valueTab[INDEX2D(ix, iy)];
}
