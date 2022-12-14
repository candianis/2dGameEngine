#ifndef RENDERHEALTHSYSTEM_H
#define RENDERHEALTHSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/SpriteComponent.h"

#include <SDL.h>
#include <string>

class RenderHealthBarSystem : public System {
public:
	RenderHealthBarSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<HealthComponent>();
	}

	void Update(SDL_Renderer* renderer,const std::unique_ptr<AssetStore>& assetStore, const SDL_Rect& camera) {
        for (auto entity : GetSystemEntities()) {
            const auto& health = entity.GetComponent<HealthComponent>();
            const auto& sprite = entity.GetComponent<SpriteComponent>();
            const auto& transform = entity.GetComponent<TransformComponent>();

            SDL_Color healthBarColor = { 0, 255, 0 };

            if (health.healthPercentage <= 70 && health.healthPercentage >= 40) {
                healthBarColor = { 255, 255, 0 };
            }
            if (health.healthPercentage < 40) {
                healthBarColor = { 255, 0, 0 };
            }

            double healthBarWidth = 0.5;
            int healthBarHeight = 5;
            double healthBarPosX = (transform.position.x + (sprite.width * transform.scale.x)) - camera.x;
            double healthBarPosY = (transform.position.y) - camera.y;

            SDL_Rect healthBarRect = {
                static_cast<int>(healthBarPosX),
                static_cast<int>(healthBarPosY),
                static_cast<int>(healthBarWidth * health.healthPercentage),
                healthBarHeight
            };
        
            SDL_SetRenderDrawColor(renderer, healthBarColor.r, healthBarColor.g, healthBarColor.b, 255);
            SDL_RenderFillRect(renderer, &healthBarRect);

            // Render the health percentage text label indicator
            std::string healthText = std::to_string(health.healthPercentage) + "%";

            SDL_Surface* surface = TTF_RenderText_Blended(assetStore->GetFont("pixelFull-font"), healthText.c_str(), healthBarColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            int labelWidth = 0;
            int labelHeight = 0;
            SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);
            SDL_Rect healthBarTextRect = {
                static_cast<int>(healthBarPosX),
                static_cast<int>(healthBarPosY) - 18,
                labelWidth,
                labelHeight
            };

            SDL_RenderCopy(renderer, texture, NULL, &healthBarTextRect);
            SDL_DestroyTexture(texture);
		}
	}
};

#endif