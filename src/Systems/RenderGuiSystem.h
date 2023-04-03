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

	void Update(const std::unique_ptr<Registry>& registry) {
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		
		//ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize;
		if (ImGui::Begin("Spawn Enemies")) {
			//This need to be static so that their information persists throughout frames
			static int position[2] = { 0,0 };
			static float scale[2] = { 1,1 };
			static int rotation{ 0 };
			static float velocity[2] = { 0,0 };
			static float projectileVelocity[2] = { 0,0 };
			static int frequency{ 0 };
			static int duration{ 0 };
			static int health{ 100 };
			const char* spriteOptions[] = { "tank-image", "truck-image", "chopper-image" };
			static int current_sprite_index{ 0 };
			const char* preview_value = spriteOptions[current_sprite_index];
			static ImGuiComboFlags flags = 0;

			ImGui::InputInt2("Position", position);
			ImGui::InputFloat2("Scale", scale);
			ImGui::SliderInt("Rotation", &rotation, 0, 360);
			ImGui::InputFloat2("Velocity ", velocity);


			if (ImGui::BeginCombo("Sprite Options", preview_value, flags)) {
				for (int n = 0; n < IM_ARRAYSIZE(spriteOptions); n++) {
					const bool is_selected = (current_sprite_index == n);
					if (ImGui::Selectable(spriteOptions[n], is_selected))
						current_sprite_index = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::InputFloat2("Projectile Velocity", projectileVelocity);
			ImGui::InputInt("Projectile Frequency", &frequency);
			ImGui::InputInt("Projectile Duration", &duration);
			ImGui::InputInt("Health", &health);

			if (ImGui::Button("Create new Enemy")) {
				Entity enemy = registry->CreateEntity();
				enemy.Group("enemies");
				enemy.AddComponent<TransformComponent>(glm::vec2(static_cast<double>(position[0]), static_cast<double>(position[1])), glm::vec2(scale[0], scale[1]), rotation);
				enemy.AddComponent<RigidBodyComponent>(glm::vec2(velocity[0], velocity[1]));
				enemy.AddComponent<SpriteComponent>(preview_value, 32, 32, LAYER_ENEMIES);
				enemy.AddComponent<BoxColliderComponent>(32, 32);
				enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projectileVelocity[0], projectileVelocity[1]), frequency * 1000, duration * 1000, 10, false);
				enemy.AddComponent<HealthComponent>(health);
			}
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	}
};

#endif 