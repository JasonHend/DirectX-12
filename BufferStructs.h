#pragma once
#include <DirectXMath.h>

// Struct that defines basic data to pass into the vertex shader
struct VertexShaderExternalData
{
	DirectX::XMFLOAT4X4 m4World;
	DirectX::XMFLOAT4X4 m4View;
	DirectX::XMFLOAT4X4 m4Projection;
	DirectX::XMFLOAT4X4 m4WorldInvTranspose;
};

// Struct that defines basic data that will be sent to the pixel shader
struct PixelDataExternalData
{
	unsigned int albedo;
	unsigned int normal;
	unsigned int metal;
	unsigned int roughness;
	DirectX::XMFLOAT2 uvScale;
	DirectX::XMFLOAT2 uvOffset;
};