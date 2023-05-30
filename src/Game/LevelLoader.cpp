#include "./LevelLoader.h"
#include "./Game.h"
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
#include <fstream>

LevelLoader::LevelLoader()
{

}

LevelLoader::~LevelLoader()
{

}

void LevelLoader::LoadLevel(const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, int level)
{
	assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
	assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
	assetStore->AddTexture(renderer, "tree-image", "./assets/images/tree.png");
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

	Game::mapWidth = mapNumCols * tileSize * tileScale;
	Game::mapHeight = mapNumRows * tileSize * tileScale;

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
	tank.AddComponent<TransformComponent>(glm::vec2(600.0, 500.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 0.0));
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

	//Obstacles
	Entity treeA = registry->CreateEntity();
	treeA.Group("obstacles");
	treeA.AddComponent<TransformComponent>(glm::vec2(700.0, 500.0), glm::vec2(1.0, 1.0), 0.0);
	treeA.AddComponent<SpriteComponent>("tree-image", 16, 32, LAYER_OBSTACLES);
	treeA.AddComponent<BoxColliderComponent>(16, 32);

	Entity treeB = registry->CreateEntity();
	treeB.Group("obstacles");
	treeB.AddComponent<TransformComponent>(glm::vec2(500.0, 500.0), glm::vec2(1.0, 1.0), 0.0);
	treeB.AddComponent<SpriteComponent>("tree-image", 16, 32, LAYER_OBSTACLES);
	treeB.AddComponent<BoxColliderComponent>(16, 32);

	Entity label = registry->CreateEntity();
	label.AddComponent<TextLabelComponent>(glm::vec2(Game::windowWidth / 2 - 40, 10), "Chopper 1.0", "charriot-font", green, true);
}
