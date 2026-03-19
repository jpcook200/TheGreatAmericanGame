#pragma once
#include "EntityManager.h"
#include "AssetManager.h"

class AnimationSystem
{

public:
	AnimationSystem();
	void update(EntityManager& entityManager, const AssetManager& assetManager);
};

