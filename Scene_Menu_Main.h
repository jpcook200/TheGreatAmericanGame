#pragma once

#include "GameEngine.h"
#include "AssetManager.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <iostream>

class Scene_Menu_Main : public Scene

{
protected:
	std::string m_title = "The Great American Game";	// the game menu title
	sf::Text m_menuText;								// would be the displayed version of the game title

	std::vector<std::string> m_menuStrings;				// vector of menu button texts
	std::vector<std::string> m_levelPaths;				// vector of level paths to load levels from
	/*std::vector<sf::Text> m_menuItems;	*/
	std::vector<std::unique_ptr<sf::Text>> m_menuItems;	// would be a vector of actual displayed menu items
	size_t m_selectedMenuIndex = 0;						// selected index of menu

	void init();										// initialise the menu scene
	void update();										// run any updates in the game loop		
	void onEnd();										// methods runs when the scene ends
	void sDoAction(const Action& action);				// get action type and perform action

public:
	Scene_Menu_Main(GameEngine* gameEngine = nullptr);		// constructor
	void sRender();										// render method to render the buttons
};