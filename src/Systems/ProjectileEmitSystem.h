#ifndef PROJECTILEEMITSYSTEM_H
#define PROJECTILEEMITSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/ProjectileComponent.h"
#include <SDL.h>

class ProjectileEmitSystem : public System {
public:
	ProjectileEmitSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<ProjectileEmitterComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::EmitProjectile);
	}

	void EmitProjectile(KeyPressedEvent& event) {
		if (event.symbol != SDLK_SPACE)
			return;

		for (auto entity : GetSystemEntities()) {
			if (!entity.HasComponent<KeyboardControlledComponent>())
				continue;
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& keyboardControl = entity.GetComponent<KeyboardControlledComponent>();
			auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();

			if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency) {
				glm::vec2 projectilePosition = transform.position;
				if (entity.HasComponent<SpriteComponent>()) {
					const auto& sprite = entity.GetComponent<SpriteComponent>();
					projectilePosition.x += (transform.scale.x * sprite.width / 2);
					projectilePosition.y += (transform.scale.y * sprite.height / 2);
				}

				glm::vec2 projectileVelocity;
				switch (keyboardControl.direction) {
				case direction_axis::UP:
					projectileVelocity = glm::vec2(0.0, -projectileEmitter.projectileVelocity.y);
					break;

				case direction_axis::RIGHT:
					projectileVelocity = glm::vec2(projectileEmitter.projectileVelocity.x, 0.0);
					break;

				case direction_axis::DOWN:
					projectileVelocity = glm::vec2(0.0, projectileEmitter.projectileVelocity.y);
					break;

				case direction_axis::LEFT:
					projectileVelocity = glm::vec2(-projectileEmitter.projectileVelocity.x, 0.0);
				}

				Entity projectile = entity.registry->CreateEntity();
				projectile.Group("projectiles");
				projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
				projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
				projectile.AddComponent<SpriteComponent>("bullet-texture", 4, 4, LAYER_BULLETS);
				projectile.AddComponent<BoxColliderComponent>(4, 4);
				projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);

				projectileEmitter.lastEmissionTime = SDL_GetTicks();
			}
		}
	}

	void Update(std::unique_ptr<Registry>& registry) {
		for (auto entity : GetSystemEntities()) {
			const auto& transform = entity.GetComponent<TransformComponent>();
			auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();

			if (entity.HasComponent<KeyboardControlledComponent>() || projectileEmitter.repeatFrequency == 0) continue;

			 if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency) {
				glm::vec2 projectilePosition = transform.position;
				if (entity.HasComponent<SpriteComponent>()) {
					const auto& sprite = entity.GetComponent<SpriteComponent>();
					projectilePosition.x += (transform.scale.x * sprite.width / 2);
					projectilePosition.y += (transform.scale.y * sprite.height / 2);
				}

				Entity projectile = registry->CreateEntity();
				projectile.Group("projectiles");
				projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
				projectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
				projectile.AddComponent<SpriteComponent>("bullet-texture", 4, 4, LAYER_BULLETS);
				projectile.AddComponent<BoxColliderComponent>(4, 4);
				projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);

				//Update the projectile emitter component last emission to the current milliseconds
				projectileEmitter.lastEmissionTime = SDL_GetTicks();
			}
		}
	}
};

#endif // !PROJECTILEEMITSYSTEM_H
