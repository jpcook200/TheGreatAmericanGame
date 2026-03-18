#pragma once
#include "EntityManager.h"
#include "Vec2.h"

class MovementSystem
{
	void handleInput(std::shared_ptr<Entity> e);
	void applyDamping(CTransform& t, CTransformSmooth& s);
	void handleIdleBob(std::shared_ptr<Entity> e);
	void updateTransform(std::shared_ptr<Entity> e);
	void resolveParentChild(std::shared_ptr<Entity> e);

public:
	MovementSystem();
	void update(EntityManager& entityManager);

};

