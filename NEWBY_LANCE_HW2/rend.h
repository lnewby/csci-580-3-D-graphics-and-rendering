#include "gz.h"
#include  <stdio.h>
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

	inline void swapVertices(GzCoord * vertex, int v1, int v2) {
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

	inline void sortVerticesByY(GzPointer *valueList) {
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

	inline float slope(GzCoord vertex1, GzCoord vertex2, int rise, int run) {
		bool notDivideByZero = vertex1[run] != vertex2[run];

		return (notDivideByZero)
			? (vertex2[rise] - vertex1[rise]) / (vertex2[run] - vertex1[run])
			: NULL;
	}

	inline float getXCoord(GzCoord vertex1, GzCoord vertex2, float givenY) {
		float Acoff = vertex2[Y] - vertex1[Y];
		float Bcoff = vertex1[X] - vertex2[X];
		float Ccoff = vertex2[X] * vertex1[Y] - vertex1[X] * vertex2[Y];
		float newXCoord = (-Ccoff - Bcoff * givenY) / Acoff;

		return newXCoord;
	}

	inline float getZCoord(GzCoord vertex1, GzCoord vertex2, float givenX) {
		float Acoff = vertex2[X] - vertex1[X];
		float Bcoff = vertex1[Z] - vertex2[Z];
		float Ccoff = vertex2[Z] * vertex1[X] - vertex1[Z] * vertex2[X];
		float newZCoord = (-Ccoff - Bcoff * givenX) / Acoff;

		return newZCoord;
	}

	void setupEdgeDDAs(GzPointer *valueList);

	void sortEdgesLeftOrRight();

	void advanceSpan();

	void advanceEdges();
};

#endif

