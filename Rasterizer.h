#pragma once
#include "Shaders.h"

void DrawPixel(unsigned int arr[], unsigned int index, unsigned int color)
{
	arr[index] = color; // Draw a pixel to the Raster Surface
}

void ClearColorBuffer(unsigned int color)
{
	// Iterate through our buffer and clear it
	for (int i = 0; i < NUM_PIXELS; i++)
	{
		pixelArr[i] = color;
	}
}

void ClearZBuffer(float number)
{
	// Iterate through our buffer and clear it
	for (int i = 0; i < NUM_PIXELS; i++)
	{
		zBuffer[i] = number;
	}
}

void PlotPixel(const VERTEX& vertex)
{
	// If we're within bounds, plot the pixel on the Raster Surface
	if (vertex.x >= 0 && vertex.x < RASTER_WIDTH && vertex.y >= 0 && vertex.y < RASTER_HEIGHT)
	{
		int coordinate = MakeOneDimenCoord(vertex.x, vertex.y, RASTER_WIDTH); // Convert 2D coordinate to 1D

		 // Check if the vertex.z is less than our zBuffer so we draw in bounds
		if (vertex.z < zBuffer[coordinate])
		{
			zBuffer[coordinate] = vertex.z; // Plot on zBuffer
			pixelArr[coordinate] = vertex.color; // Plot on Raster Surface
		}
	}
}

void FillTriangle(const VERTEX& vertex01, const VERTEX& vertex02, const VERTEX& vertex03)
{
	// Create copies of our passed in const vertices
	VERTEX vertex01_COPY = vertex01;
	VERTEX vertex02_COPY = vertex02;
	VERTEX vertex03_COPY = vertex03;

	// Convert our screen space coordinates
	vertex01_COPY = GetScreenPositionOfVertexTOVERTEX(vertex01_COPY);
	vertex02_COPY = GetScreenPositionOfVertexTOVERTEX(vertex02_COPY);
	vertex03_COPY = GetScreenPositionOfVertexTOVERTEX(vertex03_COPY);

	// Find the min of X and Y and the max of X and Y
	int minX = static_cast<int>(min(min(vertex01_COPY.x, vertex02_COPY.x), vertex03_COPY.x));
	int minY = static_cast<int>(min(min(vertex01_COPY.y, vertex02_COPY.y), vertex03_COPY.y));
	int maxX = static_cast<int>(max(max(vertex01_COPY.x, vertex02_COPY.x), vertex03_COPY.x));
	int maxY = static_cast<int>(max(max(vertex01_COPY.y, vertex02_COPY.y), vertex03_COPY.y));

	// Iterate through to plot our pixels
	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			VECTOR3 barycentricCoord; // Define a VECTOR3 for our barycentric coordinate and find out barycentric coordinate below
			barycentricCoord = FindBarycentricCoordinates((VECTOR2)vertex01_COPY, (VECTOR2)vertex02_COPY, (VECTOR2)vertex03_COPY, { static_cast<float>(x) * 1.0f, static_cast<float>(y) * 1.0f });

			// If the barycentric coordinate is out of bounds, reset to the top of the nested loop
			if (barycentricCoord.x < 0 || barycentricCoord.x > 1 || barycentricCoord.y < 0 || barycentricCoord.y > 1 || barycentricCoord.z < 0 || barycentricCoord.z > 1)
			{
				continue;
			}

			// Use Berp to find each vector component after being barycentrically interpolated
			float berpResultZ = Berp(vertex01_COPY.z, vertex02_COPY.z, vertex03_COPY.z, barycentricCoord);
			float berpResultU = Berp(vertex01_COPY.u, vertex02_COPY.u, vertex03_COPY.u, barycentricCoord);
			float berpResultV = Berp(vertex01_COPY.v, vertex02_COPY.v, vertex03_COPY.v, barycentricCoord);

			// Use the Pixel Shader to set the color of the vertex
			uint pixel01;
			pixel01 = COLOR_WHITE;
			if (PixelShader)
			{
				PixelShader(berpResultU, berpResultV, pixel01);
			}

			// Plot the pixel using the values gathered during the function
			PlotPixel(VERTEX(x, y, berpResultZ, 0, pixel01));
		}
	}
}

// This function will need adjusted for handling Vector/Matrix math I believe
void ParametricLineEquation(const VERTEX& startVertex, const VERTEX& endVertex)
{
	// Copy our passed in vertices to new vertices
	VERTEX startVertex_COPY = startVertex;
	VERTEX endVertex_COPY = endVertex;

	// Use our Vertex Shaders to modify the copies
	if (VertexShader)
	{
		VertexShader(startVertex_COPY);
		VertexShader(endVertex_COPY);
	}

	// Get the screen position of the vertices
	VERTEX newVertex01 = GetScreenPositionOfVertexTOVERTEX(startVertex_COPY);
	VERTEX newVertex02 = GetScreenPositionOfVertexTOVERTEX(endVertex_COPY);

	// Define our variables for math operations
	int ratio;
	float xLerp;
	float yLerp;
	float zLerp;

	// Grab the delta of x and y and do an absolute operation on it
	int deltaX = std::abs(newVertex02.x - newVertex01.x);
	int deltaY = std::abs(newVertex02.y - newVertex01.y);

	// Determine what our ratio should be
	if (deltaY < deltaX)
	{
		ratio = deltaX;
	}
	else
	{
		ratio = deltaY;
	}

	for (unsigned int i = 0; i <= ratio; i++)
	{
		// Grab the lerp of each component
		float newI = i / static_cast<float>(ratio);
		xLerp = (newVertex02.x - newVertex01.x) * newI + newVertex01.x + 0.5f;
		yLerp = (newVertex02.y - newVertex01.y) * newI + newVertex01.y + 0.5f;
		zLerp = Lerp(newVertex01.z, newVertex02.z, newI); // Not needed now, needed in next lab

		// Use the Pixel Shader to set the color of the vertex
		uint pixel01;
		pixel01 = COLOR_WHITE;

		// Plot the pixel
		PlotPixel(VERTEX(xLerp, yLerp, zLerp, 1, pixel01));
	}
}

void DrawTriangle(const VERTEX& startVertex, const VERTEX& middleVertex, const VERTEX& endVertex)
{
	// Create copies of our passed in const vertices
	VERTEX startVertex_COPY = startVertex;
	VERTEX middleVertex_COPY = middleVertex;
	VERTEX endVertex_COPY = endVertex;
	VERTEX blitCheck_COPY;

	// Use our Vertex Shaders to modify the copies
	if (VertexShader)
	{
		VertexShader(startVertex_COPY);
		VertexShader(middleVertex_COPY);
		VertexShader(endVertex_COPY);
	}

	// Call FillTriangle so we can render our cube properly
	FillTriangle(startVertex_COPY, middleVertex_COPY, endVertex_COPY);
}

void DrawGrid()
{
	for (int i = 0; i < std::size(indexBuffer02); i += 2)
	{
		ParametricLineEquation(gridVertices[indexBuffer02[i]], gridVertices[indexBuffer02[i + 1]]); // Render our grid to the screen
	}
}

void DrawCubeWireFrame()
{
	for (int i = 0; i < std::size(indexBuffer); i += 2)
	{
		DrawTriangle(cubeVertices[indexBufferTriangle[i]], cubeVertices[indexBufferTriangle[i + 1]], cubeVertices[indexBufferTriangle[i + 2]]); // Render our wireframe cube to the screen
	}
}

void DrawCubeTexture()
{
	// Iterate over our index buffer for our triangle and apply our UV's properly
	for (int i = 0; i < std::size(indexBufferTriangle); i += 3)
	{
		if (i % 2 == 0)
		{
			cubeVertices[indexBufferTriangle[i]].u = indexBufferUV[0];
			cubeVertices[indexBufferTriangle[i]].v = indexBufferUV[1];
			cubeVertices[indexBufferTriangle[i + 1]].v = indexBufferUV[2];
			cubeVertices[indexBufferTriangle[i + 1]].u = indexBufferUV[3];
			cubeVertices[indexBufferTriangle[i + 2]].v = indexBufferUV[4];
			cubeVertices[indexBufferTriangle[i + 2]].u = indexBufferUV[5];
		}
		else
		{
			cubeVertices[indexBufferTriangle[i]].u = indexBufferUV[6];
			cubeVertices[indexBufferTriangle[i]].v = indexBufferUV[7];
			cubeVertices[indexBufferTriangle[i + 1]].v = indexBufferUV[8];
			cubeVertices[indexBufferTriangle[i + 1]].u = indexBufferUV[9];
			cubeVertices[indexBufferTriangle[i + 2]].v = indexBufferUV[10];
			cubeVertices[indexBufferTriangle[i + 2]].u = indexBufferUV[11];
		}
		DrawTriangle(cubeVertices[indexBufferTriangle[i]], cubeVertices[indexBufferTriangle[i + 1]], cubeVertices[indexBufferTriangle[i + 2]]); // Render our triangle
	}
}

void BresenhamLineAlgorithm(unsigned int arr[], int startY, int startX, int endY, int endX)
{
	// Calculate slope
	int deltaY = endY - startY;
	int deltaX = endX - startX;

	// If delta x is a negative number, we swap starting and ending values for both axes
	if (deltaX < 0)
	{
		// Swap x values
		int tempNum = startX;
		startX = endX;
		endX = tempNum;

		// Swap y values
		tempNum = startY;
		startY = endY;
		endY = tempNum;
	}

	int slope; // Define our slope variable

	// Variables for keeping track of currentY and currentX as well as setting up error as 0
	int currentY = startY;
	int currentX = startX;
	int error = 0;

	if (deltaX >= deltaY) // If line is not steep
	{
		slope = deltaX / deltaY; // Calculate our slope
		for (int i = startX; i < endX; i++)
		{
			int coordinate = MakeOneDimenCoord(i, currentY, RASTER_WIDTH); // Turn our current iteration of x and y into a 1D coordinate
			DrawPixel(arr, coordinate, COLOR_WHITE); // Draw a pixel of the line at our current iteration
			error += slope; // Increment our error by our slope
			if (error < -0.5)
			{
				// If our margin of error passes the threshold, adjust our currentY and error to continue the algorithm
				currentY -= 1;
				error += 1;
			}
			if (error > 0.5)
			{
				// If our margin of error passes the threshold, adjust our currentY and error to continue the algorithm
				currentY += 1;
				error -= 1;
			}
		}
	}
	else // Steep line
	{
		slope = deltaY / deltaX; // Calculate our slope
		for (int i = startY; i < endY; i++)
		{
			int coordinate = MakeOneDimenCoord(currentX, i, RASTER_WIDTH); // Turn our current iteration of x and y into a 1D coordinate
			DrawPixel(arr, coordinate, COLOR_WHITE); // Draw a pixel of the line at our current iteration
			error += slope; // Increment our error by our slope
			if (error < -0.5)
			{
				// If our margin of error passes the threshold, adjust our currentY and error to continue the algorithm
				currentX -= 1;
				error += 1;
			}
			if (error > 0.5)
			{
				// If our margin of error passes the threshold, adjust our currentY and error to continue the algorithm
				currentX += 1;
				error -= 1;
			}
		}
	}
}

void DrawLine(const VERTEX& startVertex, const VERTEX& endVertex)
{
	// Copy our passed in vertices to new vertices
	VERTEX startVertex_COPY = startVertex;
	VERTEX endVertex_COPY = endVertex;

	// Use our Vertex Shaders to modify the copies
	if (VertexShader)
	{
		VertexShader(startVertex_COPY);
		VertexShader(endVertex_COPY);
	}

	// Calculate screen space
	SCREEN_XY screenStart = GetScreenPositionOfVertexTOSCREENXY(startVertex_COPY);
	SCREEN_XY screenEnd = GetScreenPositionOfVertexTOSCREENXY(endVertex_COPY);

	for (NUM_PIXELS;;)
	{
		// Make a copy of the Pixel Data just like with the vertex
		uint pixel_COPY;
		pixel_COPY = COLOR_GREEN;
		if (PixelShader)
		{
			PixelShader(0, 0, pixel_COPY); // Modify our copy
		}

		// Draw our points to the Raster
		BresenhamLineAlgorithm(pixelArr, startVertex_COPY.y, startVertex_COPY.x, endVertex_COPY.y, endVertex_COPY.x);
	}
}