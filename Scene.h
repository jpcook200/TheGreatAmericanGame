#pragma once

#include "GameEngine.h"
#include "EntityManager.h"
#include "Action.h"
#include "Vec2.h"

#include <cstddef>
#include <map>
#include <SFML/Graphics.hpp>

class GameEngine;												// allow access to game engine without circular dependency

struct InputKey
{
	enum Device
	{
		Mouse,
		Keyboard
	};

	Device device;
	int code;

	bool operator<(const InputKey& other) const
	{
		if (device != other.device)
			return static_cast<int>(device) < static_cast<int>(other.device);

		return code < other.code;
	}
};

typedef std::map<InputKey, std::string> ActionMap;				// a map connecting an action name to an key Reference

class Scene
{
protected:

	GameEngine* m_game = nullptr;								// save current game engin einstance to member variables for use
	EntityManager m_entityManager;								// each scene has its own entitymanager
	ActionMap m_actionMap;										// map of keyboard inputs to action names
	bool m_pause = false;										// boolean to start game to paused or play
	bool m_hasEnded = false;									// not sure
	size_t m_currentFrame = 0;									// the current game frame

	virtual void onEnd() = 0;									// virtual on end function not sure what its for yet
	void setPaused(bool paused);								// set the game to pause - pause systems

public:
	Scene();													// default constructor
	Scene(GameEngine* gameEngine);								// constructor
	virtual ~Scene();											// destructor

	virtual void update(float dt) = 0;							// virtual void update function to update any scene info, must be included in child classes
	virtual void sDoAction(const Action& action) = 0;			// action system which converts actions to output created in child class
	virtual void sRender() = 0;									// render items in the scene, cerated in the child

	virtual void doAction(const Action& action);				// calls sDoAction allowing the childs set actions to work
	void simulate(const size_t frames);							// simulate the scene 
	void registerAction(										// initlise actions and their keys in the map
		InputKey inputKey,
		const std::string& actionName
	);

	size_t getWidth() const;									// allow other classes to get the scene width
	size_t getHeight() const;									// allow other classes to get scene height
	size_t getCurrentFrame() const;								// allow other classes to get current game frame in scene

	bool hasEnded() const;										// flag for has ended - not sure
	ActionMap& getActionMap();									// allow other classes to access the action map		
	void drawLine(												// draw a line on the screen
		const Vec2& p1,
		const Vec2& p2
	);

};