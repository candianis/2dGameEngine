#include "ECS.h"


int IComponent::nextId = 0;

int Entity::GetId() const
{
	return id;
}

void Entity::Kill() {
	registry->KillEntity(*this);
}

void System::AddEntityToSystem(Entity entity)
{
	entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity)
{
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) {
		return entity == other;
		}), entities.end());
}

std::vector<Entity> System::GetSystemEntities() const
{
	return entities;
}

const Signature& System::GetComponentSignature() const
{
	return componentSignature;
}

Entity Registry::CreateEntity()
{
	int entityId;
	if (freeIds.empty()) {
		//If there are no free ids waiting to be reused then...
		entityId = numEntities++;
		if (entityId >= entityComponentSignatures.size()) {
			entityComponentSignatures.resize(entityId + 1);
		}
	}
	else {
		//Reuse an id from the list of previously removed entities
		entityId = freeIds.front();
		freeIds.pop_front();
	}

	Entity entity(entityId);
	entity.registry = this;
	entitiesToBeAdded.insert(entity);

	
	Logger::Log("Entity created with id: " + std::to_string(entityId));
	
	return entity;
}

void Registry::KillEntity(Entity entity) {
	entitiesToBeKilled.insert(entity);

	Logger::Log("Entity destroyed with id: " + std::to_string(entity.GetId()));
}

void Registry::AddEntityToSystem(Entity entity) {
	const int entityId = entity.GetId();

	const auto& entityComponentSignature = entityComponentSignatures[entityId];

	for (auto& system : systems) {
		const auto& systemComponentSignature = system.second->GetComponentSignature();

		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

		if (isInterested) {
			system.second->AddEntityToSystem(entity);
		}
	}
}

void Registry::RemoveEntityFromSystems(Entity entity) {
	for (auto system : systems) {
		system.second->RemoveEntityFromSystem(entity);
	}
}

void Registry::Update()
{
	//Entities that are waiting to be added to the system
	for (auto entity : entitiesToBeAdded) {
		AddEntityToSystem(entity);
	}
	entitiesToBeAdded.clear();

	//Entities that are waiting to be killed/removed from the system
	for (auto entity : entitiesToBeKilled) {
		RemoveEntityFromSystems(entity);

		entityComponentSignatures[entity.GetId()].reset();

		//Make the entity id available for other entities
		freeIds.push_back(entity.GetId());
	}
	entitiesToBeKilled.clear();
}