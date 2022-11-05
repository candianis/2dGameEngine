#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Events/KeyPressedEvent.h"
#include "../EventBus/EventBus.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include <iostream>

class InputSystem : public System {
public:
	InputSystem() {
		
	}

	void Update(std::unique_ptr<EventBus>& eventBus, bool& isRunning) {
		SDL_Event keyPressed;
		while (SDL_PollEvent(&keyPressed)) {

			switch (keyPressed.type) {
			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_KEYDOWN:
				eventBus->EmitEvent<KeyPressedEvent>(keyPressed.key.keysym.sym);
				if (keyPressed.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
				}
				if (keyPressed.key.keysym.sym == SDLK_d) {
					
				}
				break;
			}
		}

	}
};

#endif // !INPUTSYSTEM_H
