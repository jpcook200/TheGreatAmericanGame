#pragma once
#include "EntityManager.h"

class AimSystem
{
	void updateEntityFlip(CTransform& transform, const Vec2& dir);
	void updateAimRotation(std::shared_ptr<Entity> e, CTransform& transform, float angle);

public:
	AimSystem();
	void update(EntityManager& entityManager, Vec2 mousePos);

};

