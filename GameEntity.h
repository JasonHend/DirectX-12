#pragma once

#include <memory>
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"

// Defines a game entity that holds a transform and a mesh to draw objects onto the screen / update their positions
class GameEntity
{
public:
	// Constructors
	GameEntity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

	// Getters
	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Material> GetMaterial();
	std::shared_ptr<Transform> GetTransform();

	// Setters
	void SetMesh(std::shared_ptr<Mesh> mesh);
	void SetMaterial(std::shared_ptr<Material> material);

private:
	// Mesh and transform objects
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Material> material;
};

