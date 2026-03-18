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
		sMovement.update(m_entityManager);
		sAim.update(m_entityManager, m_game->getMousePos());
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
	spawnPlayer();
}

void Scene_Play::spawnPlayer()
{
	m_player = m_entityManager.addEntity("player");

	m_player->addComponent<CTransform>(Vec2(100, 100));
	
	m_player->addComponent<CAnimation>(
		m_game->getAssetManager().getAnimation("subMove"),
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
		0.1f,
		0.2f,
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

void Scene_Play::sAnimation()
{
	// First take the player velocity and set the player state based on the direction of movement, also set the facing and scale for the player sprite
	auto& transform = m_player->getComponent<CTransform>();
	auto& stateComp = m_player->getComponent<CState>();
	auto& input = m_player->getComponent<CInput>();

	 std::cout << "Player State: " << static_cast<int>(stateComp.state) << "\n";
	// handle changing animations 
	if (stateComp.changeAnimate)
	{
		std::string aniName;

		switch (stateComp.state)
		{
			case PlayerState::IDLE:
				std::cout << "player idle" << "\n";
				aniName = "subIdle"; 
				break;
			case PlayerState::MOVE:
				aniName = "subMove";
				break;
			default: 
				transform.angle = 0;
				aniName = "subIdle";
				break;
		}

		// Add or replace the CAnimation component
		m_player->addComponent<CAnimation>(
			m_game->getAssetManager().getAnimation(aniName),
			true
		);
	}

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
	auto& transform = m_player->getComponent<CTransform>();
	sf::Vector2f playerPos(transform.pos.x, transform.pos.y);

	// Get the current view
	sf::View view = m_game->getWindow().getView();
	sf::Vector2f viewCenter = view.getCenter();
	sf::Vector2f viewSize = view.getSize();

	// Define the dead zone rectangle (e.g., 1/3 of the view size)
	float deadZoneWidth = viewSize.x / 5.f;
	float deadZoneHeight = viewSize.y / 5.f;

	float left = viewCenter.x - deadZoneWidth / 2.f;
	float right = viewCenter.x + deadZoneWidth / 2.f;
	float top = viewCenter.y - deadZoneHeight / 2.f;
	float bottom = viewCenter.y + deadZoneHeight / 2.f;

	// Check if player is outside dead zone, move view accordingly
	if (playerPos.x < left)
		viewCenter.x -= left - playerPos.x;
	else if (playerPos.x > right)
		viewCenter.x += playerPos.x - right;

	if (playerPos.y < top)
		viewCenter.y -= top - playerPos.y;
	else if (playerPos.y > bottom)
		viewCenter.y += playerPos.y - bottom;

	// Smooth camera movement (optional)
	float lerpSpeed = 0.1f;
	view.setCenter(view.getCenter() + (viewCenter - view.getCenter()) * lerpSpeed);

	// Apply the view
	m_game->getWindow().setView(view);

	// Clear the window
	m_game->getWindow().clear(sf::Color(50, 50, 255));

	// Clear window first
	m_game->getWindow().clear(sf::Color::Black); // base color

	// Get window size
	sf::Vector2u winSize = m_game->getWindow().getSize();

	// Size of each checker square
	float tileSize = 50.f;

	// Draw checkerboard
	for (float y = 0; y < winSize.y; y += tileSize)
	{
		for (float x = 0; x < winSize.x; x += tileSize)
		{
			// Determine color: alternating
			bool isWhite = (int(x / tileSize) + int(y / tileSize)) % 2 == 0;
			sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
			tile.setPosition({ x, y });
			tile.setFillColor(isWhite ? sf::Color(200, 200, 200) : sf::Color(100, 100, 100));

			m_game->getWindow().draw(tile);
		}
	}

	// After this, draw all your entities as usual
	// Draw all entities
	for (auto& e : m_entityManager.getEntities())
	{
		if (e->hasComponent<CAnimation>() && e->hasComponent<CTransform>())
		{
			auto& t = e->getComponent<CTransform>();
			auto& anim = e->getComponent<CAnimation>().animation;

			anim.getSprite().setPosition({ t.pos.x, t.pos.y });
			anim.getSprite().setScale({ t.scale.x, t.scale.y });

			sf::Angle angle = sf::degrees(t.angle);
			anim.getSprite().setRotation(angle);

			m_game->getWindow().draw(anim.getSprite());
		}
	}
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
