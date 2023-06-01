#ifndef KEYBOARDSYSTEM_H
#define KEYBOARDSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/KeyReleasedEvent.h"
#include <SDL.h>

class KeyboardControlSystem : public System {
public:
	KeyboardControlSystem() {
		RequireComponent<KeyboardControlledComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<RigidBodyComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
		eventBus->SubscribeToEvent<KeyReleasedEvent>(this, &KeyboardControlSystem::OnKeyReleased);
	}

	void OnKeyPressed(KeyPressedEvent& event) {
		for (auto entity : GetSystemEntities()) {
			auto& keyboardControl = entity.GetComponent<KeyboardControlledComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

			switch (event.symbol) {
			case SDLK_UP:
			case SDLK_w:
				rigidbody.velocity = keyboardControl.upVelocity;
				sprite.srcRect.y = sprite.height * 0;
				keyboardControl.direction = direction_axis::UP;
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				rigidbody.velocity = keyboardControl.rightVelocity;
				sprite.srcRect.y = sprite.height * 1;
				keyboardControl.direction = direction_axis::RIGHT;
				break;
			case SDLK_DOWN:
			case SDLK_s:
				rigidbody.velocity = keyboardControl.downVelocity;
				sprite.srcRect.y = sprite.height * 2;
				keyboardControl.direction = direction_axis::DOWN;
				break;
			case SDLK_LEFT:
			case SDLK_a:
				rigidbody.velocity = keyboardControl.leftVelocity;
				sprite.srcRect.y = sprite.height * 3;
				keyboardControl.direction = direction_axis::LEFT;
				break;
			}
		}
	}

	void OnKeyReleased(KeyReleasedEvent& event) {
		for (auto entity : GetSystemEntities()) {
			const auto& keyboardControl = entity.GetComponent<KeyboardControlledComponent>();
			auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

			switch (event.symbol) {
			case SDLK_UP:
			case SDLK_w:
			case SDLK_RIGHT:
			case SDLK_d:
			case SDLK_DOWN:
			case SDLK_s:
			case SDLK_LEFT:
			case SDLK_a:
				rigidbody.velocity = keyboardControl.nullVelocity;
				break;
				
			case SDLK_SPACE:

				break;
			}		
		}
	}

	void Update(std::unique_ptr<EventBus>& eventBus) {

	}

};

#endif // !KEYBOARDSYSTEM_H
