#ifndef TEXTFOLLOWERCOMPONENT_H
#define TEXTFOLLOWERCOMPONENT_H

#include <SDL_ttf.h>
#include <string>
#include <glm/glm.hpp>

struct TextFollowerComponent {
	glm::vec2 offset;
	std::string text;
	std::string assetId;
	SDL_Color color;

	TextFollowerComponent(glm::vec2 offset = glm::vec2(0), std::string text = "", std::string assetId = "",const SDL_Color& color = {0,0,0}) {
		this->offset = offset;
		this->text = text;
		this->assetId = assetId;
		this->color = color;
	}
};

#endif