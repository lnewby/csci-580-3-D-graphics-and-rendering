/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

#define PI (float) 3.14159265358979323846

int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
/* HW 3.1
// Create rotate matrix : rotate along x axis
// Pass back the matrix using mat value
*/

	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
/* HW 3.2
// Create rotate matrix : rotate along y axis
// Pass back the matrix using mat value
*/

	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
/* HW 3.3
// Create rotate matrix : rotate along z axis
// Pass back the matrix using mat value
*/

	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
/* HW 3.4
// Create translation matrix
// Pass back the matrix using mat value
*/

	return GZ_SUCCESS;
}


int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
/* HW 3.5
// Create scaling matrix
// Pass back the matrix using mat value
*/

	return GZ_SUCCESS;
}


GzRender::GzRender(int xRes, int yRes)
{
/* HW1.1 create a framebuffer for MS Windows display:
 -- set display resolution
 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
 -- allocate memory for pixel buffer
 */
	xres = xRes;
	yres = yRes;
	pixelDepthInBytes = 3; // Storaging RGB values, 1 byte per color.
	totalPixels = xRes * yRes;

	int frameBufferSize = pixelDepthInBytes * totalPixels;
	framebuffer = new char[frameBufferSize];
	pixelbuffer = new GzPixel[totalPixels];

/* HW 3.6
- setup Xsp and anything only done once 
- init default camera 
*/ 
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
		pixelbuffer[key].red = 3000;
		pixelbuffer[key].green = 1500;
		pixelbuffer[key].blue = 500;
		pixelbuffer[key].alpha = 1;
		pixelbuffer[key].z = MAXINT;
	}

	return GZ_SUCCESS;
}

int GzRender::GzBeginRender()
{
/* HW 3.7 
- setup for start of each frame - init frame buffer color,alpha,z
- compute Xiw and projection xform Xpi from camera definition 
- init Ximage - put Xsp at base of stack, push on Xpi and Xiw 
- now stack contains Xsw and app can push model Xforms when needed 
*/ 

	return GZ_SUCCESS;
}

int GzRender::GzPutCamera(GzCamera camera)
{
/* HW 3.8 
/*- overwrite renderer camera structure with new camera definition
*/

	return GZ_SUCCESS;	
}

int GzRender::GzPushMatrix(GzMatrix	matrix)
{
/* HW 3.9 
- push a matrix onto the Ximage stack
- check for stack overflow
*/
	
	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
/* HW 3.10
- pop a matrix off the Ximage stack
- check for stack underflow
*/

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
			flatcolor[RED] = boundColor(color[RED]);
			flatcolor[GREEN] = boundColor(color[GREEN]);
			flatcolor[BLUE] = boundColor(color[BLUE]);
			break;
		default:
			break;
		}
	}

	return GZ_SUCCESS;
}

int GzRender::GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList)
/* numParts - how many names and values */
{
/* HW 2.2
-- Pass in a triangle description with tokens and values corresponding to
      GZ_NULL_TOKEN:		do nothing - no values
      GZ_POSITION:		3 vert positions in model space
-- Invoke the rastrizer/scanline framework
-- Return error code
*/
	for (unsigned int token = 0; token < numParts; token++) {
		switch (nameList[token]) {
		case GZ_POSITION:
			sortVerticesByY(valueList);
			setupEdgeDDAs(valueList);
			sortEdgesLeftOrRight();
			advanceEdges();
			break;
		case GZ_NULL_TOKEN:
			break;
		default:
			break;
		}
	}

	return GZ_SUCCESS;
}

void GzRender::setupEdgeDDAs(GzPointer *valueList) {
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

void GzRender::sortEdgesLeftOrRight() {
	float calulatedX = getXCoord(edgeDDA[three].start, edgeDDA[three].end, edgeDDA[one].end[Y]);
	if (!edgeDDA[one].slopeX) { // check if top of triangle has 0 slope
		if (edgeDDA[one].start[X] < edgeDDA[one].end[X]) {
			edgeDDA[two].edgeOnLeft = false;
			edgeDDA[three].edgeOnLeft = true;
		}
		else {
			edgeDDA[two].edgeOnLeft = true;
			edgeDDA[three].edgeOnLeft = false;
		}
	}
	else if (!edgeDDA[two].slopeX) { // check if base of triangle has 0 slope
		if (edgeDDA[two].start[X] < edgeDDA[two].end[X]) {
			edgeDDA[one].edgeOnLeft = true;
			edgeDDA[three].edgeOnLeft = false;
		}
		else {
			edgeDDA[one].edgeOnLeft = false;
			edgeDDA[three].edgeOnLeft = true;
		}
	}
	else if (calulatedX < edgeDDA[one].end[X]) {
		edgeDDA[one].edgeOnLeft = false;
		edgeDDA[two].edgeOnLeft = false;
		edgeDDA[three].edgeOnLeft = true;
	}
	else {
		edgeDDA[one].edgeOnLeft = true;
		edgeDDA[two].edgeOnLeft = true;
		edgeDDA[three].edgeOnLeft = false;
	}
}

void GzRender::advanceSpan() {
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

void GzRender::advanceEdges() {
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