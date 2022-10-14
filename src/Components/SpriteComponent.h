#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL.h>
#include <string>

struct SpriteComponent {
	std::string assetId;
	int width;
	int height;
	SDL_Rect srcRect;

	SpriteComponent(std::string assetId = "", int width = 0, int height = 0, int srcRectx = 0, int srcRecty = 0) {
		this->assetId = assetId;
		this->width = width;
		this->height = height;
		this->srcRect = {srcRectx, srcRecty, width, height};
	}

};


#endif // !SPRITECOMPONENT_H
