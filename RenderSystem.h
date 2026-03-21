#pragma once

#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "ParticleSystem.h"

class RenderSystem
{
	void updateCamera(std::shared_ptr<Entity> player, sf::RenderWindow& window);
	void drawEntity(std::shared_ptr<Entity> e, sf::RenderWindow& window);

	void drawCheckerBoard(sf::RenderWindow& window);

public:
	RenderSystem();
	void update(EntityManager& entityManager, sf::RenderWindow& window, ParticleSystem& particleSystem);
	
};

