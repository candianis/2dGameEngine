#ifndef CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraFollowComponent.h"

class CameraMovementSystem : public System {
public:
	CameraMovementSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<CameraFollowComponent>();
	}

	void Update(SDL_Rect& camera) {
		for (auto entity : GetSystemEntities()) {
			auto transform = entity.GetComponent<TransformComponent>();

			if (transform.position.x  + (camera.w / 2) < Game::mapWidth) {
				camera.x = transform.position.x - (Game::windowWidth / 2);
			}
			if (transform.position.y + (camera.h) < Game::mapHeight) {
				camera.y = transform.position.y - (Game::windowHeight);
			}
			
			//Terniary operator for keeping the camera rectangle view inside the screen limits
			camera.x = camera.x < 0 ? 0 : camera.x;
			camera.y = camera.y < 0 ? 0 : camera.y;
			camera.x = camera.x > camera.w ? camera.w : camera.x;
			camera.y = camera.y > camera.h ? camera.y : camera.y;
		}
	}
};

#endif // !CAMERAMOVEMENTSYSTEM_H