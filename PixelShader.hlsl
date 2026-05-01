#include "ShaderIncludes.hlsli"

// Create samplers
SamplerState BasicSampler : register(s0);

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 screenPosition	: SV_POSITION;
	float2 uv               : TEXCOORD;
	float3 normal           : NORMAL;
	float3 tangent          : TANGENT;
	float3 worldPosition    : POSITION;
};

cbuffer ExternalData : register(b0)
{
	unsigned int albedo;
	unsigned int normal;
	unsigned int metal;
	unsigned int roughness;
    unsigned int height;
    float3 pad;
	float2 uvScale;
	float2 uvOffset;
	float3 cameraPosition;
	unsigned int lightCount;
	Light lights[3];
};

// Parallax mapping helpers
// From ray marching slides on my courses
float2 GetParallaxUV(float2 uv, float3 normal, float3 tangent, float3 view, int samples, Texture2D HeightMap)
{
    // Calculate a TBN matrix with B negated
    float3 N = normal;
    float3 T = normalize(tangent - N * dot(tangent, N));
    float3 B = cross(T, N);
    float3x3 TBN = float3x3(T, -B, N);
    
    // Get the view vector in tangent space from world space
    float3 view_TS = mul(TBN, view);

    // Calculate the ray direction
    float viewLength = length(view_TS);
    float parallaxLength = sqrt(viewLength * viewLength - view_TS.z * view_TS.z) / view_TS.z;
    float2 rayDir = normalize(view_TS.xy) * parallaxLength * uvScale;
    
    // Tracking height and position using ray marching
    float currentHeight = 1.0f;
    float2 currentPos = uv;
    float stepSize = 1.0f / samples;
    float2 uvStep = rayDir * stepSize;
    
    // Calculate derivatives for sampling
    float2 dx = ddx(uv);
    float2 dy = ddy(uv);
    
    // Raymarch through the object
    for (int i = 0; i < samples; i++)
    {
        // Offset and grabbing height
        currentPos -= uvStep;
        currentHeight -= stepSize;
        float heightAtPos = HeightMap.SampleGrad(BasicSampler, currentPos, dx, dy).r;
        
        // If value is below the heightmap, we have hit
        if (currentHeight < heightAtPos)
        {
            break;
        }
    }
    
    // Return the position
    return currentPos;
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// Get textures from the resource descriptor heap
	Texture2D AlbedoTexture = ResourceDescriptorHeap[albedo];
	Texture2D NormalTexture = ResourceDescriptorHeap[normal];
	Texture2D MetalTexture = ResourceDescriptorHeap[metal];
	Texture2D RoughnessTexture = ResourceDescriptorHeap[roughness];
    Texture2D HeightMap = ResourceDescriptorHeap[height];

	// Normalize tangents and normals
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);
	
	// Calculate view
    float3 view = cameraPosition - input.worldPosition;

	// Sample normals
	input.normal = NormalMapping(NormalTexture, BasicSampler, input.uv, input.normal, input.tangent);

    input.uv = GetParallaxUV(input.uv, input.normal, input.tangent, view, 8, HeightMap);
	
	// Texture loading before inclusion of lights
	float4 surfaceColor = AlbedoTexture.Sample(BasicSampler, input.uv);
	surfaceColor.rgb = pow(surfaceColor.rgb, 2.2f);

	// Roughness and metal sampling
	float roughness = RoughnessTexture.Sample(BasicSampler, input.uv).r;
	float metalness = MetalTexture.Sample(BasicSampler, input.uv).r;

	// Specular
	float3 specularColor = lerp(F0_NON_METAL, surfaceColor.rgb, metalness);

	// Total light
	float3 totalLight = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < lightCount; i++)
	{
		Light light = lights[i];
		light.direction = normalize(light.direction);

		switch (light.type)
		{
			case LIGHT_TYPE_DIRECTIONAL:
				totalLight += DirectionalLight(light, input.normal, float3(surfaceColor.rgb), cameraPosition, input.worldPosition, roughness, metalness);
				break;
			
			case LIGHT_TYPE_POINT:
				totalLight += PointLight(light, input.normal, float3(surfaceColor.rgb), cameraPosition, input.worldPosition, roughness, metalness);
				break;

			case LIGHT_TYPE_SPOT:
				totalLight += SpotLight(light, input.normal, float3(surfaceColor.rgb), cameraPosition, input.worldPosition, roughness, metalness);
				break;
		}
	}
	
    return float4(pow(totalLight, 1.0f / 2.2f), 1.0f);
}
