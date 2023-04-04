#ifndef RENDER_GUI_SYSTEM_H
#define RENDER_GUI_SYSTEM_H

#include "../ECS/ECS.h"
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <imgui/imgui_impl_sdlrenderer.h>
#include <imgui/imgui_impl_sdl.h>


class RenderGuiSystem : public System {
public:

	RenderGuiSystem() = default;

	void Update(const std::unique_ptr<Registry>& registry, const SDL_Rect& camera) {
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		
		//ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize;
		if (ImGui::Begin("Spawn Enemies")) {
			//This need to be static so that their information persists throughout frames
			static int position[2] = { 0,0 };
			static int scale[2] = { 1,1 };
			static float rotation{ 0 };
			static float velocity[2] = { 0,0 };
			static float projectileRotation{ 0 };
			static float projectileVelocity{ 0 };
			static int frequency{ 0 };
			static int duration{ 0 };
			static int health{ 100 };
			const char* spriteOptions[] = { "tank-image", "truck-image", "chopper-image" };
			static int current_sprite_index{ 0 };
			const char* preview_value = spriteOptions[current_sprite_index];
			static ImGuiComboFlags flags = 0;

			if(ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::InputInt2("Position (x,y)", position);
				ImGui::SliderInt2("Scale   (x,y)", scale, 1, 10);
				ImGui::SliderAngle("Rotation", &rotation, 0, 360);
			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Rigidbody", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::InputFloat2("Velocity ", velocity);
			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
				if (ImGui::BeginCombo("Texture Id", preview_value, flags)) {
					for (int n = 0; n < IM_ARRAYSIZE(spriteOptions); n++) {
						const bool is_selected = (current_sprite_index == n);
						if (ImGui::Selectable(spriteOptions[n], is_selected))
							current_sprite_index = n;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Projectile Emitter", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::SliderAngle("Angle", &projectileRotation, 0, 360);
				ImGui::SliderFloat("Velocity", &projectileVelocity, 0, 500);
				ImGui::InputInt("Frequency (sec)", &frequency);
				ImGui::InputInt("Duration (sec)", &duration);
			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Health")) {
				ImGui::SliderInt("%", &health, 0, 100);
			}

			if (ImGui::Button("Create new Enemy")) {
				Entity enemy = registry->CreateEntity();
				enemy.Group("enemies");
				enemy.AddComponent<TransformComponent>(glm::vec2(position[0], position[1]), glm::vec2(scale[0], scale[1]), glm::degrees(rotation));
				enemy.AddComponent<RigidBodyComponent>(glm::vec2(velocity[0], velocity[1]));
				enemy.AddComponent<SpriteComponent>(preview_value, 32, 32, LAYER_ENEMIES);
				enemy.AddComponent<BoxColliderComponent>(32, 32);
				double projVelX = std::cos(projectileRotation) * projectileVelocity;
				double projVelY = std::sin(projectileRotation) * projectileVelocity;
				enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projVelX, projVelY), frequency * 1000, duration * 1000, 10, false);
				enemy.AddComponent<HealthComponent>(health);

				for (int i = 0; i < 2; i++) {
					position[i] = 0;
					velocity[i] = 0;
					scale[i] = 1;
				}
				preview_value = spriteOptions[0];
				projectileVelocity = projectileRotation = frequency = duration = rotation = 0;
				health = 100;
			}
		}
		ImGui::End();

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
		ImGui::SetNextWindowBgAlpha(0.9f);
		if (ImGui::Begin("Map coordinates", NULL, windowFlags)) {
			ImGui::Text("Map coordinates (x=%.1f, y=%.1f)",	ImGui::GetIO().MousePos.x + camera.x, ImGui::GetIO().MousePos.y + camera.y);
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	}
};

#endif 