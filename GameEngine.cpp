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
	settings.antiAliasingLevel = 16;

	// Create window
	m_window.create(
		sf::VideoMode({ 1280, 768 }),
		"The Great American Game",
		sf::Style::Default,
		sf::State::Windowed,
		settings
	);

	ImGui::SFML::Init(m_window);
	ImGui::GetIO().FontGlobalScale = 1.5f;
	m_window.setFramerateLimit(60);

	// Init first scene
	changeScene("MENU", std::make_shared<Scene_Menu_Main>(this));
}

void GameEngine::update(float dt)
{
	// call the current scenes update method so each scene is in charge of how things update
	// to do once scene is created
	getCurrentScene()->update(dt);
}

void GameEngine::sUserInput()
{
	//user input system, register key ups and downs and call each scenes do action system accordingly

	while (const std::optional event = m_window.pollEvent())
	{
		ImGui::SFML::ProcessEvent(m_window, *event);
		auto& actionMap = getCurrentScene()->getActionMap();

		if (event->is<sf::Event::Closed>())
		{
			quit();
		}

		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			int keyCode = static_cast<int>(keyPressed->code);
			InputKey inputKey{ InputKey::Device::Keyboard, keyCode };
			if (actionMap.contains(inputKey))
			{
				// Send action with "START" type
				getCurrentScene()->doAction(Action(actionMap.at(inputKey), "START"));
			}
		}

		if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			int keyCode = static_cast<int>(keyReleased->code);
			InputKey inputKey{ InputKey::Device::Keyboard, keyCode };
			if (actionMap.contains(inputKey))
			{
				// Send action with "END" type
				int keyCode = static_cast<int>(keyReleased->code);
				getCurrentScene()->doAction(Action(actionMap.at(inputKey), "END"));
			}
		}

		//mouse 
		if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			int mouseButtonCode = static_cast<int>(mouseButtonPressed->button);
			InputKey inputKey{ InputKey::Device::Mouse, mouseButtonCode };
			
			if (actionMap.contains(inputKey))
			{
				getCurrentScene()->doAction(Action(actionMap.at(inputKey), "START"));
			}
		}

		if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
		{
			int mouseButtonCode = static_cast<int>(mouseButtonReleased->button);
			InputKey inputKey{ InputKey::Device::Keyboard, mouseButtonCode };

			if (actionMap.contains(inputKey))
			{
				getCurrentScene()->doAction(Action(actionMap.at(inputKey), "END"));
			}
		}
	}
}


void GameEngine::run()
{
	sf::Clock deltaClock;
	//main game loop
	while (isRunning())
	{
		sf::Time dt = deltaClock.restart();
		float deltaTime = dt.asSeconds();
		ImGui::SFML::Update(m_window, dt);
		m_window.clear();
		sUserInput();
		update(deltaTime);
		
		ImGui::SFML::Render(m_window);
		m_window.display();
	}

	ImGui::SFML::Shutdown();

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

Vec2 GameEngine::getMousePos()
{
	// get the current mouse position in the game world and return it as a Vec2
	sf::Vector2f mousePos = getWindow().mapPixelToCoords(sf::Mouse::getPosition(getWindow()));
	
	return Vec2(mousePos.x, mousePos.y);
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

