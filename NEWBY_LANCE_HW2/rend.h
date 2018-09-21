
#include	 "gz.h"
#include <stdio.h>
#ifndef GZRENDER_
#define GZRENDER_


/* Camera defaults */
#define	DEFAULT_FOV		35.0
#define	DEFAULT_IM_Z	(-10.0)  /* world coords for image plane origin */
#define	DEFAULT_IM_Y	(5.0)    /* default look-at point = 0,0,0 */
#define	DEFAULT_IM_X	(-10.0)

#define	DEFAULT_AMBIENT	{0.1, 0.1, 0.1}
#define	DEFAULT_DIFFUSE	{0.7, 0.6, 0.5}
#define	DEFAULT_SPECULAR	{0.2, 0.3, 0.4}
#define	DEFAULT_SPEC		32

#define	MATLEVELS	10		/* how many matrix pushes allowed */
#define	MAX_LIGHTS	10		/* how many lights allowed */

/* vertex object controls */
#define ASCEND 0
#define DESCEND 1

#define one   0
#define two	  1
#define three 2

/* Dummy definition : change it later */
#ifndef GzCamera
#define GzCamera	    	GzPointer
#endif

#ifndef GzLight
#define GzLight		GzPointer
#endif

#ifndef GzTexture
#define GzTexture	GzPointer
#endif

#ifndef GZ_DDA
typedef	struct {
	GzCoord start;
	GzCoord end;
	GzCoord current;
	float slopeX;
	float slopeZ;
	int edgeVertices[2];
	bool edgeOnLeft;
} GzDDA;
#define GZ_DDA
#endif;

using namespace std;

class GzRender{			/* define a renderer */
  

public:
	unsigned short	xres;
	unsigned short	yres;
	GzPixel		*pixelbuffer;		/* frame buffer array */
	char*	framebuffer;
	unsigned int pixelDepthInBytes; /* pixel depth for framebuffer */
	unsigned int totalPixels;		/* width x height */

	GzCamera		camera;
	short		matlevel;	/* top of stack - current xform */
	GzMatrix		Ximage[MATLEVELS];	/* stack of xforms (Xsm) */
	GzMatrix		Xnorm[MATLEVELS];	/* xform for norms (Xim) */
	GzMatrix		Xsp;		/* NDC to screen (pers-to-screen) */
	GzColor		flatcolor;    /* color state for flat shaded triangles */
	int			interp_mode;
	int			numlights;
	GzLight		lights[MAX_LIGHTS];
	GzLight		ambientlight;
	GzColor		Ka, Kd, Ks;
	float		spec;		/* specular power */
	GzTexture	tex_fun;  /* tex_fun(float u, float v, GzColor color) */
	GzDDA		edgeDDA[3];
	GzDDA		spanDDA;

	// Constructors
	GzRender(int xRes, int yRes);
	~GzRender();

	// HW1: Display methods
	int GzDefault();
	int GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z);
	int GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth	*z);

	int GzFlushDisplay2File(FILE* outfile);
	int GzFlushDisplay2FrameBuffer();

	// HW2: Render methods
	int GzPutAttribute(int numAttributes, GzToken *nameList, GzPointer *valueList);
	int GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList);
	
	// Extra methods: NOT part of API - just for general assistance */
	inline int ARRAY(int x, int y){return x+(y*xres);}	/* simplify fbuf indexing */
	inline short ctoi(float color) {return (short)((int)(color * ((1 << 12) - 1)));}		/* convert float color to GzIntensity short */

	inline float boundColor(float color) {
		float colorBound = 1.0;
		return (color <= colorBound) ? color : colorBound;
	}

	inline GzIntensity clampColorRange(GzIntensity color) {
		GzIntensity clampedColor = color;

		if (color < 0)
			clampedColor = 0;
		else if (color > 4095)
			clampedColor = 4095;

		return clampedColor;
	}

	unsigned char convertGzIntensityTo8bitRGB(GzIntensity color) {
		return (unsigned char)(color >> 4);
	}

	void swapVertices(GzCoord * vertex, int v1, int v2) {
		GzCoord tempVertex;
		tempVertex[X] = vertex[v1][X];
		tempVertex[Y] = vertex[v1][Y];
		tempVertex[Z] = vertex[v1][Z];
		vertex[v1][X] = vertex[v2][X];
		vertex[v1][Y] = vertex[v2][Y];
		vertex[v1][Z] = vertex[v2][Z];
		vertex[v2][X] = tempVertex[X];
		vertex[v2][Y] = tempVertex[Y];
		vertex[v2][Z] = tempVertex[Z];
	}

	void sortVerticesByY(GzPointer *valueList) {
		// TODO: Refactor to use mergesort
		GzCoord * vertex = (GzCoord*) valueList[0];		
		if (vertex[one][Y] > vertex[two][Y]) {
			swapVertices(vertex, one, two);
		}
		if (vertex[one][Y] > vertex[three][Y]) {
			swapVertices(vertex, one, three);
		}
		if (vertex[two][Y] > vertex[three][Y]) {
			swapVertices(vertex, two, three);
		}
	}

	float slope(GzCoord vertex1, GzCoord vertex2, int rise, int run) {
		bool notDivideByZero = vertex1[run] != vertex2[run];

		return (notDivideByZero)
			? (vertex2[rise] - vertex1[rise]) / (vertex2[run] - vertex1[run])
			: NULL;
	}

	float getXCoord(GzCoord vertex1, GzCoord vertex2, float givenY) {
		float Acoff = vertex2[Y] - vertex1[Y];
		float Bcoff = vertex1[X] - vertex2[X];
		float Ccoff = vertex2[X] * vertex1[Y] - vertex1[X] * vertex2[Y];
		float newXCoord = (-Ccoff - Bcoff * givenY) / Acoff;

		return newXCoord;
	}

	float getZCoord(GzCoord vertex1, GzCoord vertex2, float givenX) {
		float Acoff = vertex2[X] - vertex1[X];
		float Bcoff = vertex1[Z] - vertex2[Z];
		float Ccoff = vertex2[Z] * vertex1[X] - vertex1[Z] * vertex2[X];
		float newZCoord = (-Ccoff - Bcoff * givenX) / Acoff;

		return newZCoord;
	}

	void setupEdgeDDAs(GzPointer *valueList) {
		GzCoord * vertex = (GzCoord*)valueList[0];
		int totalEdges = 3;

		// set DDA edge vertices
		edgeDDA[one].edgeVertices[one] = one;
		edgeDDA[one].edgeVertices[two] = two;
		edgeDDA[two].edgeVertices[one] = two;
		edgeDDA[two].edgeVertices[two] = three;
		edgeDDA[three].edgeVertices[one] = one;
		edgeDDA[three].edgeVertices[two] = three;

		// complete edge DDA
		for (int edgeNum = 0; edgeNum < totalEdges; edgeNum++) {
			// set start
			int *vertNum = edgeDDA[edgeNum].edgeVertices;
			edgeDDA[edgeNum].start[X] = vertex[vertNum[one]][X];
			edgeDDA[edgeNum].start[Y] = vertex[vertNum[one]][Y];
			edgeDDA[edgeNum].start[Z] = vertex[vertNum[one]][Z];
			// set end
			edgeDDA[edgeNum].end[X] = vertex[vertNum[two]][X];
			edgeDDA[edgeNum].end[Y] = vertex[vertNum[two]][Y];
			edgeDDA[edgeNum].end[Z] = vertex[vertNum[two]][Z];
			// set current
			edgeDDA[edgeNum].current[X] = vertex[vertNum[one]][X];
			edgeDDA[edgeNum].current[Y] = vertex[vertNum[one]][Y];
			edgeDDA[edgeNum].current[Z] = vertex[vertNum[one]][Z];
			// set slope x
			edgeDDA[edgeNum].slopeX = slope(edgeDDA[edgeNum].start, edgeDDA[edgeNum].end, X, Y);
			// set slope z
			edgeDDA[edgeNum].slopeZ = slope(edgeDDA[edgeNum].start, edgeDDA[edgeNum].end, Z, Y);
		}
		
	}

	void sortEdgesLeftOrRight() {
		float calulatedX = getXCoord(edgeDDA[three].start, edgeDDA[three].end, edgeDDA[one].end[Y]);
		if (!edgeDDA[one].slopeX) { // check if top of triangle has 0 slope
			if (edgeDDA[one].start[X] < edgeDDA[one].end[X]) {
				edgeDDA[two].edgeOnLeft = false;
				edgeDDA[three].edgeOnLeft = true;
			} else {
				edgeDDA[two].edgeOnLeft = true;
				edgeDDA[three].edgeOnLeft = false;
			}
		} else if (!edgeDDA[two].slopeX) { // check if base of triangle has 0 slope
			if (edgeDDA[two].start[X] < edgeDDA[two].end[X]) {
				edgeDDA[one].edgeOnLeft = true;
				edgeDDA[three].edgeOnLeft = false;
			}
			else {
				edgeDDA[one].edgeOnLeft = false;
				edgeDDA[three].edgeOnLeft = true;
			}
		} else if (calulatedX < edgeDDA[one].end[X]) {
			edgeDDA[one].edgeOnLeft = false;
			edgeDDA[two].edgeOnLeft = false;
			edgeDDA[three].edgeOnLeft = true;
		} else {
			edgeDDA[one].edgeOnLeft = true;
			edgeDDA[two].edgeOnLeft = true;
			edgeDDA[three].edgeOnLeft = false;
		}
	}

	void advanceSpan() {
		spanDDA.current[X] = spanDDA.start[X];
		spanDDA.current[Y] = spanDDA.start[Y];
		spanDDA.current[Z] = spanDDA.start[Z];

		// Advance span current position to left - most covered pixel(ceiling)
		float newX = ceil(spanDDA.start[X]);
		spanDDA.current[X] = newX;
		spanDDA.current[Z] = getZCoord(spanDDA.start, spanDDA.end, newX);

		// current triangle color
		GzIntensity red = ctoi(flatcolor[RED]);
		GzIntensity green = ctoi(flatcolor[GREEN]);
		GzIntensity blue = ctoi(flatcolor[BLUE]);

		// Interpolate span position and parameters (Z) until current position > end
		while (spanDDA.current[X] <= spanDDA.end[X]) {
			// Test interpolated-Z against FB-Z for each pixel - low Z wins
			int currentPixelIndex = ARRAY(spanDDA.current[X], spanDDA.current[Y]);
			if (currentPixelIndex < totalPixels && spanDDA.current[Z] < pixelbuffer[currentPixelIndex].z) {
				GzPut(spanDDA.current[X], spanDDA.current[Y], red, green, blue, 1, spanDDA.current[Z]);
			}

			// Advance span current position
			newX += 1;
			spanDDA.current[X] = newX;
			spanDDA.current[Z] = getZCoord(spanDDA.start, spanDDA.end, newX);
		}
	}

	void advanceEdges() {
		// Advance (1-2) and (1-3) DDA current positions to top y-scan line (ceiling)
		float newY = ceil(edgeDDA[one].start[Y]);
		edgeDDA[one].current[X] = getXCoord(edgeDDA[one].start, edgeDDA[one].end, newY);
		edgeDDA[one].current[Y] = newY;
		edgeDDA[one].current[Z] = getZCoord(edgeDDA[one].start, edgeDDA[one].end, edgeDDA[one].current[X]);
		edgeDDA[three].current[X] = getXCoord(edgeDDA[three].start, edgeDDA[three].end, newY);
		edgeDDA[three].current[Y] = newY;
		edgeDDA[three].current[Z] = getZCoord(edgeDDA[three].start, edgeDDA[three].end, edgeDDA[three].current[X]);

		// Switch from 1 - 2 edge to 2 - 3 edge when current 1 - 2 edge position > Y(2)	
		while (edgeDDA[one].current[Y] <= edgeDDA[one].end[Y]) {
			// Setup span DDA on successive lines based on edge DDA position values
			// Set span DDA current and end positions to right and left edge values 
			spanDDA.start[X] = edgeDDA[one].edgeOnLeft ? edgeDDA[one].current[X] : edgeDDA[three].current[X];
			spanDDA.start[Y] = edgeDDA[one].edgeOnLeft ? edgeDDA[one].current[Y] : edgeDDA[three].current[Y];
			spanDDA.start[Z] = edgeDDA[one].edgeOnLeft ? edgeDDA[one].current[Z] : edgeDDA[three].current[Z];

			spanDDA.end[X] = edgeDDA[one].edgeOnLeft ? edgeDDA[three].current[X] : edgeDDA[one].current[X];
			spanDDA.end[Y] = edgeDDA[one].edgeOnLeft ? edgeDDA[three].current[Y] : edgeDDA[one].current[Y];
			spanDDA.end[Z] = edgeDDA[one].edgeOnLeft ? edgeDDA[three].current[Z] : edgeDDA[one].current[Z];

			advanceSpan();

			// Advance (1-2) and (1-3) DDA current positions
			newY += 1;
			edgeDDA[one].current[X] = getXCoord(edgeDDA[one].start, edgeDDA[one].end, newY);
			edgeDDA[one].current[Y] = newY;
			edgeDDA[one].current[Z] = getZCoord(edgeDDA[one].start, edgeDDA[one].end, edgeDDA[one].current[X]);

			edgeDDA[three].current[X] = getXCoord(edgeDDA[three].start, edgeDDA[three].end, newY);
			edgeDDA[three].current[Y] = newY;
			edgeDDA[three].current[Z] = getZCoord(edgeDDA[three].start, edgeDDA[three].end, edgeDDA[three].current[X]);
		}

		// Advance (2-3) and (1-3) DDA current positions to top y-scan line (ceiling)
		newY = ceil(edgeDDA[two].start[Y]);
		edgeDDA[two].current[X] = getXCoord(edgeDDA[two].start, edgeDDA[two].end, newY);
		edgeDDA[two].current[Y] = newY;
		edgeDDA[two].current[Z] = getZCoord(edgeDDA[two].start, edgeDDA[two].end, edgeDDA[two].current[X]);
		edgeDDA[three].current[X] = getXCoord(edgeDDA[three].start, edgeDDA[three].end, newY);
		edgeDDA[three].current[Y] = newY;
		edgeDDA[three].current[Z] = getZCoord(edgeDDA[three].start, edgeDDA[three].end, edgeDDA[three].current[X]);

		// End advance when current 2 - 3 edge position > Y(3)	
		while (edgeDDA[two].current[Y] <= edgeDDA[two].end[Y]) {
			// Setup span DDA on successive lines based on edge DDA position values
			// Set span DDA current and end positions to right and left edge values 
			spanDDA.start[X] = edgeDDA[two].edgeOnLeft ? edgeDDA[two].current[X] : edgeDDA[three].current[X];
			spanDDA.start[Y] = edgeDDA[two].edgeOnLeft ? edgeDDA[two].current[Y] : edgeDDA[three].current[Y];
			spanDDA.start[Z] = edgeDDA[two].edgeOnLeft ? edgeDDA[two].current[Z] : edgeDDA[three].current[Z];

			spanDDA.end[X] = edgeDDA[two].edgeOnLeft ? edgeDDA[three].current[X] : edgeDDA[two].current[X];
			spanDDA.end[Y] = edgeDDA[two].edgeOnLeft ? edgeDDA[three].current[Y] : edgeDDA[two].current[Y];
			spanDDA.end[Z] = edgeDDA[two].edgeOnLeft ? edgeDDA[three].current[Z] : edgeDDA[two].current[Z];

			advanceSpan();

			// Advance (1-2) and (1-3) DDA current positions
			newY += 1;
			edgeDDA[two].current[X] = getXCoord(edgeDDA[two].start, edgeDDA[two].end, newY);
			edgeDDA[two].current[Y] = newY;
			edgeDDA[two].current[Z] = getZCoord(edgeDDA[two].start, edgeDDA[two].end, edgeDDA[two].current[X]);

			edgeDDA[three].current[X] = getXCoord(edgeDDA[three].start, edgeDDA[three].end, newY);
			edgeDDA[three].current[Y] = newY;
			edgeDDA[three].current[Z] = getZCoord(edgeDDA[three].start, edgeDDA[three].end, edgeDDA[three].current[X]);
		}

	}
};

#endif

