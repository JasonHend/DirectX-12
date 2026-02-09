#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct Vertex
{
	DirectX::XMFLOAT3 Position;	    // The local position of the vertex
	DirectX::XMFLOAT2 UV;			// UV data for the vertex
	DirectX::XMFLOAT3 Normal;		// Which direction is the vertex facing
	DirectX::XMFLOAT3 Tangent;		// Tangent to the normal of the vertex
};