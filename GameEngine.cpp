#include "GameEngine.h"
#include "Scene_Menu_Main.h"

#include <iostream>

GameEngine::GameEngine(const std::string& path)
{
	init(path);
}

void GameEngine::init(const std::string& path)
{
	// Load assets
	m_assetManager.loadFromFile(path);

	sf::ContextSettings settings;
	settings.antiAliasingLevel = 8;

	// Create window
	m_window.create(
		sf::VideoMode({ 1280, 768 }),
		"The Great American Game",
		sf::Style::Default,
		sf::State::Windowed,
		settings
	);

	m_window.setFramerateLimit(60);

	// Init first scene
	changeScene("MENU", std::make_shared<Scene_Menu_Main>(this));
}

void GameEngine::update()
{
	// call the current scenes update method so each scene is in charge of how things update
	// to do once scene is created
	getCurrentScene()->update();
}

void GameEngine::sUserInput()
{
	//user input system, register key ups and downs and call each scenes do action system accordingly

	while (const std::optional event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			quit();
		}

		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			auto& actionMap = getCurrentScene()->getActionMap();
			if (actionMap.contains(keyPressed->code))
			{
				// Send action with "START" type
				getCurrentScene()->doAction(Action(actionMap.at(keyPressed->code), "START"));
			}
		}

		if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			auto& actionMap = getCurrentScene()->getActionMap();
			if (actionMap.contains(keyReleased->code))
			{
				// Send action with "END" type
				getCurrentScene()->doAction(Action(actionMap.at(keyReleased->code), "END"));
			}
		}
	}
}


void GameEngine::run()
{
	//main game loop
	while (isRunning())
	{
		m_window.clear();
		sUserInput();
		update();
		m_window.display();
	}
}

void GameEngine::quit()
{
	m_running = false;
	m_window.close();
}

sf::RenderWindow& GameEngine::getWindow()
{
	return m_window;
}

const AssetManager& GameEngine::getAssetManager() const
{
	return m_assetManager;
}

std::shared_ptr<Scene> GameEngine::getCurrentScene()
{
	return m_sceneMap.at(m_currentScene);
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	if (endCurrentScene)
	{
		m_sceneMap.erase(m_currentScene);
	}

	m_currentScene = sceneName;
	m_sceneMap[sceneName] = scene;
}

bool GameEngine::isRunning()
{
	return m_running && m_window.isOpen();
}

