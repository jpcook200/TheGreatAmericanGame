#pragma once
#include "EntityManager.h"

class AimSystem
{
	/*
		@brief Helper method to update the entity's flip based on the direction to the mouse.
		@param transform - reference to the entity's CTransform component
		@param dir - the direction vector from the entity to the mouse position
	*/
	void updateEntityFlip(CTransform& transform, const Vec2& dir);

	/*
		@brief Helper method to update the entity's rotation to face the mouse position.
		@param e - shared pointer to the entity being updated
		@param transform - reference to the entity's CTransform component
		@param angle - the angle in degrees from the entity to the mouse position
	*/
	void updateAimRotation(std::shared_ptr<Entity> e, CTransform& transform, float angle);

public:
	AimSystem();
	/*
		@brief Update method for the AimSystem. 
		This method will update the rotation of all entities with the "player" tag to face the mouse position. 
		It will also flip the entity's sprite if the mouse is to the left of the entity.
		@param entityManager - reference to the EntityManager instance to access the entities
		@param mousePos - the current position of the mouse in world coordinates
	*/
	void update(EntityManager& entityManager, Vec2 mousePos);

};

