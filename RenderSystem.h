#pragma once

#include <SFML/Graphics.hpp>
#include "EntityManager.h"

class RenderSystem
{
	void updateCamera(std::shared_ptr<Entity> player, sf::RenderWindow& window);
	void drawEntity(std::shared_ptr<Entity> e, sf::RenderWindow& window);

public:
	RenderSystem();
	void update(EntityManager& entityManager, sf::RenderWindow& window);
	
};

