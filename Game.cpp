#include "Game.h"
#include "Graphics.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "Window.h"
#include "BufferStructs.h"

#include <DirectXMath.h>

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// The constructor is called after the window and graphics API
// are initialized but before the game loop begins
// --------------------------------------------------------
Game::Game()
{
	// Create the main camera
	XMFLOAT3 initialPos = XMFLOAT3(0.0f, 0.0f, -15.0f);
	XMFLOAT3 startOrientation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mainCamera = std::make_shared<Camera>(
		Window::AspectRatio(),
		initialPos,
		startOrientation,
		120.0f,
		0.1f,
		1000.0f,
		10.0f,
		10.0f);

	CreateRootSigAndPipelineState();
	CreateGeometry();
	CreateLights();
}


// --------------------------------------------------------
// Clean up memory or objects created by this class
// 
// Note: Using smart pointers means there probably won't
//       be much to manually clean up here!
// --------------------------------------------------------
Game::~Game()
{
	// Wait for the GPU before we shut down
	Graphics::WaitForGPU();
}


// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void Game::CreateGeometry()
{
	// Load in primative mesh objects
	std::shared_ptr<Mesh> cube = std::make_shared<Mesh>(FixPath(L"../../Assets/Models/cube.obj").c_str());
	std::shared_ptr<Mesh> cylinder = std::make_shared<Mesh>(FixPath(L"../../Assets/Models/cylinder.obj").c_str());
	std::shared_ptr<Mesh> helix = std::make_shared<Mesh>(FixPath(L"../../Assets/Models/helix.obj").c_str());
	std::shared_ptr<Mesh> sphere = std::make_shared<Mesh>(FixPath(L"../../Assets/Models/sphere.obj").c_str());
	std::shared_ptr<Mesh> torus = std::make_shared<Mesh>(FixPath(L"../../Assets/Models/torus.obj").c_str());
	std::shared_ptr<Mesh> quad = std::make_shared<Mesh>(FixPath(L"../../Assets/Models/quad.obj").c_str());
	std::shared_ptr<Mesh> quad2Side = std::make_shared<Mesh>(FixPath(L"../../Assets/Models/quad_double_sided.obj").c_str());

	// Load in textures
	unsigned int rockAlbedo = Graphics::LoadTexture(FixPath(L"../../Assets/Textures/Rock/albedo.png").c_str());
	unsigned int rockNormal = Graphics::LoadTexture(FixPath(L"../../Assets/Textures/Rock/normal.png").c_str());
	unsigned int rockMetal = Graphics::LoadTexture(FixPath(L"../../Assets/Textures/Rock/metal.png").c_str());
	unsigned int rockRoughness = Graphics::LoadTexture(FixPath(L"../../Assets/Textures/Rock/roughness.png").c_str());
	unsigned int rockHeight = Graphics::LoadTexture(FixPath(L"../../Assets/Textures/Rock/height.png").c_str());

	// Create Materials
	std::shared_ptr<Material> rockMaterial = std::make_shared<Material>(
		DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT2(1.0f, 1.0f),
		DirectX::XMFLOAT2(0.0f, 0.0f),
		pipelineState,
		rockAlbedo,
		rockNormal,
		rockMetal,
		rockRoughness,
		rockHeight);

	// Create entities
	std::shared_ptr<GameEntity> sphereEntity = std::make_shared<GameEntity>(cube, rockMaterial);
	std::shared_ptr<GameEntity> helixEntity = std::make_shared<GameEntity>(helix, rockMaterial);
	std::shared_ptr<GameEntity> torusEntity = std::make_shared<GameEntity>(torus, rockMaterial);

	sphereEntity->GetTransform()->SetPosition(-3.0f, 0.0f, 0.0f);
	helixEntity->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	torusEntity->GetTransform()->SetPosition(3.0f, 0.0f, 0.0f);

	entities.push_back(sphereEntity);
	entities.push_back(helixEntity);
	entities.push_back(torusEntity);

	// Create the sky
	sky = std::make_shared<Sky>(
		FixPath(L"../../Assets/Textures/Skybox/right.png").c_str(),
		FixPath(L"../../Assets/Textures/Skybox/left.png").c_str(),
		FixPath(L"../../Assets/Textures/Skybox/up.png").c_str(),
		FixPath(L"../../Assets/Textures/Skybox/down.png").c_str(),
		FixPath(L"../../Assets/Textures/Skybox/front.png").c_str(),
		FixPath(L"../../Assets/Textures/Skybox/back.png").c_str(),
		cube);
}


/// <summary>
/// Create all lights that will be sent to objects for rendering
/// </summary>
void Game::CreateLights()
{
	// Fill out light structures and push them to the vector
	directionalLight = {};
	directionalLight.type = LIGHT_TYPE_DIRECTIONAL;
	directionalLight.direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
	directionalLight.range = 15.0f;
	directionalLight.position = XMFLOAT3(0.0f, 10.0f, 0.0f);
	directionalLight.intensity = 2.0f;
	directionalLight.color = XMFLOAT3(0.502f, 0.502f, 0.502f);

	pointLight = {};
	pointLight.type = LIGHT_TYPE_POINT;
	pointLight.range = 5.0f;
	pointLight.position = XMFLOAT3(-5.0f, 0.0f, 0.0f);
	pointLight.intensity = 1.75f;
	pointLight.color = XMFLOAT3(1.0f, 1.0f, 1.0f);

	spotLight = {};
	spotLight.type = LIGHT_TYPE_SPOT;
	spotLight.direction = XMFLOAT3(0.5f, -0.75f, -0.3f);
	spotLight.range = 10.0f;
	spotLight.position = XMFLOAT3(-2.0f, 15.0f, 2.0f);
	spotLight.intensity = 3.0f;
	spotLight.spotInnerAngle = 30.0f;
	spotLight.spotOuterAngle = 58.0f;
	spotLight.color = XMFLOAT3(0.2f, 0.7f, 0.6f);

	lights.push_back(directionalLight);
	lights.push_back(pointLight);
	lights.push_back(spotLight);
	lights.resize(numLights);
}


// --------------------------------------------------------
// Loads the two basic shaders, then creates the root signature
// and pipeline state object for our very basic demo.
// --------------------------------------------------------
void Game::CreateRootSigAndPipelineState()
{
	// Blobs to hold raw shader byte code used in several steps below
	Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderByteCode;
	Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderByteCode;
	// Load shaders
	{
		// Read our compiled vertex shader code into a blob
		// - Essentially just "open the file and plop its contents here"
		D3DReadFileToBlob(
			FixPath(L"VertexShader.cso").c_str(), vertexShaderByteCode.GetAddressOf());
		D3DReadFileToBlob(
			FixPath(L"PixelShader.cso").c_str(), pixelShaderByteCode.GetAddressOf());
	}
	// Input layout
	const unsigned int inputElementCount = 4;
	D3D12_INPUT_ELEMENT_DESC inputElements[inputElementCount] = {};
	{
		inputElements[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // R32 G32 B32 = float3
		inputElements[0].SemanticName = "POSITION"; // Name must match semantic in shader
		inputElements[0].SemanticIndex = 0; // This is the first POSITION semantic

		inputElements[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElements[1].Format = DXGI_FORMAT_R32G32_FLOAT; // R32 G32 = float2
		inputElements[1].SemanticName = "TEXCOORD";
		inputElements[1].SemanticIndex = 0; // This is the first TEXCOORD semantic

		inputElements[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElements[2].Format = DXGI_FORMAT_R32G32B32_FLOAT; // R32 G32 B32 = float3
		inputElements[2].SemanticName = "NORMAL";
		inputElements[2].SemanticIndex = 0; // This is the first NORMAL semantic

		inputElements[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElements[3].Format = DXGI_FORMAT_R32G32B32_FLOAT; // R32 G32 B32 = float3
		inputElements[3].SemanticName = "TANGENT";
		inputElements[3].SemanticIndex = 0; // This is the first TANGENT semantic
	}
	// Root Signature
	{
		// Define a table of constant buffer views (CBV's)
		D3D12_DESCRIPTOR_RANGE cbvTable = {};
		cbvTable.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		cbvTable.NumDescriptors = 1;
		cbvTable.BaseShaderRegister = 0;
		cbvTable.RegisterSpace = 0;
		cbvTable.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// Define a root parameter to create root signature
		D3D12_ROOT_PARAMETER rootParam = {};
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		rootParam.DescriptorTable.NumDescriptorRanges = 1;
		rootParam.DescriptorTable.pDescriptorRanges = &cbvTable;

		// New root parameter for pixel shader visibility
		D3D12_ROOT_PARAMETER psRootParam = {};
		psRootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		psRootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		psRootParam.DescriptorTable.NumDescriptorRanges = 1;
		psRootParam.DescriptorTable.pDescriptorRanges = &cbvTable;

		D3D12_ROOT_PARAMETER rootParams[] = { rootParam, psRootParam };

		// Create a single static sampler (available to all pixel shaders)
		D3D12_STATIC_SAMPLER_DESC anisoWrap = {};
		anisoWrap.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		anisoWrap.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		anisoWrap.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		anisoWrap.Filter = D3D12_FILTER_ANISOTROPIC;
		anisoWrap.MaxAnisotropy = 16;
		anisoWrap.MaxLOD = D3D12_FLOAT32_MAX;
		anisoWrap.ShaderRegister = 0; // Means register(s0) in the shader
		anisoWrap.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		D3D12_STATIC_SAMPLER_DESC samplers[] = { anisoWrap };

		// Describe the full root signature
		D3D12_ROOT_SIGNATURE_DESC rootSig = {};
		rootSig.Flags = 
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED;
		rootSig.NumParameters = ARRAYSIZE(rootParams);
		rootSig.pParameters = rootParams;
		rootSig.NumStaticSamplers = ARRAYSIZE(samplers);
		rootSig.pStaticSamplers = samplers;
		ID3DBlob* serializedRootSig = 0;
		ID3DBlob* errors = 0;
		D3D12SerializeRootSignature(
			&rootSig,
			D3D_ROOT_SIGNATURE_VERSION_1,
			&serializedRootSig,
			&errors);

		// Check for errors during serialization
		if (errors != 0)
		{
			OutputDebugString((wchar_t*)errors->GetBufferPointer());
		}

		// Actually create the root sig
		Graphics::Device->CreateRootSignature(
			0,
			serializedRootSig->GetBufferPointer(),
			serializedRootSig->GetBufferSize(),
			IID_PPV_ARGS(rootSignature.GetAddressOf()));
	}
	// Pipeline state
	{
		// Describe the pipeline state
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		// -- Input assembler related ---
		psoDesc.InputLayout.NumElements = inputElementCount;
		psoDesc.InputLayout.pInputElementDescs = inputElements;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		// Overall primitive topology type (triangle, line, etc.) is set here
		// IASetPrimTop() is still used to set list/strip/adj options
		// Root sig
		psoDesc.pRootSignature = rootSignature.Get();
		// -- Shaders (VS/PS) ---
		psoDesc.VS.pShaderBytecode = vertexShaderByteCode->GetBufferPointer();
		psoDesc.VS.BytecodeLength = vertexShaderByteCode->GetBufferSize();
		psoDesc.PS.pShaderBytecode = pixelShaderByteCode->GetBufferPointer();
		psoDesc.PS.BytecodeLength = pixelShaderByteCode->GetBufferSize();
		// -- Render targets ---
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		psoDesc.SampleDesc.Count = 1;
		psoDesc.SampleDesc.Quality = 0;
		// -- States ---
		psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		psoDesc.RasterizerState.DepthClipEnable = true;
		psoDesc.DepthStencilState.DepthEnable = true;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask =
			D3D12_COLOR_WRITE_ENABLE_ALL;
		// -- Misc ---
		psoDesc.SampleMask = 0xffffffff;
		// Create the pipe state object
		Graphics::Device->CreateGraphicsPipelineState(
			&psoDesc,
			IID_PPV_ARGS(pipelineState.GetAddressOf()));
	}
	// Set up the viewport and scissor rectangle
	{
		// Set up the viewport so we render into the correct
		// portion of the render target
		viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)Window::Width();
		viewport.Height = (float)Window::Height();
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		// Define a scissor rectangle that defines a portion of
		// the render target for clipping. This is different from
		// a viewport in that it is applied after the pixel shader.
		// We need at least one of these, but we're rendering to
		// the entire window, so it'll be the same size.
		scissorRect = {};
		scissorRect.left = 0;
		scissorRect.top = 0;
		scissorRect.right = Window::Width();
		scissorRect.bottom = Window::Height();
	}
}


// --------------------------------------------------------
// Handle resizing to match the new window size
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{
	// Resize the viewport and scissor rectangle
	{
		// Set up the viewport so we render into the correct
		// portion of the render target
		viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)Window::Width();
		viewport.Height = (float)Window::Height();
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		// Define a scissor rectangle that defines a portion of
		// the render target for clipping. This is different from
		// a viewport in that it is applied after the pixel shader.
		// We need at least one of these, but we're rendering to
		// the entire window, so it'll be the same size.
		scissorRect = {};
		scissorRect.left = 0;
		scissorRect.top = 0;
		scissorRect.right = Window::Width();
		scissorRect.bottom = Window::Height();
	}

	// Update any objects that would require it
	{
		// Calculate aspect ratio
		float aspectRatio = viewport.Width / viewport.Height;

		// Cameras
		mainCamera->UpdateProjectionMatrix(aspectRatio);
	}
}


// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Update main camera
	mainCamera->Update(deltaTime);

	// Example input checking: Quit if the escape key is pressed
	if (Input::KeyDown(VK_ESCAPE))
		Window::Quit();
}


// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Grab the current back buffer for this frame
	Microsoft::WRL::ComPtr<ID3D12Resource> currentBackBuffer =
		Graphics::BackBuffers[Graphics::SwapChainIndex()];
	// Clearing the render target
	{
		// Transition the back buffer from present to render target
		D3D12_RESOURCE_BARRIER rb = {};
		rb.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		rb.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		rb.Transition.pResource = currentBackBuffer.Get();
		rb.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		rb.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		rb.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		Graphics::CommandList->ResourceBarrier(1, &rb);
		// Background color (Cornflower Blue in this case) for clearing
		float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		// Clear the RTV
		Graphics::CommandList->ClearRenderTargetView(
			Graphics::RTVHandles[Graphics::SwapChainIndex()],
			color,
			0, 0); // No scissor rectangles
		// Clear the depth buffer, too
		Graphics::CommandList->ClearDepthStencilView(
			Graphics::DSVHandle,
			D3D12_CLEAR_FLAG_DEPTH,
			1.0f, // Max depth = 1.0f
			0, // Not clearing stencil, but need a value
			0, 0); // No scissor rects
	}

	// Rendering here!
	{
		// Set overall pipeline state
		Graphics::CommandList->SetPipelineState(pipelineState.Get());
		// Set up descriptor heap
		Graphics::CommandList->SetDescriptorHeaps(1, Graphics::CBVSRVDescriptorHeap.GetAddressOf());
		// Root sig (must happen before root descriptor table)
		Graphics::CommandList->SetGraphicsRootSignature(rootSignature.Get());
		// Set up other commands for rendering
		Graphics::CommandList->OMSetRenderTargets(
			1, &Graphics::RTVHandles[Graphics::SwapChainIndex()], true, &Graphics::DSVHandle);
		Graphics::CommandList->RSSetViewports(1, &viewport);
		Graphics::CommandList->RSSetScissorRects(1, &scissorRect);
		Graphics::CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Entity rendering loop
		for (auto& e : entities)
		{
			// Fill out struct to send to the vertex shader
			VertexShaderExternalData cbData = {};
			cbData.m4World = e->GetTransform()->GetWorldMatrix();
			cbData.m4View = mainCamera->GetViewMatrix();
			cbData.m4Projection = mainCamera->GetProjectionMatrix();
			cbData.m4WorldInvTranspose = e->GetTransform()->GetWorldInverseTransposeMatrix();

			// Copy the struct data over to the gpu
			D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = Graphics::FillNextConstantBufferAndGetGPUDescriptorHandle(&cbData, sizeof(cbData));
			
			// Utilize the command list to set the root descriptor table with the handle
			Graphics::CommandList->SetGraphicsRootDescriptorTable(0, gpuHandle);

			// Grab material reference for filling out structs
			std::shared_ptr<Material> material = e->GetMaterial();

			// Fill out struct for pixel shader
			PixelShaderExternalData psData = {};
			psData.albedo = material->GetAlbedo();
			psData.normal = material->GetNormalMap();
			psData.metal = material->GetMetalness();
			psData.roughness = material->GetRoughness();
			psData.height = material->GetHeight();
			psData.uvScale = material->GetUVScale();
			psData.uvOffset = material->GetUVOffset();
			psData.cameraPosition = mainCamera->GetTransform()->GetPosition();
			psData.lightCount = numLights;
			memcpy(psData.lights, &lights[0], sizeof(Light) * numLights);

			// Copy to GPU
			gpuHandle = Graphics::FillNextConstantBufferAndGetGPUDescriptorHandle(&psData, sizeof(psData));

			// Set root descriptor
			Graphics::CommandList->SetGraphicsRootDescriptorTable(1, gpuHandle);

			// Grab both vertex and index buffer views from the mesh
			D3D12_VERTEX_BUFFER_VIEW vbView = e->GetMesh()->GetVertexBufferView();
			D3D12_INDEX_BUFFER_VIEW ibView = e->GetMesh()->GetIndexBufferView();

			// Set both buffers
			Graphics::CommandList->IASetVertexBuffers(0, 1, &vbView);
			Graphics::CommandList->IASetIndexBuffer(&ibView);

			// Change to the correct pipeline state
			Graphics::CommandList->SetPipelineState(material->GetPipelineState().Get());

			// Finally call draw indexed
			Graphics::CommandList->DrawIndexedInstanced(e->GetMesh()->GetIndexCount(), 1, 0, 0, 0);
		}
	}

	sky->Draw(mainCamera);

	// Present
	{
		// Transition back to present
		D3D12_RESOURCE_BARRIER rb = {};
		rb.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		rb.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		rb.Transition.pResource = currentBackBuffer.Get();
		rb.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		rb.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		rb.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		Graphics::CommandList->ResourceBarrier(1, &rb);
		// Must occur BEFORE present
		Graphics::CloseAndExecuteCommandList();
		// Present the current back buffer and move to the next one
		bool vsync = Graphics::VsyncState();
		Graphics::SwapChain->Present(
			vsync ? 1 : 0,
			vsync ? 0 : DXGI_PRESENT_ALLOW_TEARING);
		Graphics::AdvanceSwapChainIndex();
		// Wait for the GPU to be done and then reset the command list & allocator
		Graphics::WaitForGPU();
		Graphics::ResetAllocatorAndCommandList();
	}
}



