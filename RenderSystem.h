#pragma once

#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "ParticleSystem.h"
#include <memory>
#include "Vec2.h"

class RenderSystem
{
	void updateCamera(std::shared_ptr<Entity> player, sf::RenderWindow& window);
	void drawEntity(std::shared_ptr<Entity> e, sf::RenderWindow& window);

	void drawCheckerBoard(sf::RenderWindow& window);

public:
	RenderSystem();
	void update(EntityManager& entityManager, sf::RenderWindow& window, ParticleSystem& particleSystem, bool devMode, Vec2& gridSize);
	void drawBBox(std::shared_ptr<Entity> e, sf::RenderWindow& window);
	void drawGrid(Vec2& gridSize, sf::RenderWindow& window);
	void drawLine(const Vec2& p1, const Vec2& p2, sf::RenderWindow&);
};

