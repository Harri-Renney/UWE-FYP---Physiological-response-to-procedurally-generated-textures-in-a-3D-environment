#ifndef NOISE_H
#define NOISE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TABSIZE	256
#define TABMASK	(TABSIZE-1)
#define PERM(x)	perm[(x)&TABMASK]
#define INDEX3D(ix,iy,iz)	PERM((ix)+PERM((iy)+PERM(iz)))
#define INDEX2D(ix,iy)	PERM((ix)+PERM((iy)))

#define RANDMASK  0x7fffffff
#define RANDNBR   ((rand() & 32767)/(double) 32767 )

#define FLOOR(x) ((int)(x) - ((x) < 0 && (x) != (int)(x)))
#define CEIL(x) ((int)(x) + ((x) > 0 && (x) != (int)(x)))
#define CLAMP(x,a,b) ((x) =< (a) ? (a) : ((x) >= (b) ? (b) : (x)))
#define LERP(t,x0,x1)  ((x0) + (t)*((x1)-(x0)))

#define PULSE(a,b,x) (step((a),(x)) - step((b),(x)))
#define boxstep(a,b,x) clamp(((x)-(a))/((b)-(a)),0,1)

/* Coefficients of basis matrix. */
#define CR00     -0.5
#define CR01      1.5
#define CR02     -1.5
#define CR03      0.5
#define CR10      1.0
#define CR11     -2.5
#define CR12      2.0
#define CR13     -0.5
#define CR20     -0.5
#define CR21      0.0
#define CR22      0.5
#define CR23      0.0
#define CR30      0.0
#define CR31      1.0
#define CR32      0.0
#define CR33      0.0

float step(float a, float x);
float clamp(float x, float a, float b);
float spline(float x, int nknots, float *knot);

float vnoise3D(float x, float y, float z);
float vnoise2D(float x, float y);

#endif