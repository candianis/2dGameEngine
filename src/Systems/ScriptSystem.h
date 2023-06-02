#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ScriptComponent.h"

//Native C++ functions that will be binded to Lua
glm::vec2 GetEntityPosition(Entity entity) {
	if (entity.HasComponent<TransformComponent>()) {
		const auto& transform = entity.GetComponent<TransformComponent>();
		return transform.position;
	}

	else {
		Logger::Err("The entity with the id: " + std::to_string(entity.GetId()) + " does not have a transform component.");
		return glm::vec2(0);
	}
}

glm::vec2 GetEntityScale(Entity entity) {
	if (entity.HasComponent<TransformComponent>()) {
		const auto& transform = entity.GetComponent<TransformComponent>();
		return transform.scale;
	}

	else {
		Logger::Err("The entity with the id: " + std::to_string(entity.GetId()) + " does not have a transform component.");
		return glm::vec2(0);
	}
}

double GetEntityRotation(Entity entity) {
	if (entity.HasComponent<TransformComponent>()) {
		const auto& transform = entity.GetComponent<TransformComponent>();
		return transform.rotation;
	}

	else {
		Logger::Err("The entity with the id: " + std::to_string(entity.GetId()) + " does not have a transform component.");
		return 0.0;
	}
}

glm::vec2 GetEntityVelocity(Entity entity) {
	if (entity.HasComponent<RigidBodyComponent>()) {
		const auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
		return rigidbody.velocity;
	}
	else {
		Logger::Err("The entity with the id: " + std::to_string(entity.GetId()) + " does not have a rigidbody component. ");
		return glm::vec2(0);
	}
}

void SetEntityPosition(Entity entity, glm::vec2 position) {
	if (entity.HasComponent<TransformComponent>()) {
		auto& transform = entity.GetComponent<TransformComponent>();
		transform.position = position;
	}
	else {
		Logger::Err("The Entity with id: " + std::to_string(entity.GetId()) + " has no transform component and its position could not be changed.");
	}
}

void SetEntityScale(Entity entity, glm::vec2 scale) {
	if (entity.HasComponent<TransformComponent>()) {
		auto& transform = entity.GetComponent<TransformComponent>();
		transform.scale = scale;
	}
	else {
		Logger::Err("The Entity with id: " + std::to_string(entity.GetId()) + " has no transform component and its scale could not be changed.");
	}
}

void SetEntityRotation(Entity entity, double rotation) {
	if (entity.HasComponent<TransformComponent>()) {
		auto& transform = entity.GetComponent<TransformComponent>();
		transform.rotation = rotation;
	}
	else {
		Logger::Err("The Entity with id: " + std::to_string(entity.GetId()) + " has no transform component and its rotation could not be changed.");
	}
}

void SetEntityVelocity(Entity entity, glm::vec2 velocity) {
	if (entity.HasComponent<RigidBodyComponent>()) {
		auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
		rigidbody.velocity = velocity;
	}

	else {
		Logger::Err("The entity with the id: " + std::to_string(entity.GetId()) + " does not have a rigidbody component and its velocity could not be changed.");
	}
}

void SetEntityAnimationFrame(Entity entity, int animationFrame) {
	if (entity.HasComponent<AnimationComponent>()) {
		auto& animation = entity.GetComponent<AnimationComponent>();
		animation.currentFrame = animationFrame;
	}
	else {
		Logger::Err("The entity with the id: " + std::to_string(entity.GetId()) + "does not have an animation component");
	}
}

class ScriptSystem : public System {
public: 
	ScriptSystem() {
		RequireComponent<ScriptComponent>();
	}

	void CreateLuaBindings(sol::state& lua) {
		lua.new_usertype<Entity>(
			"entity", 
			"get_id", &Entity::GetId,
			"destroy", &Entity::Kill,
			"has_tag", &Entity::HasTag,
			"belongs_to_group", &Entity::BelongsToGroup
			);
		lua.new_usertype<glm::vec2>(
			"vec2", sol::constructors<glm::vec2(float, float)>(), 
			"x", &glm::vec2::x,
			"y", &glm::vec2::y);

		lua.set_function("get_position", GetEntityPosition);
		lua.set_function("get_scale",	 GetEntityScale);
		lua.set_function("get_rotation", GetEntityRotation);
		lua.set_function("get_velocity", GetEntityVelocity);
		lua.set_function("set_position", SetEntityPosition);
		lua.set_function("set_scale",	 SetEntityScale);
		lua.set_function("set_rotation", SetEntityRotation);
		lua.set_function("set_velocity", SetEntityVelocity);
		lua.set_function("set_frame",	 SetEntityAnimationFrame);
	}

	void Update(double deltaTime, int ellapsedTime) {
		for (auto entity : GetSystemEntities()) {
			const auto& script = entity.GetComponent<ScriptComponent>();
			script.func(entity, deltaTime, ellapsedTime);
		}
	}
};

#endif // !SCRIPT_SYSTEM_H
