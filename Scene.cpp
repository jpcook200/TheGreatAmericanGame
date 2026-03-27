#include "Scene.h"
#include <iostream>

Scene::Scene()
{
}

Scene::Scene(GameEngine* gameEngine)
	: m_game(gameEngine)
{
}

Scene::~Scene()
{
}

void Scene::doAction(const Action& action)
{
	sDoAction(action);
}

void Scene::simulate(const size_t frames)
{
}

void Scene::registerAction(InputKey inputKey, const std::string& actionName)
{
	m_actionMap[inputKey] = actionName;
}

void Scene::setPaused(bool paused)
{
	m_pause = !paused;
}

size_t Scene::getWidth() const
{
	return m_game->getWindow().getSize().x;
}

size_t Scene::getHeight() const
{
	return m_game->getWindow().getSize().y;
}

size_t Scene::getCurrentFrame() const
{
	return m_currentFrame;
}

bool Scene::hasEnded() const
{
	return m_hasEnded;
}

ActionMap& Scene::getActionMap()
{
	return m_actionMap;
}

void Scene::drawLine(const Vec2& p1, const Vec2& p2)
{
	sf::Vertex line[] = {
		sf::Vertex(sf::Vector2f(p1.x, p1.y)),
		sf::Vertex(sf::Vector2f(p2.x, p2.y))
	};

	m_game->getWindow().draw(line, 2, sf::PrimitiveType::Lines);
}
