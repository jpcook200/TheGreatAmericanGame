#pragma once
#include "EntityManager.h"
#include "Vec2.h"

class MovementSystem
{
	void handleInput(std::shared_ptr<Entity> e, float dt);
	void applyDamping(CTransform& t, CTransformSmooth& s, float dt);
	void handleIdleBob(std::shared_ptr<Entity> e, float dt);
	void updateTransform(std::shared_ptr<Entity> e, float dt);
	void resolveParentChild(std::shared_ptr<Entity> e);
	/*
		@brief Helper method to update the entity's flip based on the direction to the mouse.
		@param transform - reference to the entity's CTransform component
		@param dir - the direction vector from the entity to the mouse position
		@param dt - the delta time since the last update, used to smooth the flipping transition
	*/
	void flipEntity(CTransform& transform, float flipSpeed, const Vec2& dir, float dt);
	void updateTilt(CTransform& transform, CTransformSmooth& smooth, float inputY, float dt);

public:
	MovementSystem();
	void update(EntityManager& entityManager, float dt, Vec2 mousePos);

};

