#pragma once
#include <DirectXMath.h>

// Struct that defines basic data to pass into the vertex shader
struct VertexShaderExternalData
{
	DirectX::XMFLOAT4X4 m4World;
	DirectX::XMFLOAT4X4 m4View;
	DirectX::XMFLOAT4X4 m4Projection;
};