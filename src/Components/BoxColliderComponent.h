#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <glm/glm.hpp>

struct BoxColliderComponent
{
	int width;
	int height;
	glm::vec2 offset;
	
	BoxColliderComponent(int width = 1, int height = 1, glm::vec2 offset = glm::vec2(0)) {
		this->width = width;
		this->height = height;
		this->offset = offset;
	}
};

#endif // !BOXCOLLIDERCOMPONENT_H
