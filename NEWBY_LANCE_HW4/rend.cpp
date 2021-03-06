/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

#define PI (float) 3.14159265358979323846

float GzRender::degreesToRadians(float degree) {
	return degree * (PI / 180);
}

void GzRender::Multiply4x4(GzMatrix mat1, GzMatrix mat2) {
	GzMatrix tempM;
	tempM[0][0] = mat1[0][0] * mat2[0][0] + mat1[0][1] * mat2[1][0] + mat1[0][2] * mat2[2][0] + mat1[0][3] * mat2[3][0];
	tempM[0][1] = mat1[0][0] * mat2[0][1] + mat1[0][1] * mat2[1][1] + mat1[0][2] * mat2[2][1] + mat1[0][3] * mat2[3][1];
	tempM[0][2] = mat1[0][0] * mat2[0][2] + mat1[0][1] * mat2[1][2] + mat1[0][2] * mat2[2][2] + mat1[0][3] * mat2[3][2];
	tempM[0][3] = mat1[0][0] * mat2[0][3] + mat1[0][1] * mat2[1][3] + mat1[0][2] * mat2[2][3] + mat1[0][3] * mat2[3][3];

	tempM[1][0] = mat1[1][0] * mat2[0][0] + mat1[1][1] * mat2[1][0] + mat1[1][2] * mat2[2][0] + mat1[1][3] * mat2[3][0];
	tempM[1][1] = mat1[1][0] * mat2[0][1] + mat1[1][1] * mat2[1][1] + mat1[1][2] * mat2[2][1] + mat1[1][3] * mat2[3][1];
	tempM[1][2] = mat1[1][0] * mat2[0][2] + mat1[1][1] * mat2[1][2] + mat1[1][2] * mat2[2][2] + mat1[1][3] * mat2[3][2];
	tempM[1][3] = mat1[1][0] * mat2[0][3] + mat1[1][1] * mat2[1][3] + mat1[1][2] * mat2[2][3] + mat1[1][3] * mat2[3][3];

	tempM[2][0] = mat1[2][0] * mat2[0][0] + mat1[2][1] * mat2[1][0] + mat1[2][2] * mat2[2][0] + mat1[2][3] * mat2[3][0];
	tempM[2][1] = mat1[2][0] * mat2[0][1] + mat1[2][1] * mat2[1][1] + mat1[2][2] * mat2[2][1] + mat1[2][3] * mat2[3][1];
	tempM[2][2] = mat1[2][0] * mat2[0][2] + mat1[2][1] * mat2[1][2] + mat1[2][2] * mat2[2][2] + mat1[2][3] * mat2[3][2];
	tempM[2][3] = mat1[2][0] * mat2[0][3] + mat1[2][1] * mat2[1][3] + mat1[2][2] * mat2[2][3] + mat1[2][3] * mat2[3][3];

	tempM[3][0] = mat1[3][0] * mat2[0][0] + mat1[3][1] * mat2[1][0] + mat1[3][2] * mat2[2][0] + mat1[3][3] * mat2[3][0];
	tempM[3][1] = mat1[3][0] * mat2[0][1] + mat1[3][1] * mat2[1][1] + mat1[3][2] * mat2[2][1] + mat1[3][3] * mat2[3][1];
	tempM[3][2] = mat1[3][0] * mat2[0][2] + mat1[3][1] * mat2[1][2] + mat1[3][2] * mat2[2][2] + mat1[3][3] * mat2[3][2];
	tempM[3][3] = mat1[3][0] * mat2[0][3] + mat1[3][1] * mat2[1][3] + mat1[3][2] * mat2[2][3] + mat1[3][3] * mat2[3][3];

	mat1[0][0] = tempM[0][0]; mat1[0][1] = tempM[0][1]; mat1[0][2] = tempM[0][2]; mat1[0][3] = tempM[0][3];
	mat1[1][0] = tempM[1][0]; mat1[1][1] = tempM[1][1]; mat1[1][2] = tempM[1][2]; mat1[1][3] = tempM[1][3];
	mat1[2][0] = tempM[2][0]; mat1[2][1] = tempM[2][1]; mat1[2][2] = tempM[2][2]; mat1[2][3] = tempM[2][3];
	mat1[3][0] = tempM[3][0]; mat1[3][1] = tempM[3][1]; mat1[3][2] = tempM[3][2]; mat1[3][3] = tempM[3][3];
}

int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
	/* HW 3.1
	// Create rotate matrix : rotate along x axis
	// Pass back the matrix using mat value
	*/

	float rad = degreesToRadians(degree);

	// rotation matrix
	GzMatrix rMat = {
		1.0f,		0.0f,		0.0f,		0.0f,
		0.0f,		cos(rad),	-sin(rad),	0.0f,
		0.0f,		sin(rad),	cos(rad),	0.0f,
		0.0f,		0.0f,		0.0f,		1.0f
	};

	Multiply4x4(mat, rMat);

	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
	/* HW 3.2
	// Create rotate matrix : rotate along y axis
	// Pass back the matrix using mat value
	*/
	float rad = degreesToRadians(degree);

	GzMatrix rMat = {
		cos(rad),	0.0f,		sin(rad),	0.0f,
		0.0f,		1.0f,		0.0f,		0.0f,
		-sin(rad),	0.0f,		cos(rad),	0.0f,
		0.0f,		0.0f,		0.0f,		1.0f
	};

	Multiply4x4(mat, rMat);

	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
	/* HW 3.3
	// Create rotate matrix : rotate along z axis
	// Pass back the matrix using mat value
	*/
	float rad = degreesToRadians(degree);

	GzMatrix rMat = {
		cos(rad),	-sin(rad),	0.0f,		0.0f,
		sin(rad),	cos(rad),	0.0f,		0.0f,
		0.0f,		0.0f,		1.0f,		0.0f,
		0.0f,		0.0f,		0.0f,		1.0f
	};

	Multiply4x4(mat, rMat);

	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
	/* HW 3.4
	// Create translation matrix
	// Pass back the matrix using mat value
	*/
	GzMatrix tMat = {
		1.0f,		0.0f,		0.0f,		translate[X],
		0.0f,		1.0f,		0.0f,		translate[Y],
		0.0f,		0.0f,		1.0f,		translate[Z],
		0.0f,		0.0f,		0.0f,		1.0f
	};

	Multiply4x4(mat, tMat);

	return GZ_SUCCESS;
}

int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
	/* HW 3.5
	// Create scaling matrix
	// Pass back the matrix using mat value
	*/
	GzMatrix sMat = {
		scale[X],	0.0f,		0.0f,		0.0f,
		0.0f,		scale[Y],	0.0f,		0.0f,
		0.0f,		0.0f,		scale[Z],	0.0f,
		0.0f,		0.0f,		0.0f,		1.0f
	};

	Multiply4x4(mat, sMat);

	return GZ_SUCCESS;
}

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
	pixelbuffer = new GzPixel[totalPixels];
	framebuffer = (char*)malloc(3 * sizeof(char) * xRes * yRes);

	/* HW 3.6
	- setup Xsp and anything only done once
	- init default camera
	*/
	Xsp[0][0] = xRes/2.0f; 	Xsp[0][1] = 0.0f;		Xsp[0][2] = 0.0f;	Xsp[0][3] = xRes/2.0f;
	Xsp[1][0] = 0.0f;		Xsp[1][1] = -yRes/2.0f;	Xsp[1][2] = 0.0f;	Xsp[1][3] = yRes/2.0f;
	Xsp[2][0] = 0.0f;		Xsp[2][1] = 0.0f;		Xsp[2][2] = MAXINT; Xsp[2][3] = 0.0f;
	Xsp[3][0] = 0.0f;		Xsp[3][1] = 0.0f;		Xsp[3][2] = 0.0f;	Xsp[3][3] = 1.0f;

	m_camera.FOV = DEFAULT_FOV;
	m_camera.lookat[X] = 0;
	m_camera.lookat[Y] = 0;
	m_camera.lookat[Z] = 0;
	m_camera.position[X] = DEFAULT_IM_X;
	m_camera.position[Y] = DEFAULT_IM_Y;
	m_camera.position[Z] = DEFAULT_IM_Z;
	m_camera.worldup[X] = 0.0f;
	m_camera.worldup[Y] = 1.0f;
	m_camera.worldup[Z] = 0.0f;

	// default top of empty position in Ximage & Xnorm stack
	matlevel = -1;
	numlights = 0;
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
	for (unsigned int key = 0; key < totalPixels; ++key) {
		pixelbuffer[key].red = 3000;
		pixelbuffer[key].green = 3000;
		pixelbuffer[key].blue = 4000;
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
	GzCoord normX;
	GzCoord normY;
	GzCoord normZ;
	GzCoord CLVector = {
		m_camera.lookat[X] - m_camera.position[X],
		m_camera.lookat[Y] - m_camera.position[Y],
		m_camera.lookat[Z] - m_camera.position[Z]
	};
	GzMatrix identityMatrix = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	float magnitudeCLVector = sqrt(CLVector[X] * CLVector[X] + CLVector[Y] * CLVector[Y] + CLVector[Z] * CLVector[Z]);

	// calculate normal Z = cam.
	normZ[X] = CLVector[X] / magnitudeCLVector;
	normZ[Y] = CLVector[Y] / magnitudeCLVector;
	normZ[Z] = CLVector[Z] / magnitudeCLVector;

	float dotProdOfUpAndNormZ = m_camera.worldup[X] * normZ[X] + m_camera.worldup[Y] * normZ[Y] + m_camera.worldup[Z] * normZ[Z];
	GzCoord upPrime = {
		m_camera.worldup[X] - dotProdOfUpAndNormZ * normZ[X],
		m_camera.worldup[Y] - dotProdOfUpAndNormZ * normZ[Y],
		m_camera.worldup[Z] - dotProdOfUpAndNormZ * normZ[Z]
	};

	float magnitudeUpPrime = sqrt(upPrime[X] * upPrime[X] + upPrime[Y] * upPrime[Y] + upPrime[Z] * upPrime[Z]);
	// calculate normal Y = upPrime / ||upPrime||
	normY[X] = upPrime[X] / magnitudeUpPrime;
	normY[Y] = upPrime[Y] / magnitudeUpPrime;;
	normY[Z] = upPrime[Z] / magnitudeUpPrime;;

	// calculate normal X = Y x Z
	normX[X] = normY[Y] * normZ[Z] - normY[Z] * normZ[Y];
	normX[Y] = normY[Z] * normZ[X] - normY[X] * normZ[Z];
	normX[Z] = normY[X] * normZ[Y] - normY[Y] * normZ[X];

	float FOVinRadians = degreesToRadians(m_camera.FOV);
	m_camera.Xiw[0][0] = normX[X]; 	m_camera.Xiw[0][1] = normX[Y];	m_camera.Xiw[0][2] = normX[Z];	m_camera.Xiw[0][3] = -1.0f * (normX[X] * m_camera.position[X] + normX[Y] * m_camera.position[Y] + normX[Z] * m_camera.position[Z]);
	m_camera.Xiw[1][0] = normY[X];	m_camera.Xiw[1][1] = normY[Y];	m_camera.Xiw[1][2] = normY[Z];	m_camera.Xiw[1][3] = -1.0f * (normY[X] * m_camera.position[X] + normY[Y] * m_camera.position[Y] + normY[Z] * m_camera.position[Z]);
	m_camera.Xiw[2][0] = normZ[X];	m_camera.Xiw[2][1] = normZ[Y];	m_camera.Xiw[2][2] = normZ[Z];	m_camera.Xiw[2][3] = -1.0f * (normZ[X] * m_camera.position[X] + normZ[Y] * m_camera.position[Y] + normZ[Z] * m_camera.position[Z]);
	m_camera.Xiw[3][0] = 0.0f;		m_camera.Xiw[3][1] = 0.0f;		m_camera.Xiw[3][2] = 0.0f;		m_camera.Xiw[3][3] = 1.0f;

	m_camera.Xpi[0][0] = 1.0f; 	m_camera.Xpi[0][1] = 0.0f;	m_camera.Xpi[0][2] = 0.0f;					m_camera.Xpi[0][3] = 0.0f;
	m_camera.Xpi[1][0] = 0.0f;	m_camera.Xpi[1][1] = 1.0f;	m_camera.Xpi[1][2] = 0.0f;					m_camera.Xpi[1][3] = 0.0f;
	m_camera.Xpi[2][0] = 0.0f;	m_camera.Xpi[2][1] = 0.0f;	m_camera.Xpi[2][2] = tan(FOVinRadians / 2.0);	m_camera.Xpi[2][3] = 0.0f;
	m_camera.Xpi[3][0] = 0.0f;	m_camera.Xpi[3][1] = 0.0f;	m_camera.Xpi[3][2] = tan(FOVinRadians / 2.0);	m_camera.Xpi[3][3] = 1.0f;

	// Push Xform matrices onto Ximage stack. The top of the stack will screen space to world space transform matrix
	GzPushMatrix(Xsp);
	setToIdentity(Xnorm[matlevel]);
	GzPushMatrix(m_camera.Xpi);
	setToIdentity(Xnorm[matlevel]);
	GzPushMatrix(m_camera.Xiw);

	return GZ_SUCCESS;
}

int GzRender::GzPutCamera(GzCamera camera)
{
	/* HW 3.8
	/*- overwrite renderer camera structure with new camera definition
	*/
	m_camera.FOV = camera.FOV;
	m_camera.lookat[X] = camera.lookat[X];
	m_camera.lookat[Y] = camera.lookat[Y];
	m_camera.lookat[Z] = camera.lookat[Z];
	m_camera.position[X] = camera.position[X];
	m_camera.position[Y] = camera.position[Y];
	m_camera.position[Z] = camera.position[Z];
	m_camera.worldup[X] = camera.worldup[X];
	m_camera.worldup[Y] = camera.worldup[Y];
	m_camera.worldup[Z] = camera.worldup[Z];

	return GZ_SUCCESS;
}

int GzRender::GzPushMatrix(GzMatrix	matrix)
{
	/* HW 3.9
	- push a matrix onto the Ximage stack
	- check for stack overflow
	*/
	int newLevel = matlevel + 1;

	if (matlevel < 0) {
		// Build Ximage
		Ximage[newLevel][0][0] = matrix[0][0];
		Ximage[newLevel][0][1] = matrix[0][1];
		Ximage[newLevel][0][2] = matrix[0][2];
		Ximage[newLevel][0][3] = matrix[0][3];
		Ximage[newLevel][1][0] = matrix[1][0];
		Ximage[newLevel][1][1] = matrix[1][1];
		Ximage[newLevel][1][2] = matrix[1][2];
		Ximage[newLevel][1][3] = matrix[1][3];
		Ximage[newLevel][2][0] = matrix[2][0];
		Ximage[newLevel][2][1] = matrix[2][1];
		Ximage[newLevel][2][2] = matrix[2][2];
		Ximage[newLevel][2][3] = matrix[2][3];
		Ximage[newLevel][3][0] = matrix[3][0];
		Ximage[newLevel][3][1] = matrix[3][1];
		Ximage[newLevel][3][2] = matrix[3][2];
		Ximage[newLevel][3][3] = matrix[3][3];
	}
	else if (matlevel < MATLEVELS) {
		// multiply the new matrix by the top of the stack matrix
		setToIdentity(Ximage[newLevel]);
		Multiply4x4(Ximage[newLevel], Ximage[matlevel]);
		Multiply4x4(Ximage[newLevel], matrix);

		setToIdentity(Xnorm[newLevel]);
		Multiply4x4(Xnorm[newLevel], Xnorm[matlevel]);
		Multiply4x4(Xnorm[newLevel], matrix);

		// remove translation from Xnorm top of stack (TOS)
		removeTranslation(Xnorm[newLevel]);
		// Remove scale from Xnorm TOS
		removeScale(Xnorm[newLevel]);
	}

	// increment to new top of the Ximage stack
	matlevel += 1;

	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
	/* HW 3.10
	- pop a matrix off the Ximage stack
	- check for stack underflow
	*/
	if (matlevel >= 0) {
		--matlevel;
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
	for (unsigned int key = 0; key < totalPixels; ++key) {
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
	for (unsigned int key = 0; key < totalPixels; ++key) {
		framebuffer[key * pixelDepthInBytes + RED] = convertGzIntensityTo8bitRGB(pixelbuffer[key].blue);
		framebuffer[key * pixelDepthInBytes + GREEN] = convertGzIntensityTo8bitRGB(pixelbuffer[key].green);
		framebuffer[key * pixelDepthInBytes + BLUE] = convertGzIntensityTo8bitRGB(pixelbuffer[key].red);
	}

	return GZ_SUCCESS;
}

int GzRender::GzPutAttribute(int numAttributes, GzToken	*nameList, GzPointer *valueList)
{
	/* HW 2.1
	-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
	-- In later homeworks set shaders, interpolaters, texture maps, and lights
	*/
	for (unsigned int token = 0; token < numAttributes; ++token) {
		float *color;
		GzColor *colorCoefficient;

		switch (nameList[token]) {
		case GZ_RGB_COLOR:
			color = (float*)valueList[token];
			flatcolor[RED] = boundFloat(color[RED], 0.0, 1.0);
			flatcolor[GREEN] = boundFloat(color[GREEN], 0.0, 1.0);
			flatcolor[BLUE] = boundFloat(color[BLUE], 0.0, 1.0);
			break;
		case GZ_INTERPOLATE:
			interp_mode = *(int *)valueList[token];
			break;
		case GZ_DIRECTIONAL_LIGHT:
			if (numlights < MAX_LIGHTS) {
				lights[numlights] = *(GzLight *)valueList[token];
				numlights += 1;
			}
			break;
		case GZ_AMBIENT_LIGHT:
			if (numlights < MAX_LIGHTS) {
				ambientlight = *(GzLight *)valueList[token];
			}
			break;
		case GZ_AMBIENT_COEFFICIENT:
			colorCoefficient = (GzColor*)valueList[token];
			Ka[RED] = colorCoefficient[0][RED];
			Ka[GREEN] = colorCoefficient[0][GREEN];
			Ka[BLUE] = colorCoefficient[0][BLUE];
			break;
		case GZ_DIFFUSE_COEFFICIENT:
			colorCoefficient = (GzColor*)valueList[token];
			Kd[RED] = colorCoefficient[0][RED];
			Kd[GREEN] = colorCoefficient[0][GREEN];
			Kd[BLUE] = colorCoefficient[0][BLUE];
			break;
		case GZ_SPECULAR_COEFFICIENT:
			colorCoefficient = (GzColor*)valueList[token];
			Ks[RED] = colorCoefficient[0][RED];
			Ks[GREEN] = colorCoefficient[0][GREEN];
			Ks[BLUE] = colorCoefficient[0][BLUE];
			break;
		case GZ_DISTRIBUTION_COEFFICIENT:
			spec = *(float*)valueList[token];
			break;
		case GZ_TEXTURE_INDEX:
			break;
		default:
			break;
		}
	}

	return GZ_SUCCESS;
}

float GzRender::dotProduct(GzCoord m, GzCoord n) {
	return m[0] * n[0] + m[1] * n[1] + m[2] * n[2];
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
	
	bool isZCoordsAllPositive = false;

	for (unsigned int token = 0; token < numParts; ++token) {
		switch (nameList[token]) {
		case GZ_POSITION: {
			GzMatrix vert4x4;
			GzCoord *vertex = (GzCoord*)valueList[0];
			// multiply Xsw
			vert4x4[0][0] = vertex[one][X]; vert4x4[0][1] = vertex[two][X]; vert4x4[0][2] = vertex[three][X];	vert4x4[0][3] = 0;
			vert4x4[1][0] = vertex[one][Y]; vert4x4[1][1] = vertex[two][Y]; vert4x4[1][2] = vertex[three][Y];	vert4x4[1][3] = 0;
			vert4x4[2][0] = vertex[one][Z]; vert4x4[2][1] = vertex[two][Z]; vert4x4[2][2] = vertex[three][Z];	vert4x4[2][3] = 0;
			vert4x4[3][0] = 1;				vert4x4[3][1] = 1;				vert4x4[3][2] = 1;					vert4x4[3][3] = 1;

			// push triangle vertices model matrix on transformation stack
			GzPushMatrix(vert4x4);

			// ignore triangles with negative z-coord
			isZCoordsAllPositive = Ximage[matlevel][2][0] >= 0 && Ximage[matlevel][2][1] >= 0 && Ximage[matlevel][2][2] >= 0;

			if (isZCoordsAllPositive) {
				for (unsigned int i = 0; i < 3; ++i) {
					for (unsigned int j = 0; j < 3; ++j) {
						// normalize coords by dividing by w
						Ximage[matlevel][i][j] /= Ximage[matlevel][3][j];

						// update valueList for passing to rasterizer
						vertex[j][i] = Ximage[matlevel][i][j];
					}
				}
				
			}

			// Pop model matrix off transformation stack
			GzPopMatrix();
		}
			break;
		case GZ_NORMAL: {
			GzMatrix norm4x4;
			GzCoord *normal = (GzCoord*)valueList[1];
			// multiply Xiw
			norm4x4[0][0] = normal[one][X]; norm4x4[0][1] = normal[two][X]; norm4x4[0][2] = normal[three][X];	norm4x4[0][3] = 0;
			norm4x4[1][0] = normal[one][Y]; norm4x4[1][1] = normal[two][Y]; norm4x4[1][2] = normal[three][Y];	norm4x4[1][3] = 0;
			norm4x4[2][0] = normal[one][Z]; norm4x4[2][1] = normal[two][Z]; norm4x4[2][2] = normal[three][Z];	norm4x4[2][3] = 0;
			norm4x4[3][0] = 1;				norm4x4[3][1] = 1;				norm4x4[3][2] = 1;					norm4x4[3][3] = 1;
			
			GzPushMatrix(norm4x4);

			for (unsigned int i = 0; i < 3; ++i) {
				for (unsigned int j = 0; j < 3; ++j) {
					// normalize coords by dividing by w
					Xnorm[matlevel][i][j] /= Xnorm[matlevel][3][j];

					// update valueList for passing to rasterizer
					normal[i][j] = Xnorm[matlevel][j][i];
				}
			}
			//transpose(Xnorm[matlevel]);

			GzPopMatrix();
		}
			break;
		case GZ_NULL_TOKEN:
			break;
		default:
			break;
		}
	}

	scanLineRender(valueList);

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
	for (int edgeNum = 0; edgeNum < totalEdges; ++edgeNum) {
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

void GzRender::shadingEquation(GzCoord N) {
	// normalize N
	normalizeVector(N);
	// Set E (should be the negation of the camera's lookat vector)
	GzCoord E = { 0, 0, -1 };
	GzColor diffuseComponent = { 0.0f, 0.0f, 0.0f };
	GzColor specularComponent = { 0.0f, 0.0f, 0.0f };
	GzColor ambientComponent;
	bool computeLightModel;
	GzCoord R;
	float RdotE;
	float NdotL;
	GzCoord nPrime = { N[X], N[Y], N[Z] };

	// Calculate Normal dot eye vector
	float NdotE = dotProduct(N, E);

	diffuseComponent[RED] = 0.0f;
	diffuseComponent[GREEN] = 0.0f;
	diffuseComponent[BLUE] = 0.0f;

	specularComponent[RED] = 0.0f;
	specularComponent[GREEN] = 0.0f;
	specularComponent[BLUE] = 0.0f;

	for (unsigned int lightIndex = 0; lightIndex < numlights; ++lightIndex) {
		computeLightModel = false;
		// normaize light direction vector
		normalizeVector(lights[lightIndex].direction);

		// Calculate Normal dot Light vector direction
		NdotL = dotProduct(N, lights[lightIndex].direction);

		// If signs of NdotL and NdotE are negative flip the normal direction
		if (NdotL < 0 && NdotE < 0) {
			nPrime[X] = -N[X];
			nPrime[Y] = -N[Y];
			nPrime[Z] = -N[Z];
			computeLightModel = true;
		}
		else if (NdotL > 0 && NdotE > 0) {
			computeLightModel = true;
		}

		if (computeLightModel) {
			// Calculate Normal dot Light vector direction
			NdotL = dotProduct(nPrime, lights[lightIndex].direction);
			
			// Calculate R (reflection vector) for each light
			// R = 2(N.L)N - L
			R[X] = 2 * NdotL * nPrime[X] - lights[lightIndex].direction[X];
			R[Y] = 2 * NdotL * nPrime[Y] - lights[lightIndex].direction[Y];
			R[Z] = 2 * NdotL * nPrime[Z] - lights[lightIndex].direction[Z];
			
			// normalize R
			normalizeVector(R);

			// R.E
			RdotE = dotProduct(R, E);

			// Calculate specular component sum[le(R.E)s]
			specularComponent[RED] += lights[lightIndex].color[RED] * powf(boundFloat(RdotE, 0, 1), spec);
			specularComponent[GREEN] += lights[lightIndex].color[GREEN] * powf(boundFloat(RdotE, 0, 1), spec);
			specularComponent[BLUE] += lights[lightIndex].color[BLUE] * powf(boundFloat(RdotE, 0, 1), spec);

			// Calculate diffuse component sum[le(N.L)]
			diffuseComponent[RED] += lights[lightIndex].color[RED] * NdotL;
			diffuseComponent[GREEN] += lights[lightIndex].color[GREEN] * NdotL;
			diffuseComponent[BLUE] += lights[lightIndex].color[BLUE] * NdotL;
		}
	}

	// Complete specular component (Ks sum[le(R.E)s])
	specularComponent[RED] = Ks[RED] * specularComponent[RED];
	specularComponent[GREEN] = Ks[GREEN] * specularComponent[GREEN];
	specularComponent[BLUE] = Ks[BLUE] * specularComponent[BLUE];

	// Complete diffuse component (Kd sum[le(N.L)])
	diffuseComponent[RED] = Kd[RED] * diffuseComponent[RED];
	diffuseComponent[GREEN] = Kd[GREEN] * diffuseComponent[GREEN];
	diffuseComponent[BLUE] = Kd[BLUE] * diffuseComponent[BLUE];

	// Complete ambient component (Ka Ia)
	ambientComponent[RED] = Ka[RED] * ambientlight.color[RED];
	ambientComponent[GREEN] = Ka[GREEN] * ambientlight.color[GREEN];
	ambientComponent[BLUE] = Ka[BLUE] * ambientlight.color[BLUE];

	// C = (Ks sum[le(R.E)s]) + (Kd sum[le(N.L)]) + (Ka Ia)
	C[RED] = boundFloat(specularComponent[RED] + diffuseComponent[RED] + ambientComponent[RED], 0, 1);
	C[GREEN] = boundFloat(specularComponent[GREEN] + diffuseComponent[GREEN] + ambientComponent[GREEN], 0, 1);
	C[BLUE] = boundFloat(specularComponent[BLUE] + diffuseComponent[BLUE] + ambientComponent[BLUE], 0, 1);
}

void GzRender::advanceSpan(GzPointer *valueList) {
	// Get vertice list
	GzCoord *vertex = (GzCoord *)valueList[0];
	GzCoord *normal = (GzCoord *)valueList[1];

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

	// interpoluate color
	GzColor interpColor;
	GzCoord interpNormal;

	// Interpolate span position and parameters (Z) until current position > end
	while (spanDDA.current[X] <= spanDDA.end[X]) {
		// Test interpolated-Z against FB-Z for each pixel - low Z wins
		int currentPixelIndex = ARRAY(spanDDA.current[X], spanDDA.current[Y]);
		if (currentPixelIndex <  totalPixels && spanDDA.current[Z] < pixelbuffer[currentPixelIndex].z) {
			switch (interp_mode) {
			case GZ_FLAT:
				shadingEquation(normal[0]);
				red = ctoi(C[RED]);
				green = ctoi(C[GREEN]);
				blue = ctoi(C[BLUE]);
				break;
			case GZ_COLOR: {
				// interpolate color
				GzColor vertColor[3];
				for (unsigned int i = 0; i < 3; ++i) {
					shadingEquation(normal[i]);
					vertColor[i][RED] = C[RED];
					vertColor[i][GREEN] = C[GREEN];
					vertColor[i][BLUE] = C[BLUE];
				}

				for (unsigned int color = 0; color < 3; ++color) {
					// Calculate normal <A, B, C>
					GzCoord c1 = {
						vertex[three][X] - vertex[one][X],
						vertex[three][Y] - vertex[one][Y],
						vertColor[three][color] - vertColor[one][color]
					};
					GzCoord c2 = {
						vertex[two][X] - vertex[three][X],
						vertex[two][Y] - vertex[three][Y],
						vertColor[two][color] - vertColor[three][color]
					};

					// r1 x r2
					GzCoord colorNormal = {
						c1[Y] * c2[Z] - c1[Z] * c2[Y],
						c1[Z] * c2[X] - c1[X] * c2[Z],
						c1[X] * c2[Y] - c1[Y] * c2[X]
					};

					float A = colorNormal[X];
					float B = colorNormal[Y];
					float C = colorNormal[Z];
					float D = -(A * vertex[one][X]) - B * vertex[one][Y] - C * vertColor[one][color];

					interpColor[color] = (-(A * spanDDA.current[X]) - (B * spanDDA.current[Y]) - D) / C;
				}

				red = ctoi(interpColor[RED]);
				green = ctoi(interpColor[GREEN]);
				blue = ctoi(interpColor[BLUE]);
			}
				break;
			case GZ_NORMALS: {
				// interpolate normals
				for (unsigned int norm = 0; norm < 3; ++norm) {
					// Calculate normal <A, B, C>
					GzCoord c1 = {
						vertex[three][X] - vertex[one][X],
						vertex[three][Y] - vertex[one][Y],
						normal[three][norm] - normal[one][norm]
					};
					GzCoord c2 = {
						vertex[two][X] - vertex[three][X],
						vertex[two][Y] - vertex[three][Y],
						normal[two][norm] - normal[three][norm]
					};

					// r1 x r2
					GzCoord normNormal = {
						c1[Y] * c2[Z] - c1[Z] * c2[Y],
						c1[Z] * c2[X] - c1[X] * c2[Z],
						c1[X] * c2[Y] - c1[Y] * c2[X]
					};

					float A = normNormal[X];
					float B = normNormal[Y];
					float C = normNormal[Z];
					float D = -(A * vertex[one][X] + B * vertex[one][Y] + C * normal[one][norm]);

					interpNormal[norm] = -(A * spanDDA.current[X] + B * spanDDA.current[Y] + D) / C;
				}

				// interpolate color
				GzColor vertColor[3];
				for (unsigned int i = 0; i < 3; ++i) {
					normalizeVector(interpNormal);
					shadingEquation(interpNormal);
					vertColor[i][RED] = C[RED];
					vertColor[i][GREEN] = C[GREEN];
					vertColor[i][BLUE] = C[BLUE];
				}

				for (unsigned int color = 0; color < 3; ++color) {
					// Calculate normal <A, B, C>
					GzCoord c1 = {
						vertex[three][X] - vertex[one][X],
						vertex[three][Y] - vertex[one][Y],
						vertColor[three][color] - vertColor[one][color]
					};
					GzCoord c2 = {
						vertex[two][X] - vertex[three][X],
						vertex[two][Y] - vertex[three][Y],
						vertColor[two][color] - vertColor[three][color]
					};

					// r1 x r2
					GzCoord colorNormal = {
						c1[Y] * c2[Z] - c1[Z] * c2[Y],
						c1[Z] * c2[X] - c1[X] * c2[Z],
						c1[X] * c2[Y] - c1[Y] * c2[X]
					};

					// normalizeVector(colorNormal);

					float A = colorNormal[X];
					float B = colorNormal[Y];
					float C = colorNormal[Z];
					float D = -(A * vertex[one][X]) - B * vertex[one][Y] - C * vertColor[one][color];

					interpColor[color] = (-(A * spanDDA.current[X]) - (B * spanDDA.current[Y]) - D) / C;
				}

				red = ctoi(interpColor[RED]);
				green = ctoi(interpColor[GREEN]);
				blue = ctoi(interpColor[BLUE]);
			}
				break;
			default:
				break;
			}

			
			GzPut(spanDDA.current[X], spanDDA.current[Y], red, green, blue, 1, spanDDA.current[Z]);
		}

		// Advance span current position
		newX += 1;
		spanDDA.current[X] = newX;
		spanDDA.current[Z] = getZCoord(spanDDA.start, spanDDA.end, newX);
	}
}

void GzRender::advanceEdges(GzPointer *valueList) {
	// Advance (1-2) and (1-3) DDA current positions to top y-scan line (ceiling)
	float newY = ceil(edgeDDA[one].start[Y]);
	edgeDDA[one].current[X] = getXCoord(edgeDDA[one].start, edgeDDA[one].end, newY);
	edgeDDA[one].current[Y] = newY;
	edgeDDA[one].current[Z] = getZCoord(edgeDDA[one].start, edgeDDA[one].end, edgeDDA[one].current[X]);

	edgeDDA[three].current[X] = getXCoord(edgeDDA[three].start, edgeDDA[three].end, newY);
	edgeDDA[three].current[Y] = newY;
	edgeDDA[three].current[Z] = getZCoord(edgeDDA[three].start, edgeDDA[three].end, edgeDDA[three].current[X]);

	// Switch from 1 - 2 edge to 2 - 3 edge when current 1 - 2 edge position > Y(2)	
	while (edgeDDA[one].current[Y] < edgeDDA[one].end[Y]) {
		// Setup span DDA on successive lines based on edge DDA position values
		// Set span DDA current and end positions to right and left edge values 
		spanDDA.start[X] = edgeDDA[one].edgeOnLeft ? edgeDDA[one].current[X] : edgeDDA[three].current[X];
		spanDDA.start[Y] = edgeDDA[one].edgeOnLeft ? edgeDDA[one].current[Y] : edgeDDA[three].current[Y];
		spanDDA.start[Z] = edgeDDA[one].edgeOnLeft ? edgeDDA[one].current[Z] : edgeDDA[three].current[Z];

		spanDDA.end[X] = edgeDDA[one].edgeOnLeft ? edgeDDA[three].current[X] : edgeDDA[one].current[X];
		spanDDA.end[Y] = edgeDDA[one].edgeOnLeft ? edgeDDA[three].current[Y] : edgeDDA[one].current[Y];
		spanDDA.end[Z] = edgeDDA[one].edgeOnLeft ? edgeDDA[three].current[Z] : edgeDDA[one].current[Z];

		advanceSpan(valueList);

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
	while (edgeDDA[two].current[Y] < edgeDDA[two].end[Y]) {
		// Setup span DDA on successive lines based on edge DDA position values
		// Set span DDA current and end positions to right and left edge values 
		spanDDA.start[X] = edgeDDA[two].edgeOnLeft ? edgeDDA[two].current[X] : edgeDDA[three].current[X];
		spanDDA.start[Y] = edgeDDA[two].edgeOnLeft ? edgeDDA[two].current[Y] : edgeDDA[three].current[Y];
		spanDDA.start[Z] = edgeDDA[two].edgeOnLeft ? edgeDDA[two].current[Z] : edgeDDA[three].current[Z];

		spanDDA.end[X] = edgeDDA[two].edgeOnLeft ? edgeDDA[three].current[X] : edgeDDA[two].current[X];
		spanDDA.end[Y] = edgeDDA[two].edgeOnLeft ? edgeDDA[three].current[Y] : edgeDDA[two].current[Y];
		spanDDA.end[Z] = edgeDDA[two].edgeOnLeft ? edgeDDA[three].current[Z] : edgeDDA[two].current[Z];

		advanceSpan(valueList);

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

void GzRender::scanLineRender(GzPointer *valueList) {
	sortVerticesByY(valueList);
	setupEdgeDDAs(valueList);
	sortEdgesLeftOrRight();
	advanceEdges(valueList);
}