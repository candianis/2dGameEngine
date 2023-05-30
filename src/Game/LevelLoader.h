#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include "../ECS/ECS.h"
#include "../Asset Store/AssetStore.h"
#include <SDL.h>
#include <sol/sol.hpp>
#include <memory>

class LevelLoader {
public:
	LevelLoader();
	~LevelLoader();

	void LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, int level);
};

#endif // !LEVEL_LOADER_H
