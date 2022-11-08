#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Asset Store/AssetStore.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL.h>
#include <algorithm>

class RenderSystem : public System {
public:
	RenderSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect camera) {
		std::vector<Entity> entitiesList = GetSystemEntities();

		std::sort(entitiesList.begin(), entitiesList.end(), [](const auto& entity1, const auto& entity2) {
			return entity1.GetComponent<SpriteComponent>().GetLayer() < entity2.GetComponent<SpriteComponent>().GetLayer();
			});

		for (auto entity : entitiesList) {
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto sprite = entity.GetComponent<SpriteComponent>();

			//Set source rectangle
			SDL_Rect srcRect = sprite.srcRect;
			SDL_Rect dstRect{
				static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
				static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)

			};

			SDL_RenderCopyEx(
				renderer, 
				assetStore->GetTexture(sprite.assetId), 
				&srcRect, 
				&dstRect, 
				transform.rotation, 
				NULL, 
				SDL_FLIP_NONE);
		}

	}


};

#endif // !RENDERSYSTEM_H
