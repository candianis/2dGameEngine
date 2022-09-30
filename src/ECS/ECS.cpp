#include "ECS.h"

int Entity::GetId() const
{
	return id;
}

void System::AddEntityToSystem(Entity entity)
{
	entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity)
{
	std::vector<Entity>::iterator it = entities.begin();

	while (it != entities.end()) {

		if (CheckIfEqual(*it, entity)) {
			it = entities.erase(it);
		}

		else
			++it;
	}
}

bool CheckIfEqual(Entity entityIt, Entity entityToDelete) {
	if (entityIt.GetId() == entityToDelete.GetId())
		return true;
	else
		return false;
}

std::vector<Entity> System::GetSystemEntities() const
{
	return entities;
}

const Signature& System::GetComponentSignature() const
{
	return componentSignature;
}
