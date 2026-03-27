#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include <memory>
#include "GameEngine.h"

class MenuSystem
{
	void drawMenu(std::shared_ptr<Entity> e, GameEngine* game);

public:
	MenuSystem() = default;
	void update(sf::RenderWindow& window, float dt, std::shared_ptr<Entity> selectedEntity, GameEngine* game);

};
