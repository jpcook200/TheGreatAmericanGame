#include "EntityManager.h"
#include <iostream>

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
	// every frame
	//for all entities in entities to add-> add them to the main entities vector
	for (auto& e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->getTag()].push_back(e);

	}

	m_entitiesToAdd.clear();

	//remove dead entities from the main entities vector
	removeDeadEntities(m_entities);

	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}

}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{

	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	m_entitiesToAdd.push_back(entity);

	return entity;
}

const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	static EntityVec emptyVec;

	auto it = m_entityMap.find(tag);

	if (it != m_entityMap.end())
	{
		return it->second;
	}

	return emptyVec;
}

const std::map<std::string, EntityVec> EntityManager::getEntityMap()
{
	return m_entityMap;
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	std::erase_if(
		vec,
		[](std::shared_ptr<Entity> e) {

			return !e->isActive();
		}
	);
}