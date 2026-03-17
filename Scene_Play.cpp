#include "Scene_Play.h"
#include <fstream>
#include <iostream>

Scene_Play::Scene_Play(GameEngine* gameEngine, const std::string& levelPath)
	:	Scene(gameEngine),
		m_levelPath(levelPath)
{
	init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath)
{
	loadLevel(levelPath);
}

void Scene_Play::update()
{
	m_entityManager.update();

	if (!m_pause)
	{
		sMovement();
		sLifespan();
		sCollision();
		m_currentFrame++;
	}

	sAnimation();
	sRender();
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	return Vec2();
}

void Scene_Play::loadLevel(const std::string& filename)
{

}

void Scene_Play::spawnPlayer()
{

}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
}

void Scene_Play::sMovement()
{
	// Apply movement to all entities
	for (auto& e : m_entityManager.getEntities())
	{
		if (e->hasComponent<CTransform>())
		{
			auto& t = e->getComponent<CTransform>();
			t.prevPos = t.pos;
			t.pos += t.velocity;
		}
	}
}

void Scene_Play::sLifespan()
{
}

void Scene_Play::sCollision()
{

}

void Scene_Play::sAnimation()
{
	// Update all entity animations
	for (auto& e : m_entityManager.getEntities())
	{
		if (e->hasComponent<CAnimation>())
		{
			e->getComponent<CAnimation>().animation.update();
		}
	}
}

void Scene_Play::sRender()
{
	for (auto& e : m_entityManager.getEntities())
	{
		//render textures
		if (e->hasComponent<CAnimation>() && e->hasComponent<CTransform>())
		{
			auto& transform = e->getComponent<CTransform>();
			auto& animation = e->getComponent<CAnimation>().animation;

			animation.getSprite().setPosition({ transform.pos.x, transform.pos.y });
			animation.getSprite().setScale({ transform.scale.x, transform.scale.y });


			sf::Angle angle = sf::degrees(transform.angle);
			animation.getSprite().setRotation(angle);

			m_game->getWindow().draw(animation.getSprite());
		}
	}

	if (m_drawGrid)
	{
		float windowWidth = (float)m_game->getWindow().getSize().x;
		float windowHeight = (float)m_game->getWindow().getSize().y;
		float leftX = m_game->getWindow().getView().getCenter().x - windowWidth / 2.0;
		float rightX = leftX + windowWidth + m_gridSize.x;
		float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

		for (float x = nextGridX; x < rightX; x += m_gridSize.y)
		{
			drawLine(Vec2(x, 0), Vec2(x, windowHeight));
		}

		for (float y = 0; y < windowHeight; y += m_gridSize.y)
		{
			drawLine(Vec2(leftX, windowHeight - y), Vec2(rightX, windowHeight - y));

		}
	}

}

void Scene_Play::sDoAction(const Action& action)
{
	if (action.getType() == "START")
	{
		
	}
	else if (action.getType() == "END")
	{

	}

}

void Scene_Play::onEnd()
{
}

void Scene_Play::setPaused(bool)
{
}

void Scene_Play::changePlayerStateTo(PlayerState s)
{
	auto& stateComp = m_player->getComponent<CState>();

	// If the new state is different from the current one
	if (stateComp.state != s)
	{
		stateComp.state = s;
		stateComp.changeAnimate = true;
	}
	else
	{
		// If we are already in the requested state, do nothing
		stateComp.changeAnimate = false;
	}
}
