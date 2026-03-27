#include "Scene_Menu_Main.h"
#include "Scene_Play.h"


Scene_Menu_Main::Scene_Menu_Main(GameEngine* gameEngine)
	: Scene(gameEngine),
	m_menuText(m_game->getAssetManager().getFont("mainFont"), m_title)
{
	init();
}

void Scene_Menu_Main::init()
{
	registerAction(InputKey{ InputKey::Device::Keyboard, (int)sf::Keyboard::Key::Space }, "PLAY");
	registerAction(InputKey{ InputKey::Device::Keyboard, (int)sf::Keyboard::Key::Escape }, "QUIT");

	int titleSize = 60;
	m_menuText.setCharacterSize(titleSize);
	m_menuText.setFillColor(sf::Color::Black);
	m_menuText.setPosition({ m_game->getWindow().getSize().x / 2.0f, 100.0f });

}

void Scene_Menu_Main::update(float dt)
{
	sRender();
}

void Scene_Menu_Main::onEnd()
{
	m_game->quit();
}

void Scene_Menu_Main::sDoAction(const Action& action)
{
	// perform actions in the scene
	if (action.getType() == "START")
	{
		if (action.getName() == "PLAY")
		{
			m_game->changeScene("PLAY", std::make_shared<Scene_Play>(m_game, ""));
			
		}
	}
}

void Scene_Menu_Main::sRender()
{
	m_game->getWindow().clear(sf::Color(50, 50, 255));
	m_game->getWindow().draw(m_menuText);
}
