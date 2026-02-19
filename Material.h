#pragma once

#include <d3d12.h>
#include <memory>
#include <wrl/client.h>
#include "DirectXMath.h"

class Material
{
public:
	// Constructor
	Material(
		DirectX::XMFLOAT3 colorTint,
		DirectX::XMFLOAT2 uvScale,
		DirectX::XMFLOAT2 uvOffset,
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState,
		unsigned int albedo,
		unsigned int normalMap,
		unsigned int metalness,
		unsigned int roughness
	);

	// Getters
	DirectX::XMFLOAT3 GetColorTint();
	DirectX::XMFLOAT2 GetUVScale();
	DirectX::XMFLOAT2 GetUVOffset();
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState();
	unsigned int GetAlbedo();
	unsigned int GetNormalMap();
	unsigned int GetMetalness();
	unsigned int GetRoughness();

	// Setters
	void SetColorTint(DirectX::XMFLOAT3 colorTint);
	void SetColorTint(float r, float g, float b);
	void SetUVScale(DirectX::XMFLOAT2 uvScale);
	void SetUVScale(float x, float y);
	void SetUVOffSet(DirectX::XMFLOAT2 uvOffset);
	void SetUVOffset(float x, float y);
	void SetAlbedo(unsigned int albedo);
	void SetNormalMap(unsigned int normalMap);
	void SetMetalness(unsigned int metalness);
	void SetRoughness(unsigned int roughness);


private:
	// Minimum data tracking
	DirectX::XMFLOAT3 colorTint;
	DirectX::XMFLOAT2 uvScale;
	DirectX::XMFLOAT2 uvOffset;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

	// Texture data
	unsigned int albedo;
	unsigned int normalMap;
	unsigned int metalness;
	unsigned int roughness;
};

