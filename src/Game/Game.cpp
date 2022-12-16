#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileLifecycleSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/RenderHealthBarSystem.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/KeyReleasedEvent.h"
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <imgui/imgui_impl_sdlrenderer.h>
#include <imgui/imgui_impl_sdl.h>
#include <iostream>
#include <fstream>
#include <iostream>

int Game::windowHeight;
int Game::windowWidth;
int Game::mapWidth;
int Game::mapHeight;

Game::Game()
{
	isRunning = false;
	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();
	eventBus = std::make_unique<EventBus>();
	Logger::Log("Game constructor called");
	Logger::Err("Error check");

}

Game::~Game()
{

	Logger::Log("Game destructor called");
}

void Game::Initialize()
{
	//SDL_DisplayMode displayMode;
	//SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = 1200;
	windowHeight = 800;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logger::Err("Error initializing SDL.");
		return;
	}

	if (TTF_Init() != 0) {
		Logger::Err("Error initializing TTF.");
	}

	 window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS
		);
	if (!window) {
		Logger::Log("Error creating SDL window.");
		return;
	}

	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		Logger::Log("Error creating SDL renderer.");
		return;
	}

	//Initilize the Imgui context
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);
	//ImGuiSDL::Initialize(renderer, windowWidth, windowHeight);

	//Initialize the camera view with the entire screen area
	camera.x = 0;
	camera.y = 0;
	camera.w = windowWidth;
	camera.h = windowHeight;

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	isRunning = true;
}

void Game::Run()
{
	Setup();
	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Game::LoadLevel(int levelId) {
	//Loadlevel will create entities and load levels

	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<CollisionSystem>();
	registry->AddSystem<RenderColliderSystem>();
	registry->AddSystem<DamageSystem>();
	registry->AddSystem<KeyboardControlSystem>();
	registry->AddSystem<CameraMovementSystem>();
	registry->AddSystem<ProjectileEmitSystem>();
	registry->AddSystem<ProjectileLifecycleSystem>();
	registry->AddSystem<RenderTextSystem>();
	registry->AddSystem<RenderHealthBarSystem>();

	assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
	assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
	assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
	assetStore->AddTexture(renderer, "jungle-map", "./assets/tilemaps/jungle.png");
	assetStore->AddTexture(renderer, "bullet-image", "./assets/images/bullet.png");

	assetStore->AddFont("charriot-font", "./assets/fonts/charriot.ttf", 18);
	assetStore->AddFont("pixelFull-font", "./assets/fonts/PixellettersFull.ttf", 16);

	//Load the tilemap
	const int tileSize = 32;
	const double tileScale = 2.0;
	int mapNumCols = 25;
	int mapNumRows = 20;

	std::ifstream mapFile;
	mapFile.open("./assets/tilemaps/jungle.map");

	if (mapFile.is_open())
	{
		if (!mapFile.good())
			return;

		for (int y = 0; y < mapNumRows; y++)
			for (int x = 0; x < mapNumCols; x++) {
				char ch;
				mapFile.get(ch);
				int srcRectY = std::atoi(&ch) * tileSize;
				mapFile.get(ch);
				int srcRectX = std::atoi(&ch) * tileSize;
				mapFile.ignore();

				Entity tile = registry->CreateEntity();
				tile.Group("tiles");
				tile.AddComponent<TransformComponent>(glm::vec2(x * (tileSize * tileScale), y * (tileSize * tileScale)), glm::vec2(tileScale, tileScale), 0.0);
				tile.AddComponent<SpriteComponent>("jungle-map", tileSize, tileSize, render_layers::LAYER_TILEMAP, false, srcRectX, srcRectY);
			}
	}
	mapFile.close();

	mapWidth = mapNumCols * tileSize * tileScale;
	mapHeight = mapNumRows * tileSize * tileScale;

	//Colors
	SDL_Color green = { 0, 255, 0 };

	//Deal with Entities
	Entity chopper = registry->CreateEntity();
	chopper.Tag("player");
	chopper.AddComponent<TransformComponent>(glm::vec2(100.0, 80.0), glm::vec2(1.0, 1.0), 0.0);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, LAYER_PLAYER);
	chopper.AddComponent<AnimationComponent>(2, 15);
	chopper.AddComponent<BoxColliderComponent>(32, 32);
	chopper.AddComponent<KeyboardControlledComponent>(glm::vec2(0, -80.0), glm::vec2(80, 0), glm::vec2(0, 80), glm::vec2(-80, 0));
	chopper.AddComponent<CameraFollowComponent>();
	chopper.AddComponent<HealthComponent>(100.0);
	chopper.AddComponent<ProjectileEmitterComponent>(glm::vec2(150.0, 150.0), 1500, 3000, 10, true);

	Entity radar = registry->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(1125.0, 16.0), glm::vec2(1.0, 1.0), 0.0);
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, LAYER_GUI, true);
	radar.AddComponent<AnimationComponent>(8, 7);

	Entity tank = registry->CreateEntity();
	tank.Group("enemies");
	tank.AddComponent<TransformComponent>(glm::vec2(500.0, 700.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	tank.AddComponent<SpriteComponent>("tank-image", 32, 32, LAYER_ENEMIES);
	tank.AddComponent<BoxColliderComponent>(32, 32);
	tank.AddComponent<ProjectileEmitterComponent>(glm::vec2(100.0, 0), 5000, 3000, 10, false);
	tank.AddComponent<HealthComponent>(100.0);

	Entity truck = registry->CreateEntity();
	truck.Group("enemies");
	truck.AddComponent<TransformComponent>(glm::vec2(410.0, 730.0), glm::vec2(1.0, 1.0), 0.0);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	truck.AddComponent<SpriteComponent>("truck-image", 32, 32, LAYER_ENEMIES);
	truck.AddComponent<BoxColliderComponent>(32, 32);
	truck.AddComponent<ProjectileEmitterComponent>(glm::vec2(0.0, 100.0), 2000, 5000, 10, false);
	truck.AddComponent<HealthComponent>(100.0);

	Entity label = registry->CreateEntity();
	label.AddComponent<TextLabelComponent>(glm::vec2(windowWidth / 2 - 40, 10), "Chopper 1.0", "charriot-font", green, true);
}

void Game::Setup() {
	LoadLevel(1);
}

void Game::ProcessInput()
{
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		//ImGui SDL input
		ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
		ImGuiIO& io = ImGui::GetIO();

		int mouseX, mouseY;
		const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

		io.MousePos = ImVec2(mouseX, mouseY);
		io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);


		switch (sdlEvent.type) {
		case SDL_QUIT:
			isRunning = false;
			break;

		case SDL_KEYDOWN:
			eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
			}
			if (sdlEvent.key.keysym.sym == SDLK_b) {
				isDebug = true;
			}
			break;
		
		case SDL_KEYUP:
			eventBus->EmitEvent<KeyReleasedEvent>(sdlEvent.key.keysym.sym);
		}
	}
}

void Game::Update()
{
	//if we are too fast then we will waste some time-> this function waits until MILLISECS PER FRAME is reached to do the rest of the body 
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
		SDL_Delay(timeToWait);
	}

	//The difference in ticks since last frame converted to seconds
	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

	//Store the previous frame time
	millisecsPreviousFrame = SDL_GetTicks();
	
	//Reset all event handlers for the current frame
	eventBus->Reset();

	//Perform the subscriptions of the events for all systems
	//We have to subscribe to events before updating the systems
	registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
	registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);
	registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventBus);

	//Update registry to create/destroy entities
	registry->Update();
	
	//Update all Systems that do not need to be rendered
	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<AnimationSystem>().Update();
	registry->GetSystem<CollisionSystem>().Update(eventBus);
	registry->GetSystem<DamageSystem>().Update();
	registry->GetSystem<CameraMovementSystem>().Update(camera);
	registry->GetSystem<ProjectileEmitSystem>().Update(registry);
	registry->GetSystem<ProjectileLifecycleSystem>().Update();

}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	//Uppdate all system that need to be rendered
	registry->GetSystem<RenderSystem>().Update(renderer, assetStore, camera);
	registry->GetSystem<RenderTextSystem>().Update(renderer ,assetStore, camera);
	registry->GetSystem<RenderHealthBarSystem>().Update(renderer, assetStore, camera);
	if (isDebug) {
		registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);

		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		//ImGuiSDL::Render(ImGui::GetDrawData());
	}

	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	//ImGuiSDL::Deinitialize();
	ImGui::DestroyContext();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

