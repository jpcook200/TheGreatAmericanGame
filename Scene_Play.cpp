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
	//register actions for the scene
	registerAction(sf::Keyboard::Key::D, "RIGHT");
	registerAction(sf::Keyboard::Key::A, "LEFT");
	registerAction(sf::Keyboard::Key::W, "UP");
	registerAction(sf::Keyboard::Key::S, "DOWN");

	loadLevel(levelPath);
}

void Scene_Play::update()
{
	m_entityManager.update();

	if (!m_pause)
	{
		MovementSystem.update(m_entityManager);
		AimSystem.update(m_entityManager, m_game->getMousePos());
		sLifespan();
		sCollision();
		m_currentFrame++;
	}

	AnimationSystem.update(m_entityManager, m_game->getAssetManager());
	sRender();
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	return Vec2();
}

void Scene_Play::loadLevel(const std::string& filename)
{
	spawnPlayer();
}

void Scene_Play::sRender()
{
	RenderSystem.update(m_entityManager, m_game->getWindow());
}

void Scene_Play::spawnPlayer()
{
	m_player = m_entityManager.addEntity("player");

	m_player->addComponent<CTransform>(Vec2(100, 100));
	
	m_player->addComponent<CAnimation>(
		m_game->getAssetManager().getAnimation("subIdle"),
		true
	);

	m_player->addComponent<CInput>();
	m_player->addComponent<CAim>(
		0,
		true,
		false
	);
	m_player->addComponent<CState>();
	m_player->addComponent<CTransformSmooth>(
		0.9f,	// acc
		5.f,	// max speed
		0.9f	// damping
	);

	m_player->addComponent<CIdleBob>(
		0.1f, //speed
		0.2f, //amplitude
		0     //time
	);

	std::shared_ptr<Entity> toolMount = m_entityManager.addEntity("player_tool_mount");
	toolMount->addComponent<CAnimation>(
		m_game->getAssetManager().getAnimation("toolMount"),
		true
	);

	toolMount->addComponent<CTransform>();

	toolMount->getComponent<CTransform>().parent = m_player;
	toolMount->getComponent<CTransform>().offSet = Vec2(toolMountOffsetX, toolMountOffsetY);

	std::shared_ptr<Entity> gun = m_entityManager.addEntity("player_gun");
	gun->addComponent<CAnimation>(
		m_game->getAssetManager().getAnimation("gun"),
		true
	);
	gun->addComponent<CTransform>();
	gun->getComponent<CTransform>().offSet = Vec2(0, 0);
	gun->getComponent<CTransform>().parent = toolMount;
	gun->addComponent<CAim>(
		0,
		false,
		true
	);

	Animation& gunAnim = gun->getComponent<CAnimation>().animation;
	gunAnim.getSprite().setOrigin({ 0.5, gunAnim.getSize().y / 2 });

}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
}

void Scene_Play::sLifespan()
{
}

void Scene_Play::sCollision()
{

}

void Scene_Play::sDoAction(const Action& action)
{
	if (action.getType() == "START")
	{
		if (action.getName() == "RIGHT")
		{
			m_player->getComponent<CInput>().right = true;
		}
		else if (action.getName() == "LEFT")
		{
			m_player->getComponent<CInput>().left = true;
		}
		else if (action.getName() == "UP")
		{
			m_player->getComponent<CInput>().up = true;
		}
		else if (action.getName() == "DOWN")
		{
			m_player->getComponent<CInput>().down = true;
		}
		
	}
	else if (action.getType() == "END")
	{
		if (action.getName() == "RIGHT")
		{
			m_player->getComponent<CInput>().right = false;
		}
		else if (action.getName() == "LEFT")
		{
			m_player->getComponent<CInput>().left = false;
		}
		else if (action.getName() == "UP")
		{
			m_player->getComponent<CInput>().up = false;
		}
		else if (action.getName() == "DOWN")
		{
			m_player->getComponent<CInput>().down = false;
		}
	}

}

void Scene_Play::onEnd()
{
}

void Scene_Play::setPaused(bool)
{
}

