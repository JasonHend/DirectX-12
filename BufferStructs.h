#pragma once
#include <DirectXMath.h>
#include "Lights.h"

// Struct that defines basic data to pass into the vertex shader
struct VertexShaderExternalData
{
	DirectX::XMFLOAT4X4 m4World;
	DirectX::XMFLOAT4X4 m4View;
	DirectX::XMFLOAT4X4 m4Projection;
	DirectX::XMFLOAT4X4 m4WorldInvTranspose;
};

// Struct that defines basic data that will be sent to the pixel shader
struct PixelShaderExternalData
{
	unsigned int albedo;
	unsigned int normal;
	unsigned int metal;
	unsigned int roughness;
	unsigned int height;
	DirectX::XMFLOAT3 pad;
	DirectX::XMFLOAT2 uvScale;
	DirectX::XMFLOAT2 uvOffset;
	DirectX::XMFLOAT3 cameraPosition;
	unsigned int lightCount;
	Light lights[3];
};

struct VertexShaderFrameData
{
	DirectX::XMFLOAT4X4 m4View;
	DirectX::XMFLOAT4X4 m4Projection;
};