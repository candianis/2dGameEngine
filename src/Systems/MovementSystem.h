#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"

class MovementSystem : public System{
public:

	MovementSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
	}

	void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
	}

	void OnCollision(CollisionEvent& event) {
		Entity a = event.a;
		Entity b = event.b;

		if (a.BelongsToGroup("enemies") && b.BelongsToGroup("obstacles")) {
			OnEnemyHitsObstacle(a, b);
		}

		if (a.BelongsToGroup("obstacles") && b.BelongsToGroup("enemies")) {
			OnEnemyHitsObstacle(b, a);
		}
	}

	void OnEnemyHitsObstacle(Entity enemy, Entity obstacle) {
		if (!enemy.HasComponent<RigidBodyComponent>() || !enemy.HasComponent<SpriteComponent>())
			return;

		auto& rigidbody = enemy.GetComponent<RigidBodyComponent>();
		auto& sprite = enemy.GetComponent<SpriteComponent>();
		auto& projectileEmitter = enemy.GetComponent<ProjectileEmitterComponent>();

		if (rigidbody.velocity.x != 0) {
			rigidbody.velocity.x *= -1;
			projectileEmitter.projectileVelocity *= -1;
			sprite.flip = sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
		}
		if (rigidbody.velocity.y != 0) {
			rigidbody.velocity.y *= -1;
			projectileEmitter.projectileVelocity *= -1;
			sprite.flip = sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
		}
	}

	void Update(double deltaTime) {

		for (auto entity : GetSystemEntities()) {
			auto& transform = entity.GetComponent<TransformComponent>();
			const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

			transform.position.x += rigidbody.velocity.x * deltaTime;
			transform.position.y += rigidbody.velocity.y * deltaTime;

			//Prevent the player from getting outside the map
			if (entity.HasTag("player")) {
				const auto& sprite = entity.GetComponent<SpriteComponent>();
				//We do not want the player to be exactly at 0,0 so we will be adding a padding
				float paddingLeft = 15;
				float paddingRight = 50;
				float paddingTop = 15;
				float paddingBottom = 50;

				transform.position.x = (transform.position.x + sprite.width) > Game::mapWidth - paddingRight ? Game::mapWidth - paddingRight - sprite.width : transform.position.x;
				transform.position.y = (transform.position.y + sprite.height) > Game::mapHeight - paddingBottom ? Game::mapHeight - paddingBottom - sprite.width : transform.position.y;
				
				transform.position.x = transform.position.x < paddingLeft ? paddingLeft : transform.position.x;
				transform.position.y = transform.position.y < paddingTop  ? paddingTop : transform.position.y;
			}
			
			int margin = 100;
			bool isEntityOutsideMap = (
				transform.position.x < -margin || transform.position.x > Game::mapWidth + margin ||
				transform.position.y < -margin || transform.position.y > Game::mapHeight + margin
			);

			if (isEntityOutsideMap && !entity.HasTag("player"))
				entity.Kill();
		}
	}
};

#endif