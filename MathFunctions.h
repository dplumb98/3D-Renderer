#pragma once
#include "Defines.h"

int MakeOneDimenCoord(int x, int y, int width)
{
	return y * width + x; // Return a 1D coordinate from a 2D coordinate
}

// Function to handle the perspective divide
void PerspectiveDivide(VERTEX& vertex)
{
	vertex.x /= vertex.w;
	vertex.y /= vertex.w;
	vertex.z /= vertex.w;
}

SCREEN_XY GetScreenPositionOfVertexTOSCREENXY(const VERTEX& vertex)
{
	SCREEN_XY screen; // Create a screen variable to return to the function
	VERTEX newVertex = vertex; // Create a copy of the passed in vertex

	// Get the screen position of both the x and y components
	screen.x = std::floor((newVertex.x + 1) * (RASTER_WIDTH / 2));
	screen.y = std::floor((1 - newVertex.y) * (RASTER_HEIGHT / 2));

	// Return the result
	return screen;
}

VERTEX GetScreenPositionOfVertexTOVERTEX(const VERTEX& vertex)
{
	VERTEX newVertex = vertex; // Create a copy of the passed in vertex

	// Get the screen position of both the x and y components
	newVertex.x = std::floor((newVertex.x + 1) * (RASTER_WIDTH >> 1));
	newVertex.y = std::floor((1 - newVertex.y) * (RASTER_HEIGHT >> 1));

	// Maintain the component values of the passed in vertex
	newVertex.z = vertex.z;
	newVertex.w = vertex.w;
	newVertex.u = vertex.u;
	newVertex.v = vertex.v;
	newVertex.color = vertex.color;

	// Return the result
	return newVertex;
}

float ImplicitLineEquation(VECTOR2 a, VECTOR2 b, VECTOR2 c)
{
	return (a.y - b.y) * c.x + (b.x - a.x) * c.y + a.x * b.y - a.y * b.x; // Compute our implicit line equation
}

VECTOR3 FindBarycentricCoordinates(VECTOR2 a, VECTOR2 b, VECTOR2 c, VECTOR2 p)
{
	// Find the maxA and subA
	float maxA = ImplicitLineEquation(c, b, a);
	float subA = ImplicitLineEquation(c, b, p);

	// Find the maxB and subB
	float maxB = ImplicitLineEquation(a, c, b);
	float subB = ImplicitLineEquation(a, c, p);

	// Find the maxC and subC
	float maxC = ImplicitLineEquation(b, a, c);
	float subC = ImplicitLineEquation(b, a, p);

	// Return the result
	return { subA / maxA, subB / maxB, subC / maxC };
}

VECTOR3 Multiply3X3MatrixAndVector3(VECTOR3 vec, MATRIX3X3 matrix)
{
	// Used for storing results of the math and returning it to the function
	VECTOR3 vecResult;

	vecResult.x = (vec.x * matrix.x.x) + (vec.y * matrix.y.x) + (vec.z * matrix.z.x); // Multiply the first column of the matrix by column one of the vector
	vecResult.y = (vec.x * matrix.x.y) + (vec.y * matrix.y.y) + (vec.z * matrix.z.y); // Multiply the second column of the matrix by column one of the vector
	vecResult.z = (vec.x * matrix.x.z) + (vec.y * matrix.y.z) + (vec.z * matrix.z.z); // Multiply the third column of the matrix by column one of the vector

	// Return the result of the math
	return vecResult;
}

VERTEX Multiply3X3MatrixAndVertex(VERTEX vertex, MATRIX3X3 matrix)
{
	// Used for storing results of the math and returning it to the function
	VERTEX vecResult;

	vecResult.x = (vertex.x * matrix.x.x) + (vertex.y * matrix.y.x) + (vertex.z * matrix.z.x); // Multiply the first column of the matrix by column one of the vector
	vecResult.y = (vertex.x * matrix.x.y) + (vertex.y * matrix.y.y) + (vertex.z * matrix.z.y); // Multiply the second column of the matrix by column one of the vector
	vecResult.z = (vertex.x * matrix.x.z) + (vertex.y * matrix.y.z) + (vertex.z * matrix.z.z); // Multiply the third column of the matrix by column one of the vector

	// Return the result of the math
	return vecResult;
}

VECTOR4 Multiply4X4MatrixAndVector4(VECTOR4 vec, MATRIX4X4 matrix)
{
	// Used for storing results of the math and returning it to the function
	VECTOR4 vertexResult;

	vertexResult.x = (vec.x * matrix.x.x) + (vec.y * matrix.y.x) + (vec.z * matrix.z.x) + (vec.w * matrix.w.x); // Multiply the first column of the matrix by column one of the vector
	vertexResult.y = (vec.x * matrix.x.y) + (vec.y * matrix.y.y) + (vec.z * matrix.z.y) + (vec.w * matrix.w.y); // Multiply the second column of the matrix by column one of the vector
	vertexResult.z = (vec.x * matrix.x.z) + (vec.y * matrix.y.z) + (vec.z * matrix.z.z) + (vec.w * matrix.w.z); // Multiply the third column of the matrix by column one of the vector
	vertexResult.w = (vec.x * matrix.x.w) + (vec.y * matrix.y.w) + (vec.z * matrix.z.w) + (vec.w * matrix.w.w); // Multiply the fourth column of the matrix by column one of the vector

	// Return the result of the math
	return vertexResult;
}

VERTEX Multiply4X4MatrixAndVertex(VERTEX vertex, MATRIX4X4 matrix)
{
	// Used for storing results of the math and returning it to the function
	VERTEX vertexResult;

	vertexResult.x = (vertex.x * matrix.x.x) + (vertex.y * matrix.y.x) + (vertex.z * matrix.z.x) + (vertex.w * matrix.w.x); // Multiply the first column of the matrix by column one of the vector
	vertexResult.y = (vertex.x * matrix.x.y) + (vertex.y * matrix.y.y) + (vertex.z * matrix.z.y) + (vertex.w * matrix.w.y); // Multiply the second column of the matrix by column one of the vector
	vertexResult.z = (vertex.x * matrix.x.z) + (vertex.y * matrix.y.z) + (vertex.z * matrix.z.z) + (vertex.w * matrix.w.z); // Multiply the third column of the matrix by column one of the vector
	vertexResult.w = (vertex.x * matrix.x.w) + (vertex.y * matrix.y.w) + (vertex.z * matrix.z.w) + (vertex.w * matrix.w.w); // Multiply the fourth column of the matrix by column one of the vector
	vertexResult.u = vertex.u;
	vertexResult.v = vertex.v;
	vertexResult.color = vertex.color;

	// Return the result of the math
	return vertexResult;
}

MATRIX3X3 Multiply3X3Matrices(MATRIX3X3 firstMatrix, MATRIX3X3 secondMatrix)
{
	// Used for storing results of the math and returning it to the function
	MATRIX3X3 matrixResult;


	// Perform matrix math by iterating and multiplying row by column from matrix one to matrix two
	matrixResult.x.x = (firstMatrix.x.x * secondMatrix.x.x) + (firstMatrix.x.y * secondMatrix.y.x) + (firstMatrix.x.z * secondMatrix.z.x);
	matrixResult.x.y = (firstMatrix.x.x * secondMatrix.x.y) + (firstMatrix.x.y * secondMatrix.y.y) + (firstMatrix.x.z * secondMatrix.z.y);
	matrixResult.x.z = (firstMatrix.x.x * secondMatrix.x.z) + (firstMatrix.x.y * secondMatrix.y.z) + (firstMatrix.x.z * secondMatrix.z.z);

	matrixResult.y.x = (firstMatrix.y.x * secondMatrix.x.x) + (firstMatrix.y.y * secondMatrix.y.x) + (firstMatrix.y.z * secondMatrix.z.x);
	matrixResult.y.y = (firstMatrix.y.x * secondMatrix.x.y) + (firstMatrix.y.y * secondMatrix.y.y) + (firstMatrix.y.z * secondMatrix.z.y);
	matrixResult.y.z = (firstMatrix.y.x * secondMatrix.x.z) + (firstMatrix.y.y * secondMatrix.y.z) + (firstMatrix.y.z * secondMatrix.z.z);

	matrixResult.z.x = (firstMatrix.z.x * secondMatrix.x.x) + (firstMatrix.z.y * secondMatrix.y.x) + (firstMatrix.z.z * secondMatrix.z.x);
	matrixResult.z.y = (firstMatrix.z.x * secondMatrix.x.y) + (firstMatrix.z.y * secondMatrix.y.y) + (firstMatrix.z.z * secondMatrix.z.y);
	matrixResult.z.z = (firstMatrix.z.x * secondMatrix.x.z) + (firstMatrix.z.y * secondMatrix.y.z) + (firstMatrix.z.z * secondMatrix.z.z);

	// Return the result of the math
	return matrixResult;
}

MATRIX4X4 Multiply4X4Matrices(MATRIX4X4 firstMatrix, MATRIX4X4 secondMatrix)
{
	// Used for storing results of the math and returning it to the function
	MATRIX4X4 matrixResult;


	// Perform matrix math by iterating and multiplying row by column from matrix one to matrix two
	matrixResult.x.x = (firstMatrix.x.x * secondMatrix.x.x) + (firstMatrix.x.y * secondMatrix.y.x) + (firstMatrix.x.z * secondMatrix.z.x) + (firstMatrix.x.w * secondMatrix.w.x);
	matrixResult.x.y = (firstMatrix.x.x * secondMatrix.x.y) + (firstMatrix.x.y * secondMatrix.y.y) + (firstMatrix.x.z * secondMatrix.z.y) + (firstMatrix.x.w * secondMatrix.w.y);
	matrixResult.x.z = (firstMatrix.x.x * secondMatrix.x.z) + (firstMatrix.x.y * secondMatrix.y.z) + (firstMatrix.x.z * secondMatrix.z.z) + (firstMatrix.x.w * secondMatrix.w.z);
	matrixResult.x.w = (firstMatrix.x.x * secondMatrix.x.w) + (firstMatrix.x.y * secondMatrix.y.w) + (firstMatrix.x.z * secondMatrix.z.w) + (firstMatrix.x.w * secondMatrix.w.w);

	matrixResult.y.x = (firstMatrix.y.x * secondMatrix.x.x) + (firstMatrix.y.y * secondMatrix.y.x) + (firstMatrix.y.z * secondMatrix.z.x) + (firstMatrix.y.w * secondMatrix.w.x);
	matrixResult.y.y = (firstMatrix.y.x * secondMatrix.x.y) + (firstMatrix.y.y * secondMatrix.y.y) + (firstMatrix.y.z * secondMatrix.z.y) + (firstMatrix.y.w * secondMatrix.w.y);
	matrixResult.y.z = (firstMatrix.y.x * secondMatrix.x.z) + (firstMatrix.y.y * secondMatrix.y.z) + (firstMatrix.y.z * secondMatrix.z.z) + (firstMatrix.y.w * secondMatrix.w.z);
	matrixResult.y.w = (firstMatrix.y.x * secondMatrix.x.w) + (firstMatrix.y.y * secondMatrix.y.w) + (firstMatrix.y.z * secondMatrix.z.w) + (firstMatrix.y.w * secondMatrix.w.w);

	matrixResult.z.x = (firstMatrix.z.x * secondMatrix.x.x) + (firstMatrix.z.y * secondMatrix.y.x) + (firstMatrix.z.z * secondMatrix.z.x) + (firstMatrix.z.w * secondMatrix.w.x);
	matrixResult.z.y = (firstMatrix.z.x * secondMatrix.x.y) + (firstMatrix.z.y * secondMatrix.y.y) + (firstMatrix.z.z * secondMatrix.z.y) + (firstMatrix.z.w * secondMatrix.w.y);
	matrixResult.z.z = (firstMatrix.z.x * secondMatrix.x.z) + (firstMatrix.z.y * secondMatrix.y.z) + (firstMatrix.z.z * secondMatrix.z.z) + (firstMatrix.z.w * secondMatrix.w.z);
	matrixResult.z.w = (firstMatrix.z.x * secondMatrix.x.w) + (firstMatrix.z.y * secondMatrix.y.w) + (firstMatrix.z.z * secondMatrix.z.w) + (firstMatrix.z.w * secondMatrix.w.w);

	matrixResult.w.x = (firstMatrix.w.x * secondMatrix.x.x) + (firstMatrix.w.y * secondMatrix.y.x) + (firstMatrix.w.z * secondMatrix.z.x) + (firstMatrix.w.w * secondMatrix.w.x);
	matrixResult.w.y = (firstMatrix.w.x * secondMatrix.x.y) + (firstMatrix.w.y * secondMatrix.y.y) + (firstMatrix.w.z * secondMatrix.z.y) + (firstMatrix.w.w * secondMatrix.w.y);
	matrixResult.w.z = (firstMatrix.w.x * secondMatrix.x.z) + (firstMatrix.w.y * secondMatrix.y.z) + (firstMatrix.w.z * secondMatrix.z.z) + (firstMatrix.w.w * secondMatrix.w.z);
	matrixResult.w.w = (firstMatrix.w.x * secondMatrix.x.w) + (firstMatrix.w.y * secondMatrix.y.w) + (firstMatrix.w.z * secondMatrix.z.w) + (firstMatrix.w.w * secondMatrix.w.w);

	// Return the result of the math
	return matrixResult;
}

MATRIX4X4 AffineOrthogonalInverse4X4Matrix(MATRIX4X4 matrix)
{
	// Used for storing results of the math and returning it to the function
	MATRIX3X3 tempMatrix;
	VECTOR3 positionVector;
	VECTOR3 newPositionVector;

	// Fill in our 3X3 matrix with the tranpose of our passed in 4X4 matrix
	tempMatrix.x.x = matrix.x.x;
	tempMatrix.x.y = matrix.y.x;
	tempMatrix.x.z = matrix.z.x;

	tempMatrix.y.x = matrix.x.y;
	tempMatrix.y.y = matrix.y.y;
	tempMatrix.y.z = matrix.z.y;

	tempMatrix.z.x = matrix.x.z;
	tempMatrix.z.y = matrix.y.z;
	tempMatrix.z.z = matrix.z.z;

	// Fill in our position vector
	positionVector.x = matrix.w.x;
	positionVector.y = matrix.w.y;
	positionVector.z = matrix.w.z;

	// Multiply the vector and matrix
	newPositionVector = Multiply3X3MatrixAndVector3(positionVector, tempMatrix);
	
	// Negate the vector
	newPositionVector = { -newPositionVector.x, -newPositionVector.y, -newPositionVector.z };

	// Set value equal to negated value
	positionVector = newPositionVector;

	// Fill our matrixResult with the result of the operations
	MATRIX4X4 matrixResult =
	{
		{ matrixResult.x.x = tempMatrix.x.x, matrixResult.x.y = tempMatrix.x.y, matrixResult.x.z = tempMatrix.x.z, matrixResult.x.w = 0},
		{ matrixResult.y.x = tempMatrix.y.x, matrixResult.y.y = tempMatrix.y.y, matrixResult.y.z = tempMatrix.y.z, matrixResult.y.w = 0 },
		{ matrixResult.z.x = tempMatrix.z.x, matrixResult.z.y = tempMatrix.z.y, matrixResult.z.z = tempMatrix.z.z, matrixResult.z.w = 0 },
		{ matrixResult.w.x = positionVector.x, matrixResult.w.y = positionVector.y, matrixResult.w.z = positionVector.z, matrixResult.w.w = 1 }
	};

	// Return the result of the math
	return matrixResult;
}

float DegreeToRadians(float angleDegrees)
{
	// Convert our passed in angle of degrees to radians
	float angleRadians = angleDegrees * (M_PI / 180.0f);

	// Return the result
	return angleRadians;
}

MATRIX4X4 ProjectionMatrix()
{
	// Get our Y Scale
	float yScale = (1 / std::tan(DegreeToRadians(FOV) / 2));

	// Set up our projection matrix
	MATRIX4X4 projection = {
	{ yScale * ASPECT_RATIO, 0, 0, 0 },
	{ 0, yScale, 0, 0 },
	{ 0, 0, FAR_PLANE / (FAR_PLANE - NEAR_PLANE), 1 },
	{ 0, 0, ((-FAR_PLANE * NEAR_PLANE) / (FAR_PLANE - NEAR_PLANE)), 0 },
	};

	// Return the result
	return projection;
}

MATRIX4X4 Create4X4YRotationMatrix(float angleDegrees)
{
	// Our 4X4 matrix
	MATRIX4X4 yRotationMatrix;

	// Convert our passed in angle of degrees to radians
	float angleRadians = DegreeToRadians(angleDegrees);

	// Assign our matrix to be a Y rotation matrix
	yRotationMatrix.x.x = cosf(angleRadians);
	yRotationMatrix.x.y = 0;
	yRotationMatrix.x.z = sinf(angleRadians);
	yRotationMatrix.x.w = 0;

	yRotationMatrix.y.x = 0;
	yRotationMatrix.y.y = 1;
	yRotationMatrix.y.z = 0;
	yRotationMatrix.y.w = 0;

	yRotationMatrix.z.x = -sinf(angleRadians);
	yRotationMatrix.z.y = 0;
	yRotationMatrix.z.z = cosf(angleRadians);
	yRotationMatrix.z.w = 0;

	yRotationMatrix.w.x = 0;
	yRotationMatrix.w.y = 0;
	yRotationMatrix.w.z = 0;
	yRotationMatrix.w.w = 1;

	// Return the result
	return yRotationMatrix;
}

MATRIX4X4 Create4X4XRotationMatrix(float angleDegrees)
{
	// Our 4X4 matrix
	MATRIX4X4 xRotationMatrix;

	// Convert our passed in angle of degrees to radians
	float angleRadians = DegreeToRadians(angleDegrees);

	// Assign our matrix to be a X rotation matrix
	xRotationMatrix.x.x = 1;
	xRotationMatrix.x.y = 0;
	xRotationMatrix.x.z = 0;
	xRotationMatrix.x.w = 0;

	xRotationMatrix.y.x = 0;
	xRotationMatrix.y.y = cosf(angleRadians);
	xRotationMatrix.y.z = -sinf(angleRadians);
	xRotationMatrix.y.w = 0;

	xRotationMatrix.z.x = 0;
	xRotationMatrix.z.y = sinf(angleRadians);
	xRotationMatrix.z.z = cosf(angleRadians);
	xRotationMatrix.z.w = 0;

	xRotationMatrix.w.x = 0;
	xRotationMatrix.w.y = 0;
	xRotationMatrix.w.z = 0;
	xRotationMatrix.w.w = 1;

	// Return the result
	return xRotationMatrix;
}

MATRIX4X4 Create4X4ZRotationMatrix(float angleDegrees)
{
	// Our 4X4 matrix
	MATRIX4X4 xRotationMatrix;

	// Convert our passed in angle of degrees to radians
	float angleRadians = DegreeToRadians(angleDegrees);

	// Assign our matrix to be a X rotation matrix
	xRotationMatrix.x.x = cosf(angleRadians);
	xRotationMatrix.x.y = -sinf(angleRadians);
	xRotationMatrix.x.z = 0;
	xRotationMatrix.x.w = 0;

	xRotationMatrix.y.x = sinf(angleRadians);
	xRotationMatrix.y.y = cosf(angleRadians);
	xRotationMatrix.y.z = 0;
	xRotationMatrix.y.w = 0;

	xRotationMatrix.z.x = 0;
	xRotationMatrix.z.y = 0;
	xRotationMatrix.z.z = 1;
	xRotationMatrix.z.w = 0;

	xRotationMatrix.w.x = 0;
	xRotationMatrix.w.y = 0;
	xRotationMatrix.w.z = 0;
	xRotationMatrix.w.w = 1;

	// Return the result
	return xRotationMatrix;
}

MATRIX4X4 MakeTranslationMatrix(float x, float y, float z)
{
	// Create our 4X4 translation matrix
	MATRIX4X4 matrix = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ x, y, z, 1 },
	};

	// Return the result
	return matrix;
}

uint ConvertToARGB(uint inputColor)
{
	return (inputColor & 0x000000FF) << 24 | (inputColor & 0x0000FF00) << 8 | (inputColor & 0x00FF0000) >> 8 | (inputColor & 0xFF000000) >> 24; // Convert from ABGR to ARGB
}

float Berp(float a, float b, float c, VECTOR3 barycentricCoordinates)
{
	return (a * barycentricCoordinates.x) + (b * barycentricCoordinates.y) + (c * barycentricCoordinates.z); // Barycentrically interpolate
}

float Lerp(float startingValue, float endingValue, float ratio)
{
	return (endingValue - startingValue) * ratio + startingValue; // Linear interpolation
}
