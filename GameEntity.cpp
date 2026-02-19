#include "GameEntity.h"

/// <summary>
/// Takes in a mesh parameter and creates a game object
/// </summary>
/// <param name="mesh">Mesh object that will be used for the entity</param>
GameEntity::GameEntity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
    : mesh(mesh), material(material)
{
    transform = std::make_shared<Transform>();
}

std::shared_ptr<Mesh> GameEntity::GetMesh() { return mesh; }
std::shared_ptr<Material> GameEntity::GetMaterial() { return material; }
std::shared_ptr<Transform> GameEntity::GetTransform() { return transform; }

void GameEntity::SetMesh(std::shared_ptr<Mesh> mesh) { this->mesh = mesh; }
void GameEntity::SetMaterial(std::shared_ptr<Material> material) { this->material = material; }
