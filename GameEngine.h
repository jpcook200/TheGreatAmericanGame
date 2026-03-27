#pragma once

#include "Scene.h"
#include "AssetManager.h"

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

#include <memory>
#include <map>

class Scene;																// forward reference to scene class
typedef std::map <std::string, std::shared_ptr<Scene>> SceneMap;			// create a map that allows scene pointers to match to a string name

class GameEngine
{
protected:
	sf::RenderWindow m_window;												// sfml's window class
	sf::ContextSettings settings;
	AssetManager m_assetManager;											// create am instance of the assets class in the game engine to allow assets to be loaded
	std::string m_currentScene;												// a string that equates to the current scene in the game, same as the the string in the map
	SceneMap m_sceneMap;													// instance of the scene map created above
	size_t m_simulationSpeed = 1;											// speed of simulation for simulating the game without user input
	bool m_running = true;													// main boolean to allow game to run or quit

	void init(const std::string& path);										// main init method called in the constructor - takes a path of a config file to init variables
	void update(float dt);															// runs every frame to update physics or anything else in the loop
	void sUserInput();														// system that takes user inputs and creates actions in the currentScene
	std::shared_ptr<Scene> getCurrentScene();								// method to get the currentScene returns a pointer to the scene

public:
	GameEngine(const std::string& path);									// constructor, runs on instantiation in the main function

	void changeScene(														// switch to a new scene
		const std::string& sceneName,
		std::shared_ptr<Scene> scene,
		bool endCurrentScene = false
	);

	void quit();															// exit the game entirely
	void run();																// main game loop runs and creates frames

	sf::RenderWindow& getWindow();											// get the actual game window instance
	const AssetManager& getAssetManager() const;							// get the actual instantiated assets object
	Vec2 getMousePos();														// get the mouse position in the game world
	bool isRunning();														// return true or false if game is running or not

};