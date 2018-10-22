#include	"gz.h"
#ifndef GZRENDER_
#define GZRENDER_

/* Camera defaults */
#define	DEFAULT_FOV	35.0
/* world coords for image plane origin */
#define	DEFAULT_IM_Z	(-10.0)  
#define	DEFAULT_IM_Y	(5.0)    
#define	DEFAULT_IM_X	(-10.0)
/* default look-at point = 0,0,0 */

#define	DEFAULT_AMBIENT	{0.1, 0.1, 0.1}
#define	DEFAULT_DIFFUSE	{0.7, 0.6, 0.5}
#define	DEFAULT_SPECULAR	{0.2, 0.3, 0.4}
#define	DEFAULT_SPEC		32

#define	MATLEVELS	100	/* how many matrix pushes allowed */
#define	MAX_LIGHTS	20	/* how many lights allowed */

#define one   0
#define two	  1
#define three 2

#ifndef GZ_DDA
typedef	struct {
	GzCoord start;;
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
	char* framebuffer;
	unsigned int pixelDepthInBytes; /* pixel depth for framebuffer */
	unsigned int totalPixels;		/* width x height */

	GzCamera		m_camera;
	short		    matlevel;	        /* top of stack - current xform */
	GzMatrix		Ximage[MATLEVELS];	/* stack of xforms (Xsm) */
	GzMatrix		Xnorm[MATLEVELS];	/* xforms for norms (Xim) */
	GzMatrix		Xsp;		        /* NDC to screen (pers-to-screen) */
	GzColor		flatcolor;          /* color state for flat shaded triangles */
	GzColor		C;		/* color state for each triangles vertex computed by shading equation */
	GzColor		vertcolor[3];
	int			interp_mode;
	int			numlights;
	GzLight		lights[MAX_LIGHTS];
	GzLight		ambientlight;
	GzColor		Ka, Kd, Ks;
	float		    spec;		/* specular power */
	GzTexture		tex_fun;    /* tex_fun(float u, float v, GzColor color) */
	GzDDA		edgeDDA[3];
	GzDDA		spanDDA;
	GzTextureIndex perspectiveUV[3]; /* uv at each triangle vertice in perspective space */

  	// Constructors
	GzRender(int xRes, int yRes);
	~GzRender();

	// Function declaration

	// HW1: Display methods
	int GzDefault();
	int GzBeginRender();
	int GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z);
	int GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth	*z);

	int GzFlushDisplay2File(FILE* outfile);
	int GzFlushDisplay2FrameBuffer();

	// HW2: Render methods
	int GzPutAttribute(int numAttributes, GzToken *nameList, GzPointer *valueList);
	int GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList);

	// HW3
	int GzPutCamera(GzCamera camera);
	int GzPushMatrix(GzMatrix	matrix);
	int GzPopMatrix();

	// Extra methods: NOT part of API - just for general assistance */
	inline int ARRAY(int x, int y){return (x+y*xres);}	/* simplify fbuf indexing */
	inline short	ctoi(float color) {return(short)((int)(color * ((1 << 12) - 1)));}		/* convert float color to GzIntensity short */


	// Object Translation
	int GzRotXMat(float degree, GzMatrix mat);
	int GzRotYMat(float degree, GzMatrix mat);
	int GzRotZMat(float degree, GzMatrix mat);
	int GzTrxMat(GzCoord translate, GzMatrix mat);
	int GzScaleMat(GzCoord scale, GzMatrix mat);

	// Homework 2
	void setupEdgeDDAs(GzPointer *valueList);
	void sortEdgesLeftOrRight();
	void advanceSpan(GzPointer *valueList);
	void advanceEdges(GzPointer *valueList);
	void scanLineRender(GzPointer *valueList);

	// Homework 3
	float degreesToRadians(float degree);
	void Multiply4x4(GzMatrix mat1, GzMatrix mat2);

	// Homework 4
	void shadingEquation(GzCoord normal);
	
	inline float dotProduct(GzCoord m, GzCoord n) {
		return m[0] * n[0] + m[1] * n[1] + m[2] * n[2];
	}

	inline float boundFloat(float value, int lowerBound, int upperBound) {
		float clampedValue = value;
		
		if (value < lowerBound)
			clampedValue = lowerBound;
		else if (value > upperBound)
			clampedValue = upperBound;

		return value;
	}

	inline GzIntensity clampColorRange(GzIntensity color) {
		GzIntensity clampedColor = color;

		if (color < 0)
			clampedColor = 0;
		else if (color > 4095)
			clampedColor = 4095;

		return clampedColor;
	}

	inline unsigned char convertGzIntensityTo8bitRGB(GzIntensity color) {
		return (unsigned char)(color >> 4);
	}

	inline void swapVertices(GzCoord *vertex, int v1, int v2) {
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

	inline void swapUV(GzTextureIndex *uv, int v1, int v2) {
		GzTextureIndex tempUV;
		tempUV[U] = uv[v1][U];
		tempUV[V] = uv[v1][V];
		uv[v1][U] = uv[v2][U];
		uv[v1][V] = uv[v2][V];
		uv[v2][U] = tempUV[U];
		uv[v2][V] = tempUV[V];
	}

	inline void sortVerticesByY(GzPointer *valueList) {
		GzCoord * vertex = (GzCoord*)valueList[0];
		GzCoord * normal = (GzCoord*)valueList[1];
		GzTextureIndex * uv = (GzTextureIndex*)valueList[2];
		if (vertex[one][Y] > vertex[two][Y]) {
			swapVertices(vertex, one, two);
			swapVertices(normal, one, two);
			swapUV(uv, one, two);
		}
		if (vertex[one][Y] > vertex[three][Y]) {
			swapVertices(vertex, one, three);
			swapVertices(normal, one, three);
			swapUV(uv, one, three);
		}
		if (vertex[two][Y] > vertex[three][Y]) {
			swapVertices(vertex, two, three);
			swapVertices(normal, two, three);
			swapUV(uv, two, three);
		}
	}

	inline float slope(GzCoord vertex1, GzCoord vertex2, int rise, int run) {
		bool notDivideByZero = vertex1[run] != vertex2[run];

		return (notDivideByZero)
			? (vertex2[rise] - vertex1[rise]) / (vertex2[run] - vertex1[run])
			: NULL;
	}

	inline float getXCoord(GzCoord vertex1, GzCoord vertex2, float givenY) {
		float Acoeff = vertex2[Y] - vertex1[Y];
		float Bcoeff = vertex1[X] - vertex2[X];
		float Ccoeff = vertex2[X] * vertex1[Y] - vertex1[X] * vertex2[Y];
		float newXCoord = (-Ccoeff - Bcoeff * givenY) / Acoeff;

		return newXCoord;
	}

	inline float getZCoord(GzCoord vertex1, GzCoord vertex2, float givenX) {
		float Acoeff = vertex2[X] - vertex1[X];
		float Bcoeff = vertex1[Z] - vertex2[Z];
		float Ccoeff = vertex2[Z] * vertex1[X] - vertex1[Z] * vertex2[X];
		float newZCoord = (-Ccoeff - Bcoeff * givenX) / Acoeff;

		return newZCoord;
	}

	inline void normalizeVector(GzCoord vector) {
		float magnitude = sqrt((vector[X] * vector[X]) + (vector[Y] * vector[Y]) + (vector[Z] * vector[Z]));

		vector[X] /= magnitude;
		vector[Y] /= magnitude;
		vector[Z] /= magnitude;
	};

	inline void setToIdentity(GzMatrix matrix) {
		matrix[0][0] = 1.0; matrix[0][1] = 0.0; matrix[0][2] = 0.0; matrix[0][3] = 0.0;
		matrix[1][0] = 0.0; matrix[1][1] = 1.0; matrix[1][2] = 0.0; matrix[1][3] = 0.0;
		matrix[2][0] = 0.0; matrix[2][1] = 0.0; matrix[2][2] = 1.0; matrix[2][3] = 0.0;
		matrix[3][0] = 0.0; matrix[3][1] = 0.0; matrix[3][2] = 0.0; matrix[3][3] = 1.0;
	}

	inline void removeTranslation(GzMatrix matrix) {
		matrix[0][3] = 0.0;
		matrix[1][3] = 0.0;
		matrix[2][3] = 0.0;
	}

	inline void removeScale(GzMatrix matrix) {
		float scaleFactor;

		for (unsigned int i = 0; i < 3; ++i) {
			scaleFactor = sqrt(matrix[0][i] * matrix[0][i] + matrix[1][i] * matrix[1][i] + matrix[2][i] * matrix[2][i]);

			for (unsigned int j = 0; j < 3; ++j) {
				matrix[j][i] /= scaleFactor;
			}
		}
	}

	inline void transpose(GzMatrix m) {
		GzMatrix temp = {
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]
		};

		for (unsigned int i = 0; i < 4; ++i) {
			for (unsigned int j = 0; j < 4; ++j) {
				m[i][j] = temp[i][j];
			}
		}
	}
};
#endif