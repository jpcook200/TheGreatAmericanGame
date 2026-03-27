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
	registerAction(InputKey{ InputKey::Device::Keyboard, (int)sf::Keyboard::Key::D }, "RIGHT");
	registerAction(InputKey{ InputKey::Device::Keyboard, (int)sf::Keyboard::Key::A }, "LEFT");
	registerAction(InputKey{ InputKey::Device::Keyboard, (int)sf::Keyboard::Key::W }, "UP");
	registerAction(InputKey{ InputKey::Device::Keyboard, (int)sf::Keyboard::Key::S }, "DOWN");
	registerAction(InputKey{ InputKey::Device::Keyboard, (int)sf::Keyboard::Key::U }, "TOGGLEDEV");
	registerAction(InputKey{ InputKey::Device::Mouse, (int)sf::Mouse::Button::Left }, "OPEN_E_MENU");

	loadLevel(levelPath);
}

void Scene_Play::update(float dt)
{
	m_entityManager.update();

	if (!m_pause)
	{
		movementSystem.update(m_entityManager, dt, m_game->getMousePos());
		aimSystem.update(m_entityManager, m_game->getMousePos(), dt);
		sLifespan(); // give dt
		sCollision();
		//particleSystem.update(m_entityManager, dt);
		m_currentFrame++;
	}
	animationSystem.update(m_entityManager, m_game->getAssetManager(), dt);
	sRender();
	menuSystem.update(m_game->getWindow(), dt, m_selectedEntity, m_game);
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
	renderSystem.update(m_entityManager, m_game->getWindow(), particleSystem, m_devMode, m_gridSize);
}

void Scene_Play::spawnPlayer()
{
	m_player = m_entityManager.addEntity("player");
	m_player->addComponent<CTransform>(Vec2(100, 100));
	m_player->addComponent<CInput>();
	m_player->addComponent<CState>();
	m_player->addComponent<CTransformSmooth>(
		500.f,	// acc
		250.f,	// max speed
		0.2f,	// damping
		8.f,	// flip speed
		1.f,	// tilt speed
		60.f	// tilt max angle
	);

	m_player->addComponent<CIdleBob>(
		7.f, //speed
		1.f, //amplitude
		0     //time
	);

	m_player->addComponent<CAnimation>(
		m_game->getAssetManager().getAnimation("subIdle"),
		true
	);

	m_player->addComponent<CBoundingBox>(
		Vec2(m_player->getComponent<CAnimation>().animation.getSize().x, m_player->getComponent<CAnimation>().animation.getSize().y * 0.75)
	);

	std::shared_ptr<Entity> bubbleEmitter = m_entityManager.addEntity("emitter");
	bubbleEmitter->addComponent<CParticles>(
		sf::Color(0, 0, 230), // color
		false, // active
		0, //timer
		0.03f, //spawnrate
		10, // count
		10.f // intenst
		);

	bubbleEmitter->addComponent<CTransform>();
	bubbleEmitter->getComponent<CTransform>().parent = m_player;
	bubbleEmitter->getComponent<CTransform>().offSet = Vec2(
		-m_player->getComponent<CAnimation>().animation.getSize().x / 2,
		0
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
		else if (action.getName() == "TOGGLEDEV")
		{
			m_devMode = !m_devMode;
		}
		else if (action.getName() == "OPEN_E_MENU")
		{
			sMouseInput();
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

void Scene_Play::sMouseInput()
{
	const ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse)
	{
		return;
	}

	if (m_devMode)
	{
		m_selectedEntity = nullptr;

		for (auto& e : m_entityManager.getEntities())
		{
			auto& position = e->getComponent<CTransform>().pos;
			auto& size = e->getComponent<CAnimation>().animation.getSize();
			Vec2 mousePos = m_game->getMousePos();

			if ((mousePos.x >= position.x - size.x / 2 && mousePos.x <= position.x + size.x / 2) &&
				(mousePos.y <= position.y + size.y / 2 && mousePos.y >= position.y - size.y / 2))
			{
				m_selectedEntity = e;
			}
		}
	}
	

}