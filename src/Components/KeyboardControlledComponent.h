#ifndef KEYBOARDCONTROLLEDCOMPONENT_H
#define KEYBOARDCONTROLLEDCOMPONENT_H

#include <glm/glm.hpp>

enum direction_axis {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

struct KeyboardControlledComponent {
	glm::vec2 upVelocity;
	glm::vec2 rightVelocity;
	glm::vec2 downVelocity;
	glm::vec2 leftVelocity;
	glm::vec2 nullVelocity;
	direction_axis direction;

	KeyboardControlledComponent(glm::vec2 upVelocity = glm::vec2(0), glm::vec2 rightVelocity = glm::vec2(0), glm::vec2 downVelocity = glm::vec2(0), glm::vec2 leftVelocity = glm::vec2(0)) {
		this->upVelocity = upVelocity;
		this->rightVelocity = rightVelocity;
		this->downVelocity = downVelocity;
		this->leftVelocity = leftVelocity;
		this->nullVelocity = glm::vec2(0);
		this->direction = RIGHT;
	}
};

#endif // !KEYBOARDCONTROLLEDCOMPONENT_H
