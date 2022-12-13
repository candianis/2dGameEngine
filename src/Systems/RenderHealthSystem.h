#ifndef RENDERHEALTHSYSTEM_H
#define RENDERHEALTHSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TextFollowerComponent.h"
#include <SDL.h>
#include <string>

class RenderHealthSystem : public System {
public:
	RenderHealthSystem() {
		RequireComponent<HealthComponent>();
	}

	void Update(SDL_Renderer* renderer, SDL_Rect camera) {
		for (auto entity : GetSystemEntities()) {
			const auto& health = entity.GetComponent<HealthComponent>();
			const auto& transform = entity.GetComponent<TransformComponent>();

			SDL_Rect lifeBarRect = {
				static_cast<int>(transform.position.x + 35 - camera.x),
				static_cast<int>(transform.position.y + 10 - camera.y),
				health.healthPercentage / 2,
				10
			};

			SDL_Color color = { 0, 255, 0, 1 }; //green
			if (health.healthPercentage <= 70 && health.healthPercentage >= 40) {
				color = { 255, 255, 0, 1 }; //yellow
			}
			if (health.healthPercentage <= 30) {
				color = { 255, 0, 0, 1 }; //red
			}

			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_RenderFillRect(renderer, &lifeBarRect);
			//SDL_RenderDrawRect(renderer, &lifeBarRect);
		}
	}
};

#endif