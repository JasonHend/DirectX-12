#pragma once
#include <wrl/client.h>
#include "Mesh.h"
#include "Camera.h"
#include "memory"

// Struct for bindless resources
struct SkyDrawResources
{
	unsigned int vsVertexBufferIndex;
	unsigned int vsCBIndex;
	unsigned int psSkyboxIndex;
};

// Everything necessary to render a skybox
class Sky
{
public:
	Sky(
		const wchar_t* right,
		const wchar_t* left,
		const wchar_t* up,
		const wchar_t* down,
		const wchar_t* front,
		const wchar_t* back,
		std::shared_ptr<Mesh> mesh);
	
	~Sky();
	
	void Draw(std::shared_ptr<Camera> currentCamera);

private:
	// Helper functions
	void InitializeRendering();

	// States and shader resource view
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

	// Mesh and shaders
	std::shared_ptr<Mesh> mesh;

	unsigned int skyboxDescriptor;
};