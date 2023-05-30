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
#include <string>
#include <string_view>
#include <sol/sol.hpp>

//Const strings 
const char* COMPONENTS = "components";

const char*TRANSFORM = "transform";
const char* POSITION = "position";
const char* SCALE = "scale";
const char* ROTATION = "rotation";

const char* RIGIDBODY = "rigidbody";
const char* VELOCITY = "velocity";

const char* SPRITE = "sprite";
const char* TEXTURE_ID = "texture_asset_id";
const char* WIDTH = "width";
const char* HEIGHT = "height";
const char* LAYER = "layer";

const char* ANIMATION = "animation";
const char* NUM_FRAMES = "num_frames";
const char* SPEED_RATE = "speed_rate";

const char* BOX_COLLIDER = "box_collider";
const char* OFFSET = "offset";

const char* KEYBOARD_CONTROL = "keyboard_control";
const char* UP_VELOCITY = "up_velocity";
const char* RIGHT_VELOCITY = "right_velocity";
const char* DOWN_VELOCITY = "down_velocity";
const char* LEFT_VELOCITY = "left_velocity";

const char* PROJECTILE_EMITTER = "projectile_emitter";
const char* PROJECTILE_VELOCITY = "projectile_velocity";
const char* PROJECTILE_DURATION = "projectile_duration";
const char* REPEAT_FREQUENCY = "repeat_frequency";
const char* HIT_DAMAGE = "hit_percent_damage";
const char* FRIENDLY = "friendly";

const char* HEALTH = "health";
const char* HEALTH_PERCENTAGE = "health_percentage";

const char* CAMERA_FOLLOW = "camera_follow";

const char* TEXT_LABEL = "text_label";

LevelLoader::LevelLoader()
{
	Logger::Log("Level loader constructor called!");
}

LevelLoader::~LevelLoader()
{
	Logger::Log("Level loader destructor called!");
}

render_layers GetLayerFromString(const std::string layerType) {
	if (layerType == "PLAYER")
		return render_layers::LAYER_PLAYER;

	if (layerType == "OBSTACLES")
		return render_layers::LAYER_OBSTACLES;

	if (layerType == "GUI")
		return render_layers::LAYER_GUI;
}

void LevelLoader::LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, int levelNumber)
{
	//Checks if the script's syntax is correct but does not execute it
	sol::load_result script = lua.load_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");
	if (!script.valid()) {
		sol::error error = script;
		std::string errorMessage = error.what();
		Logger::Err("Error loading the lua script\n" + errorMessage);
		return;
	}

	//Executes the script using the Sol state
	lua.script_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");

	Logger::Log("Level" + std::to_string(levelNumber) + ".lua was just opened");

	sol::table level = lua["Level"];

	sol::table assets = level["assets"];

	int i = 0;
	while (true) {
		sol::optional<sol::table> hasAsset = assets[i];
		if (hasAsset == sol::nullopt) {
			Logger::Err("There was en error while trying to read an asset from the lua table");
			break;
		}
		sol::table asset = assets[i];
		std::string assetType = asset["type"];
		std::string assetId = asset["id"];
		if (assetType == "texture") {
			assetStore->AddTexture(renderer, assetId, asset["file"]);
			Logger::Log("New texture added to the asset store, id: " + assetId);
		}

		if (assetType == "font") {
			assetStore->AddFont(assetId, asset["file"], asset["font_size"]);
			Logger::Log("New font added to the asset store, id: " + assetId);
		}

		i++;
	}

	sol::table tilemap = level["tilemap"];
	std::string mapFilePath = tilemap["map_file"];
	std::string mapTextureAssetId = tilemap["texture_asset_id"];

	//Load the tilemap
	const int tileSize = tilemap["tile_size"];
	const double tileScale = tilemap["tile_scale"];
	int mapNumCols = tilemap["num_cols"];
	int mapNumRows = tilemap["num_rows"];

	std::ifstream mapFile;
	mapFile.open(mapFilePath);

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
				tile.AddComponent<SpriteComponent>(mapTextureAssetId, tileSize, tileSize, render_layers::LAYER_TILEMAP, false, srcRectX, srcRectY);
			}
	}
	mapFile.close();

	Game::mapWidth = mapNumCols * tileSize * tileScale;
	Game::mapHeight = mapNumRows * tileSize * tileScale;

	//----- Entities -----//
	sol::table entities = level["entities"];

	for (size_t i = 0; i <= entities.size(); i++) {
		sol::optional<sol::table> hasEntity = entities[i];
		if (hasEntity == sol::nullopt) {
			Logger::Err("An entity could not be loaded at index: " + std::to_string(i));
			break;
		}

		sol::table entity = entities[i];

		Entity newEntity = registry->CreateEntity();
		std::cout << "A new entity was created\n";
		
		//Tag
		sol::optional<std::string> tag = entity["tag"];
		if (tag != sol::nullopt) {
			newEntity.Tag(entity["tag"]);
		}

		//Group
		sol::optional<std::string> group = entity["group"];
		if (group != sol::nullopt) {
			newEntity.Group(entity["group"]);
		}

		//Components
		sol::optional<sol::table> hasComponents = entity[COMPONENTS];
		if (hasComponents != sol::nullopt) {
			sol::optional<sol::table> transform = entity[COMPONENTS][TRANSFORM];
			if (transform != sol::nullopt) {
				newEntity.AddComponent<TransformComponent>(
					glm::vec2(entity[COMPONENTS][TRANSFORM][POSITION]["x"], entity[COMPONENTS][TRANSFORM][POSITION]["y"]),
					glm::vec2(entity[COMPONENTS][TRANSFORM][SCALE]["x"], entity[COMPONENTS][TRANSFORM][SCALE]["y"]),
					entity[COMPONENTS][TRANSFORM][ROTATION]
					);
			}

			sol::optional<sol::table> rigidbody = entity[COMPONENTS][RIGIDBODY];
			if (rigidbody != sol::nullopt) {
				newEntity.AddComponent<RigidBodyComponent>(glm::vec2(entity[COMPONENTS][RIGIDBODY]["x"], (entity[COMPONENTS][RIGIDBODY]["y"])));
			}

			sol::optional<sol::table> sprite = entity[COMPONENTS][SPRITE];
			if (sprite != sol::nullopt) {
				bool isFixed = false;
				sol::optional<bool> checkIfFixed = entity[COMPONENTS][SPRITE]["isFixed"];
				if (checkIfFixed != sol::nullopt) {
					isFixed = entity[COMPONENTS][SPRITE]["isFixed"];
				}

				newEntity.AddComponent<SpriteComponent>(
					entity[COMPONENTS][SPRITE][TEXTURE_ID],
					entity[COMPONENTS][SPRITE][WIDTH],
					entity[COMPONENTS][SPRITE][HEIGHT],
					GetLayerFromString(entity[COMPONENTS][SPRITE]["layer"]),
					isFixed
					);
			}

			sol::optional<sol::table> animation = entity[COMPONENTS][ANIMATION];
			if (animation != sol::nullopt) {
				newEntity.AddComponent<AnimationComponent>(entity[COMPONENTS][ANIMATION]["num_frames"], entity[COMPONENTS][ANIMATION]["speed_rate"]);
			}

			sol::optional<sol::table> box_collider = entity[COMPONENTS][BOX_COLLIDER];
			if (box_collider != sol::nullopt) {
				newEntity.AddComponent<BoxColliderComponent>(entity[COMPONENTS][BOX_COLLIDER][WIDTH], entity[COMPONENTS][BOX_COLLIDER][HEIGHT], glm::vec2(entity[COMPONENTS][BOX_COLLIDER]["x"], entity[COMPONENTS][BOX_COLLIDER]["y"]));
			}

			sol::optional<sol::table> keyboardControl = entity[COMPONENTS][KEYBOARD_CONTROL];
			if (keyboardControl != sol::nullopt) {
				newEntity.AddComponent<KeyboardControlledComponent>(
					glm::vec2(entity[COMPONENTS][KEYBOARD_CONTROL][UP_VELOCITY]["x"],	 entity[COMPONENTS][KEYBOARD_CONTROL][UP_VELOCITY]["y"]),
					glm::vec2(entity[COMPONENTS][KEYBOARD_CONTROL][RIGHT_VELOCITY]["x"], entity[COMPONENTS][KEYBOARD_CONTROL][RIGHT_VELOCITY]["y"]),
					glm::vec2(entity[COMPONENTS][KEYBOARD_CONTROL][DOWN_VELOCITY]["x"],  entity[COMPONENTS][KEYBOARD_CONTROL][DOWN_VELOCITY]["y"]),
					glm::vec2(entity[COMPONENTS][KEYBOARD_CONTROL][LEFT_VELOCITY]["x"],  entity[COMPONENTS][KEYBOARD_CONTROL][LEFT_VELOCITY]["y"])
					);
			}

			sol::optional<sol::table> projectileEmitter = entity[COMPONENTS][PROJECTILE_EMITTER];
			if (projectileEmitter != sol::nullopt) {
				newEntity.AddComponent<ProjectileEmitterComponent>(
					glm::vec2(entity[COMPONENTS][PROJECTILE_EMITTER][PROJECTILE_VELOCITY]["x"], entity[COMPONENTS][PROJECTILE_EMITTER][PROJECTILE_VELOCITY]["Y"]),
					entity[COMPONENTS][PROJECTILE_EMITTER][PROJECTILE_DURATION],
					entity[COMPONENTS][PROJECTILE_EMITTER][REPEAT_FREQUENCY],
					entity[COMPONENTS][PROJECTILE_EMITTER][HIT_DAMAGE],
					entity[COMPONENTS][PROJECTILE_EMITTER][FRIENDLY]
					);
			}

			sol::optional<sol::table> health = entity[COMPONENTS][HEALTH];
			if (health != sol::nullopt) {
				int healthValue = entity[COMPONENTS][HEALTH][HEALTH_PERCENTAGE];
				newEntity.AddComponent<HealthComponent>(healthValue);
			}

			sol::optional<sol::table> cameraFollow = entity[COMPONENTS][CAMERA_FOLLOW];
			if (cameraFollow != sol::nullopt) {
				newEntity.AddComponent<CameraFollowComponent>();
			}

			sol::optional<sol::table> textLabel = entity[COMPONENTS][TEXT_LABEL];
			if (textLabel != sol::nullopt) {
				SDL_Color green = { 0, 255, 0 };

				SDL_Color desiredColor = { entity[COMPONENTS][TEXT_LABEL]["color"]["r"], entity[COMPONENTS][TEXT_LABEL]["color"]["g"], entity[COMPONENTS][TEXT_LABEL]["color"]["b"] };
				newEntity.AddComponent<TextLabelComponent>(
					glm::vec2(entity[COMPONENTS][TEXT_LABEL][POSITION]["x"], entity[COMPONENTS][TEXT_LABEL][POSITION]["y"]),
					entity[COMPONENTS][TEXT_LABEL]["text"],
					entity[COMPONENTS][TEXT_LABEL]["font"],
					desiredColor,
					entity[COMPONENTS][TEXT_LABEL]["isFixed"]
					);
			}
		}
	}
}
