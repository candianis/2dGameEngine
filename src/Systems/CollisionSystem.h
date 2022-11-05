#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include <SDL.h>

class CollisionSystem : public System{
public:
	CollisionSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
	}

	void Update(std::unique_ptr<EventBus>& eventBus) {
		auto entities = GetSystemEntities();
		bool collisionHappened;

		//Loop all the entities that the system is interested in
		for (auto i = entities.begin(); i != entities.end(); i++) {
			Entity a = *i;

			auto aTransform = a.GetComponent<TransformComponent>();
			auto aCollider = a.GetComponent<BoxColliderComponent>();

			//Loop all the entities that still need to be checked (that are next to i 
			for (auto j = i + 1; j != entities.end(); j++) {
				Entity b = *j;

				auto bTransform = b.GetComponent<TransformComponent>();
				auto bCollider = b.GetComponent<BoxColliderComponent>();
				//check if a and b are colliding 
				collisionHappened = CheckAABBCollision(
					aTransform.position.x + (aCollider.offset.x * aTransform.scale.x), 
					aTransform.position.y + (aCollider.offset.y * aTransform.scale.y),
					aCollider.width * aTransform.scale.x, 
					aCollider.height * aTransform.scale.y,
					bTransform.position.x + (bCollider.offset.x * bTransform.scale.x), 
					bTransform.position.y + (bCollider.offset.y * bTransform.scale.y),
					bCollider.width * bTransform.scale.x, 
					bCollider.height * aTransform.scale.y
				);

				if (collisionHappened) {
					Logger::Log("Entity with id " + std::to_string(a.GetId()) + " collided with Entity with id " + std::to_string(b.GetId()));
					eventBus->EmitEvent<CollisionEvent>(a, b);
				}
			}
		}
	}

	void ShowColliders() {
		showingColliders = !showingColliders;
	}

private:
	bool showingColliders = false;

	bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH) {
		return (
			aX < bX + bW &&
			aX + aW > bX &&
			aY < bY + bH &&
			aY + aH > bY 
			);
	}

};

#endif // !COLLISIONSYSTEM_H
