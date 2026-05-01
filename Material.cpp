#include "Material.h"

Material::Material(
	DirectX::XMFLOAT3 colorTint,
	DirectX::XMFLOAT2 uvScale,
	DirectX::XMFLOAT2 uvOffset,
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState,
	unsigned int albedo,
	unsigned int normalMap,
	unsigned int metalness,
	unsigned int roughness,
	unsigned int height) :
	colorTint(colorTint),
	uvScale(uvScale),
	uvOffset(uvOffset),
	pipelineState(pipelineState),
	albedo(albedo),
	normalMap(normalMap),
	metalness(metalness),
	roughness(roughness),
	height(height)
{
}

DirectX::XMFLOAT3 Material::GetColorTint() { return colorTint; }
DirectX::XMFLOAT2 Material::GetUVScale() { return uvScale; }
DirectX::XMFLOAT2 Material::GetUVOffset() { return uvOffset; }
Microsoft::WRL::ComPtr<ID3D12PipelineState> Material::GetPipelineState() { return pipelineState; }

unsigned int Material::GetAlbedo() { return albedo; }
unsigned int Material::GetNormalMap() { return normalMap; }
unsigned int Material::GetMetalness() { return metalness; }
unsigned int Material::GetRoughness() { return roughness; }
unsigned int Material::GetHeight() { return height; }

void Material::SetColorTint(DirectX::XMFLOAT3 colorTint) { this->colorTint = colorTint; }
void Material::SetColorTint(float r, float g, float b) { colorTint = DirectX::XMFLOAT3(r, g, b); }
void Material::SetUVScale(DirectX::XMFLOAT2 uvScale) { this->uvOffset = uvScale; }
void Material::SetUVScale(float x, float y) { uvScale = DirectX::XMFLOAT2(x, y); }
void Material::SetUVOffSet(DirectX::XMFLOAT2 uvOffset) { this->uvOffset = uvOffset; }
void Material::SetUVOffset(float x, float y) { uvOffset = DirectX::XMFLOAT2(x, y); }

void Material::SetAlbedo(unsigned int albedo) { this->albedo = albedo; }
void Material::SetNormalMap(unsigned int normalMap) { this->normalMap = normalMap; }
void Material::SetMetalness(unsigned int metalness) { this->metalness = metalness; }
void Material::SetRoughness(unsigned int roughness) { this->roughness = roughness; }
void Material::SetHeight(unsigned int height) { this->height = height; }

