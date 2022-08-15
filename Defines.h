#pragma once
#include <iostream>
#include <corecrt_math_defines.h>
#include "RasterSurface.h"
#include "XTime.h"
#include "greendragon.h"

// Handy definitions
#define RASTER_WIDTH 500
#define RASTER_HEIGHT 500
#define RASTER_HALFWIDTH 250
#define RASTER_HALFHEIGHT 250
#define NUM_PIXELS RASTER_WIDTH * RASTER_HEIGHT
#define COLOR_WHITE 0x00FFFFFF
#define COLOR_GREEN 0x0000FF00
#define COLOR_RED 0x00FF0000
#define COLOR_BLUE 0xFF0000FF
#define COLOR_YELLOW 0xFFFFFF00
#define COLOR_CYAN 0xFF00FFFF
#define NEAR_PLANE 0.1f
#define FAR_PLANE 10.0f
#define FOV 90.0f
#define ASPECT_RATIO static_cast<float>(RASTER_HEIGHT) / static_cast<float>(RASTER_WIDTH)
#define identity4X4 {{ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 }}

// Struct definition for a Vector2
struct VECTOR2
{
	float x;
	float y;

	// Overloaded constructor for our Vector2
	VECTOR2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

// Struct definition for a Vector3
struct VECTOR3
{
	float x;
	float y;
	float z;
};

// Struct definition for a Vector4
struct VECTOR4
{
	float x;
	float y;
	float z;
	float w;
};

// Struct definition for a 3x3 matrix
struct MATRIX3X3
{
	VECTOR3 x;
	VECTOR3 y;
	VECTOR3 z;
};

// Struct definition for a 4x4 matrix
struct MATRIX4X4
{
	VECTOR4 x;
	VECTOR4 y;
	VECTOR4 z;
	VECTOR4 w;
};

// Struct definition for a vertex
struct VERTEX
{
	float x;
	float y;
	float z;
	float w;
	unsigned int color;
	float u;
	float v;

	// Default constructor for our Vertex
	VERTEX()
	{
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 1;
		unsigned int color = COLOR_WHITE;
		float u = 0;
		float v = 0;
	}

	// Overloaded constructor for our Vertex
	VERTEX(float x, float y, float z, float w, unsigned int color, float u = 0, float v = 0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		this->color = color;
		this->u = u;
		this->v = v;
	}

	// Operator so we can C style cast to a Vector2
	operator VECTOR2() { return VECTOR2(x, y); }
};

// Struct definition for 2D vertices
struct VERTEX_2D
{
	VECTOR2 vectorPos;
	unsigned int color;
};

// Struct definition for screen x and y coordinates
struct SCREEN_XY
{
	unsigned int x;
	unsigned int y;
};

// typedef definition for a pixel
typedef unsigned int uint;

// Variables for the Raster Surface
unsigned int pixelArr[NUM_PIXELS]; // The pixel array for our Raster Surface
const unsigned int pixelArrSize{ static_cast<unsigned int>(std::size(pixelArr)) }; // Cast the size of pixelArr to a const unsigned int

// This will hold all of the data we need for our cube
VERTEX cubeVertices[] = {
	// All our points on our cube
	VERTEX(-0.25f,    -0.25f, -0.25f,    1, COLOR_WHITE, 0, 0), //0
	VERTEX(0.25f,    -0.25f, -0.25f,    1, COLOR_WHITE, 0, 0), //1
	VERTEX(0.25f,    -0.25f,  0.25f,    1, COLOR_WHITE, 0, 0), //2
	VERTEX(-0.25f,    -0.25f,  0.25f,    1, COLOR_WHITE, 0, 0), //3
	VERTEX(-0.25f,     0.25f, -0.25f,    1, COLOR_WHITE, 0, 0), //4
	VERTEX(0.25f,     0.25f, -0.25f,    1, COLOR_WHITE, 0, 0), //5
	VERTEX(0.25f,     0.25f,  0.25f,    1, COLOR_WHITE, 0, 0), //6
	VERTEX(-0.25f,     0.25f,  0.25f,    1, COLOR_WHITE, 0, 0), //7
};

// This will hold all of the data we need for our grid
VERTEX gridVertices[] = {
	
	// Handle the x in the negative z
	VERTEX(-0.5, 0, -0.5, 1, COLOR_WHITE),
	VERTEX(-0.4, 0, -0.5, 1, COLOR_WHITE),
	VERTEX(-0.3, 0, -0.5, 1, COLOR_WHITE),
	VERTEX(-0.2, 0, -0.5, 1, COLOR_WHITE),
	VERTEX(-0.1, 0, -0.5, 1, COLOR_WHITE),
	VERTEX(0, 0, -0.5, 1, COLOR_WHITE),
	VERTEX(0.1, 0, -0.5, 1, COLOR_WHITE),
	VERTEX(0.2, 0, -0.5, 1, COLOR_WHITE),
	VERTEX(0.3, 0, -0.5, 1, COLOR_WHITE),
	VERTEX(0.4, 0, -0.5, 1, COLOR_WHITE),
	VERTEX(0.5, 0, -0.5, 1, COLOR_WHITE),

	// Handle the x in the positive z
	VERTEX(-0.5, 0, 0.5, 1, COLOR_WHITE),
	VERTEX(-0.4, 0, 0.5, 1, COLOR_WHITE),
	VERTEX(-0.3, 0, 0.5, 1, COLOR_WHITE),
	VERTEX(-0.2, 0, 0.5, 1, COLOR_WHITE),
	VERTEX(-0.1, 0, 0.5, 1, COLOR_WHITE),
	VERTEX(0, 0, 0.5, 1, COLOR_WHITE),
	VERTEX(0.1, 0, 0.5, 1, COLOR_WHITE),
	VERTEX(0.2, 0, 0.5, 1, COLOR_WHITE),
	VERTEX(0.3, 0, 0.5, 1, COLOR_WHITE),
	VERTEX(0.4, 0, 0.5, 1, COLOR_WHITE),
	VERTEX(0.5, 0, 0.5, 1, COLOR_WHITE),

	// Handle the z in the positive x
	VERTEX(0.5, 0, -0.5, 1, COLOR_WHITE),
	VERTEX(0.5, 0, -0.4, 1, COLOR_WHITE),
	VERTEX(0.5, 0, -0.3, 1, COLOR_WHITE),
	VERTEX(0.5, 0, -0.2, 1, COLOR_WHITE),
	VERTEX(0.5, 0, -0.1, 1, COLOR_WHITE),
	VERTEX(0.5, 0, 0, 1, COLOR_WHITE),
	VERTEX(0.5, 0, 0.1, 1, COLOR_WHITE),
	VERTEX(0.5, 0, 0.2, 1, COLOR_WHITE),
	VERTEX(0.5, 0, 0.3, 1, COLOR_WHITE),
	VERTEX(0.5, 0, 0.4, 1, COLOR_WHITE),
	VERTEX(0.5, 0, 0.5, 1, COLOR_WHITE),

	// Handle the z in the negative x
	VERTEX(-0.5, 0, -0.5, 1, COLOR_WHITE),
	VERTEX(-0.5, 0, -0.4, 1, COLOR_WHITE),
	VERTEX(-0.5, 0, -0.3, 1, COLOR_WHITE),
	VERTEX(-0.5, 0, -0.2, 1, COLOR_WHITE),
	VERTEX(-0.5, 0, -0.1, 1, COLOR_WHITE),
	VERTEX(-0.5, 0, 0, 1, COLOR_WHITE),
	VERTEX(-0.5, 0, 0.1, 1, COLOR_WHITE),
	VERTEX(-0.5, 0, 0.2, 1, COLOR_WHITE),
	VERTEX(-0.5, 0, 0.3, 1, COLOR_WHITE),
	VERTEX(-0.5, 0, 0.4, 1, COLOR_WHITE),
	VERTEX(-0.5, 0, 0.5, 1, COLOR_WHITE),
};

// Our index buffer to iterate and draw our cube
int indexBuffer[] = {
	0, 1,
	1, 2,
	2, 3,
	3, 0,
	4, 5,
	5, 6,
	6, 7,
	7, 4,
	3, 7,
	0, 4,
	1, 5,
	2, 6
};

// Our index buffer to iterate and draw our grid
int indexBuffer02[] = {
	0, 11,
	1, 12,
	2, 13,
	3, 14,
	4, 15,
	5, 16,
	6, 17,
	7, 18,
	8, 19,
	9, 20,
	10, 21,
	21, 32,
	22, 33,
	23, 34,
	24, 35,
	25, 36,
	26, 37,
	27, 38,
	28, 39,
	29, 40,
	30, 41,
	31, 42,
	32, 43,
};

// Our index buffer to iterate and draw our triangle
int indexBufferTriangle[] = {
	0, 4, 1,
	5, 1, 4,
	1, 5, 2,
	6, 2, 5,
	2, 6, 3,
	7, 3, 6,
	3, 7, 0,
	4, 0, 7,
	2, 1, 3,
	0, 3, 1,
	4, 7, 5,
	6, 5, 7,
};

// Our index buffer to iterate and draw our UV Points
int indexBufferUV[] = {
	0, 1, 0,
	0, 1, 1,
	1, 0, 1,
	1, 0, 0,
};

// Z Buffer
float zBuffer[NUM_PIXELS];
