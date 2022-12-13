#ifndef RENDERTEXTFOLLOWERSYSTEM_H
#define RENDERTEXTFOLLOWERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Asset Store/AssetStore.h"
#include "../Components/TextFollowerComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/HealthComponent.h"
#include <SDL.h>

class RenderTextFollowerSystem : public System {
public:
	RenderTextFollowerSystem() {
		RequireComponent<TextFollowerComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect camera) {
		for (auto entity : GetSystemEntities()) {
			const auto& textFollower = entity.GetComponent<TextFollowerComponent>();
			const auto& entityTransform = entity.GetComponent<TransformComponent>();
			const auto& health = entity.GetComponent<HealthComponent>();

			SDL_Color color;
			if (health.healthPercentage > 70)
				color = textFollower.color;
			if (health.healthPercentage <= 70 && health.healthPercentage > 30)
				color = { 255, 255, 0 };
			if (health.healthPercentage <= 30)
				color = { 255, 0, 0 };

			SDL_Surface* surface = TTF_RenderText_Blended(
				assetStore->GetFont(textFollower.assetId),
				textFollower.text.c_str(),
				color
			);

			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);

			int labelWidth = 0;
			int labelHeight = 0;

			SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

			SDL_Rect dstRect = {
				static_cast<int>(entityTransform.position.x + textFollower.offset.x) - camera.x,
				static_cast<int>(entityTransform.position.y + textFollower.offset.y) - camera.y,
				labelWidth,
				labelHeight
			};

			SDL_RenderCopy(renderer, texture, NULL, &dstRect);
		}
	}

};

#endif