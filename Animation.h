#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Vec2.h"

class Animation
{
	sf::Texture initialTexture;
	sf::Sprite m_sprite{ initialTexture };							// the sprite for the animation - shows a rectangle inside the sprite sheet
	size_t m_frameCount = 1;										// total number of frames for the animation
	size_t m_currentFrame = 0;										// the current active frame
	size_t m_speed = 0;												// speed of playing
	Vec2 m_size = { 1, 1 };											// size of one frame
	std::string m_name = "none";									// animation name

public:
	Animation();
	Animation(const std::string& name, const sf::Texture& t);		// another constructor with default frame count and speed
	Animation(														// another contructor for cutom frameCount and speed?
		const std::string& name,
		const sf::Texture& t,
		size_t frameCount,
		size_t speed
	);

	void update();													// method that updates the current frame to new part of sprite sheet
	bool hasEnded() const;											// flag if the animation has ended
	const std::string& getName() const;								// get the name of the animation
	const Vec2& getSize() const;									// get the sie of one frame
	sf::Sprite& getSprite();										// get the actual sprite by reference
};