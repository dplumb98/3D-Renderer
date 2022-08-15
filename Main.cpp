#include "Rasterizer.h"

XTime timer; // Define our timer

int main()
{
	RS_Initialize("Devon Plumb", RASTER_WIDTH, RASTER_HEIGHT); // Initialize our Raster Surface

	// Set our Vertex Shader to VS_Perspective
	VertexShader = VS_Perspective;

	// Set up our matrices
	MATRIX4X4 gridMatrix = identity4X4;
	MATRIX4X4 cubeTranslation = MakeTranslationMatrix(0, 0.25, 0);

	// Set up our camera matrix
	MATRIX4X4 camera = MakeTranslationMatrix(0, 0, -1);
	MATRIX4X4 rotation = Create4X4XRotationMatrix(-18);
	camera = Multiply4X4Matrices(camera, rotation);

	// Setup our SV_'s for the greendragon header file
	SV_TextureArr = greendragon_pixels;
	SV_TextureHeight = greendragon_height;
	SV_TextureWidth = greendragon_width;

	do
	{
		timer.Signal(); // Recalculate time every loop iteration
		ClearColorBuffer(0); // Clear our color buffer every loop iteration to prevent color carrying over
		ClearZBuffer(FAR_PLANE); // Clear our zBuffer back to the far plane every loop iteration to reset our zBuffer for rendering

		// Set our camera and projection up to give depth and 3D look to our Raster Surface
		SV_ViewMatrix = AffineOrthogonalInverse4X4Matrix(camera);
		SV_ProjectionMatrix = ProjectionMatrix();

		// Use the Pixel Shader and function call to texture our cube
		PixelShader = PS_TextureCube;

		// Set our SV_WorldMatrix to our gridMatrix for rendering
		SV_WorldMatrix = gridMatrix;

		// Loop over our grid buffer and render our grid
		DrawGrid();
		
		// Create our cube matrix based off of total time, a translation matrix, and a Y rotation matrix so our cube will rotate
		MATRIX4X4 cubeMatrix = Multiply4X4Matrices(cubeTranslation, Create4X4YRotationMatrix(static_cast<float>(timer.TotalTime()) * 60));

		// Set our SV_WorldMatrix to our cubeMatrix for rendering
		SV_WorldMatrix = cubeMatrix;

		// Loop over our cube buffer and render our cube
		DrawCubeTexture();

	} while (RS_Update(pixelArr, NUM_PIXELS));

	RS_Shutdown(); // Shutdown our Raster Surface
}