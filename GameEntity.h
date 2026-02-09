#pragma once

#include "Mesh.h"
#include "Transform.h"
#include <memory>

// Defines a game entity that holds a transform and a mesh to draw objects onto the screen / update their positions
class GameEntity
{
public:
	// Constructors
	GameEntity(Mesh* mesh);

	// Getters
	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Transform> GetTransform();

	// Setters
	void SetMesh(std::shared_ptr<Mesh> mesh);

private:
	// Mesh and transform objects
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Transform> transform;
};

