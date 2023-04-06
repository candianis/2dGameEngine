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

    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera) {
        // Create a vector with both Sprite and Transform component of all entities
        struct RenderableEntity {
            TransformComponent transformComponent;
            SpriteComponent spriteComponent;
        };

        //We want to render only the entities that are inside the camera view
        std::vector<RenderableEntity> renderableEntities;
        for (auto entity : GetSystemEntities()) {
            RenderableEntity renderableEntity;
            renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
            renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();


            int margin = 100;

            bool isEntityOutsideView = (
                renderableEntity.transformComponent.position.x + (renderableEntity.transformComponent.scale.x * renderableEntity.spriteComponent.width) < camera.x - margin ||
                renderableEntity.transformComponent.position.x > camera.x + camera.w + margin ||
                renderableEntity.transformComponent.position.y + (renderableEntity.transformComponent.scale.x * renderableEntity.spriteComponent.width) < camera.y - margin ||
                renderableEntity.transformComponent.position.y > camera.y + camera.h + margin
            );

            //Cull the sprites that are outside the camera view that are not fixed or UI
            if (isEntityOutsideView && !renderableEntity.spriteComponent.isFixed)
                continue;

            renderableEntities.emplace_back(renderableEntity);
        }

        std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b) {
            return a.spriteComponent.layer < b.spriteComponent.layer;
        });

        for (auto entity : renderableEntities) {
            const auto& transform = entity.transformComponent;
            const auto& sprite = entity.spriteComponent;

            SDL_Rect srcRect = sprite.srcRect;

            SDL_Rect dstRect = {
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
                sprite.flip
            );
        }
    }


};

#endif // !RENDERSYSTEM_H
