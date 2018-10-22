/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"

#define PI (float) 3.14159265358979323846

GzColor	*image=NULL;
int xs, ys;
int reset = 1;

inline float boundFloat(float value, int lowerBound, int upperBound) {
	float clampedValue = value;

	if (value < lowerBound)
		clampedValue = lowerBound;
	else if (value > upperBound)
		clampedValue = upperBound;

	return value;
}

inline int ARRAY(int x, int y) { return (x + y*xs); }	/* simplify fbuf indexing */

void bilinearInterpolation(float u, float v, GzColor color) {
	// Initialization
	GzColor A, B, C, D;

	// scale u, v to size of image xs, xy
	float texelX = u * (xs - 1);
	float texelY = v * (ys - 1);
	int x = floor(texelX);
	int y = floor(texelY);
	float s = texelX - x;
	float t = texelY - y;
	int texelIndex = ARRAY(x, y);

	A[RED] = image[texelIndex][RED];
	A[GREEN] = image[texelIndex][GREEN];
	A[BLUE] = image[texelIndex][BLUE];

	x = ceil(texelX);
	y = floor(texelY);
	texelIndex = ARRAY(x, y);
	B[RED] = image[texelIndex][RED];
	B[GREEN] = image[texelIndex][GREEN];
	B[BLUE] = image[texelIndex][BLUE];

	x = ceil(texelX);
	y = ceil(texelY);
	texelIndex = ARRAY(x, y);
	C[RED] = image[texelIndex][RED];
	C[GREEN] = image[texelIndex][GREEN];
	C[BLUE] = image[texelIndex][BLUE];

	x = floor(texelX);
	y = ceil(texelY);
	texelIndex = ARRAY(x, y);
	D[RED] = image[texelIndex][RED];
	D[GREEN] = image[texelIndex][GREEN];
	D[BLUE] = image[texelIndex][BLUE];

	// Color(p(u,v)) = s t C + (1-s) t D + s (1-t) B + (1-s) (1-t) A
	color[RED] = s * t * C[RED] + (1 - s) * t * D[RED] + s * (1 - t) * B[RED] + (1 - s) * (1 - t) * A[RED];
	color[GREEN] = s * t * C[GREEN] + (1 - s) * t * D[GREEN] + s * (1 - t) * B[GREEN] + (1 - s) * (1 - t) * A[GREEN];
	color[BLUE] = s * t * C[BLUE] + (1 - s) * t * D[BLUE] + s * (1 - t) * B[BLUE] + (1 - s) * (1 - t) * A[BLUE];
}

/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
	unsigned char		pixel[3];
	unsigned char     dummy;
	char  		foo[8];
	int   		i, j;
	FILE			*fd;

	if (reset) {          /* open and load texture file */
		fd = fopen ("texture", "rb");
		if (fd == NULL) {
			fprintf (stderr, "texture file not found\n");
			exit(-1);
		}
		fscanf (fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
		image = (GzColor*)malloc(sizeof(GzColor)*(xs+1)*(ys+1));
		if (image == NULL) {
			fprintf (stderr, "malloc for texture image failed\n");
			exit(-1);
		}

		for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
			fread(pixel, sizeof(pixel), 1, fd);
			image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
			image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
			image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
		}

		reset = 0;          /* init is done */
		fclose(fd);
	}

	/* bounds-test u,v to make sure nothing will overflow image array bounds */
	boundFloat(u, 0, 1);
	boundFloat(v, 0, 1);

	/* determine texture cell corner values and perform bilinear interpolation */
	/* set color to interpolated GzColor value and return */
	bilinearInterpolation(u, v, color);  

	return GZ_SUCCESS;
}

/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{
	// scale u, v to image size of 256 x 256
	u *= 256;
	v *= 256;

	double radius = sqrt((u*u + v*v));
	double angle;
	if (u == 0.0f) {
		angle = PI / 2.0;
	}
	else {
		angle = atan(v / u);
	}
	radius = radius + (2 * sin(20 * angle + v / 150));

	// Place radius of grain into range of [0, 59]
	double grain = (int)round(radius) % 60;

	// Pick a different color 2/3 of the time
	if (grain < 40) {
		color[RED] = 204;
		color[GREEN] = 153;
		color[BLUE] = 0;
	}
	else {
		color[RED] = 153;
		color[GREEN] = 102;
		color[BLUE] = 51;
	}

	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture()
{
	if(image!=NULL)
		free(image);
	return GZ_SUCCESS;
}

