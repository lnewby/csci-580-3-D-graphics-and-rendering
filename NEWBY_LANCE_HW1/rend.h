
#include	"gz.h"
#include <bitset>

#ifndef GZRENDER
class GzRender{			/* define a renderer */
  

public:
	// Attributes
	unsigned short	xres;			/* frame resolution */
	unsigned short	yres;
	GzPixel	*pixelbuffer;			/* pixel buffer array */
	char *framebuffer;				/* buffer for display */
	unsigned int pixelDepthInBytes; /* pixel depth for framebuffer */
	unsigned int totalPixels;		/* width x height */
	
	// Memory management
	GzRender(int xRes, int yRes);	/* Constructor - create object with correct buffer sizes */
	~GzRender();					/* Destructor - free memory */

	// Methods
	int GzDefault();
	int GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z);
	int GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth	*z);

	int GzFlushDisplay2File(FILE* outfile);
	int GzFlushDisplay2FrameBuffer();
	
	// Helper methods
	inline int ARRAY(int x, int y){return x+(y*xres);}	/* easy access to pixel index */
	
	inline GzIntensity clampColorRange(GzIntensity color) {
		GzIntensity clampedColor = color;

		if (color < 0) 
			clampedColor = 0;
		else if (color > 4095) 
			clampedColor = 4095;

		return clampedColor;
	}
	
	unsigned char convertGzIntensityTo8bitRGB(GzIntensity color) {
		return (unsigned char)( color >> 4 );
	}

};

#define GZRENDER
#endif

