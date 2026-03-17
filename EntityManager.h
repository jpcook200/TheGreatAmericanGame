#pragma once

#include "Entity.h"
#include <vector>
#include <map>
#include <memory>
#include <string>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;				// type definition of a vecotr of pointers to entities
typedef std::map<std::string, EntityVec> EntityMap;					// a map of string labels to entity vectors

class EntityManager
{
	EntityVec m_entities;											// all entities
	EntityVec m_entitiesToAdd;										// vector of entities to spawn next frame
	EntityMap m_entityMap;											// map of entity vectors
	size_t m_totalEntities = 0;										// number of entities

	void removeDeadEntities(EntityVec& vec);						// method to delete entities from the vector

public:
	EntityManager();												// constructor
	void update();													// update method runs every frame

	std::shared_ptr<Entity> addEntity(const std::string& tag);		// add entity to vector and map

	const EntityVec& getEntities();									// get all entities
	const EntityVec& getEntities(const std::string& tag);			// get entities by tag
	const std::map<std::string, EntityVec> getEntityMap();			// get the entities map

};