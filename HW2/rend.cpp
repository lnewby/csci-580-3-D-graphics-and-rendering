#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

GzRender::GzRender(int xRes, int yRes)
{
	/* HW1.1 create a framebuffer for MS Windows display:
	-- set display resolution
	-- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
	-- allocate memory for pixel buffer
	*/
	// Store Props & set defaults
	xres = xRes;
	yres = yRes;
	pixelDepthInBytes = 3; // Storaging RGB values, 1 byte per color.
	totalPixels = xRes * yRes;

	int frameBufferSize = pixelDepthInBytes * totalPixels;
	framebuffer = new char[frameBufferSize];
	pixelbuffer = new GzPixel[totalPixels];
}

GzRender::~GzRender()
{
	/* HW1.2 clean up, free buffer memory */
	delete[] framebuffer;
	delete[] pixelbuffer;
}

int GzRender::GzDefault()
{
	/* HW1.3 set pixel buffer to some default values - start a new frame */
	for (unsigned int key = 0; key < totalPixels; key++) {
		pixelbuffer[key].red = 2110;
		pixelbuffer[key].green = 1970;
		pixelbuffer[key].blue = 1580;
		pixelbuffer[key].alpha = 1;
		pixelbuffer[key].z = MAXINT;
	}

	return GZ_SUCCESS;
}


int GzRender::GzPut(int pixelX, int pixelY, GzIntensity red, GzIntensity green, GzIntensity blue, GzIntensity alpha, GzDepth z)
{
	/* HW1.4 write pixel values into the buffer */
	int pixelIsOnScreen = (pixelX > -1 && pixelX < xres) && (pixelY > -1 && pixelY < yres);

	if (pixelIsOnScreen) {
		int pixelBufferIndex = ARRAY(pixelX, pixelY);
		pixelbuffer[pixelBufferIndex].red = clampColorRange(red);
		pixelbuffer[pixelBufferIndex].green = clampColorRange(green);
		pixelbuffer[pixelBufferIndex].blue = clampColorRange(blue);
		pixelbuffer[pixelBufferIndex].alpha = alpha;
		pixelbuffer[pixelBufferIndex].z = z;
	}

	return GZ_SUCCESS;
}


int GzRender::GzGet(int pixelX, int pixelY, GzIntensity *red, GzIntensity *green, GzIntensity *blue, GzIntensity *alpha, GzDepth *z)
{
	/* HW1.5 retrieve a pixel information from the pixel buffer */
	int pixelIsOnScreen = (pixelX > -1 && pixelX < xres) && (pixelY > -1 && pixelY < yres);

	if (pixelIsOnScreen) {
		int pixelBufferIndex = ARRAY(pixelX, pixelY);
		*red = pixelbuffer[pixelBufferIndex].red;
		*green = pixelbuffer[pixelBufferIndex].green;
		*blue = pixelbuffer[pixelBufferIndex].blue;
		*alpha = pixelbuffer[pixelBufferIndex].alpha;
		*z = pixelbuffer[pixelBufferIndex].z;
	}

	return GZ_SUCCESS;
}


int GzRender::GzFlushDisplay2File(FILE* outfile)
{
	/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */
	// Print ppm file header
	fprintf(outfile, "P6 %d %d 255\n", xres, yres);

	// Append all 8-bit binary pixel color values in raster order.
	for (unsigned int key = 0; key < totalPixels; key++) {
		unsigned char pixelColor[] = {
			convertGzIntensityTo8bitRGB(pixelbuffer[key].red),
			convertGzIntensityTo8bitRGB(pixelbuffer[key].green),
			convertGzIntensityTo8bitRGB(pixelbuffer[key].blue)
		};

		fwrite(
			pixelColor,
			sizeof(unsigned char),
			sizeof(pixelColor),
			outfile
		);
	}

	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2FrameBuffer()
{
	/* HW1.7 write pixels to framebuffer:
	- put the pixels into the frame buffer
	- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red
	- NOT red, green, and blue !!!
	*/
	for (unsigned int key = 0; key < totalPixels; key++) {
		framebuffer[key * pixelDepthInBytes + RED] = convertGzIntensityTo8bitRGB(pixelbuffer[key].blue);
		framebuffer[key * pixelDepthInBytes + GREEN] = convertGzIntensityTo8bitRGB(pixelbuffer[key].green);
		framebuffer[key * pixelDepthInBytes + BLUE] = convertGzIntensityTo8bitRGB(pixelbuffer[key].red);
	}

	return GZ_SUCCESS;
}

/***********************************************/
/* HW2 methods: implement from here */

int GzRender::GzPutAttribute(int numAttributes, GzToken	*nameList, GzPointer *valueList) 
{
/* HW 2.1
-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
-- In later homeworks set shaders, interpolaters, texture maps, and lights
*/
	for (unsigned int token = 0; token < numAttributes; token++) {
		float * color;

		switch (nameList[token]) {
		case GZ_RGB_COLOR:
			color = (float*)valueList[0];
			flatcolor[RED] = color[RED];
			flatcolor[GREEN] = color[GREEN];
			flatcolor[BLUE] = color[BLUE];
			break;
		default:
			break;
		}
	}

	return GZ_SUCCESS;
}

int GzRender::GzPutTriangle(int	numParts, GzToken *nameList, GzPointer *valueList) 
/* numParts - how many names and values */
{
/* HW 2.2
-- Pass in a triangle description with tokens and values corresponding to
      GZ_NULL_TOKEN:		do nothing - no values
      GZ_POSITION:		3 vert positions in model space
-- Invoke the rastrizer/scanline framework
-- Return error code
*/
	for (unsigned int token = 0; token < numParts; ++token) {
		switch (nameList[token]) {
		case GZ_POSITION:
			sortVerticesByY(valueList);
			createEdgeDDAs(valueList);
			sortEdgesAsLeftOrRight();
			break;
		case GZ_NULL_TOKEN:
			break;
		default:
			break;
		}
	}
	return GZ_SUCCESS;
}

void GzRender::createEdgeDDAs(GzPointer* valueList) {
	GzCoord * vertex = (GzCoord*) valueList[0];

	edgeDDA[one] = new DDA(vertex[one], vertex[two]);
	edgeDDA[two] = new DDA(vertex[two], vertex[three]);
	edgeDDA[three] = new DDA(vertex[one], vertex[three]);
};

void GzRender::swapVertices(GzCoord * vertex, int v1, int v2) {
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

void GzRender::sortVerticesByY(GzPointer* valueList) {
	GzCoord* vertex = (GzCoord*) valueList[0];

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

void GzRender::sortEdgesAsLeftOrRight() {
	
}