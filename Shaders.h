#pragma once
#include "MathFunctions.h"

// The active vertex shader. Modifies an incoming vertex. Pre-Rasterization. 
void (*VertexShader)(VERTEX&) = 0;
// The active pixel shader. Modifies an outgoing pixel. Post-Rasterization.
void (*PixelShader)(float, float, uint&) = 0;

// All Shader Variables (Always Pre-fixed by “SV_”)
MATRIX4X4 SV_WorldMatrix;
MATRIX4X4 SV_ViewMatrix;
MATRIX4X4 SV_ProjectionMatrix;
unsigned int* SV_TextureArr;
unsigned int SV_TextureHeight;
unsigned int SV_TextureWidth;

// Various custom vertex and pixel shaders, (Pre-fixed by “VS_” & “PS_”)
// Can be swapped using above function pointers as needed for flexibility. 

// Applys the current world matrix to all
void VS_World(VERTEX& multiplyMe)
{
    multiplyMe = Multiply4X4MatrixAndVertex(multiplyMe, SV_WorldMatrix);
}

// Basic pixel shader returns the color white
void PS_White(uint& makeWhite)
{
    makeWhite = 0xFFFFFFFF;
}

// Function to handle the perspective
void VS_Perspective(VERTEX& vertex)
{
    vertex = Multiply4X4MatrixAndVertex(vertex, SV_WorldMatrix);
    vertex = Multiply4X4MatrixAndVertex(vertex, SV_ViewMatrix);
    vertex = Multiply4X4MatrixAndVertex(vertex, SV_ProjectionMatrix);
    PerspectiveDivide(vertex);
}

// Texture our cube with the converted .tga file
void PS_TextureCube(float u, float v, uint& color)
{
    int texelU = u * SV_TextureWidth;
    int texelV = v * SV_TextureHeight;
    int coordinate = MakeOneDimenCoord(texelU, texelV, SV_TextureWidth);
    color = ConvertToARGB(SV_TextureArr[coordinate]);
}