#include "Animation.h"
#include <iostream>

Animation::Animation() {}

Animation::Animation(const std::string& name, const sf::Texture& t)
	: Animation(name, t, 1, 0)
{
}

Animation::Animation(
	const std::string& name,
	const sf::Texture& t,
	size_t frameCount,
	float speed)
	: m_sprite(t),
	m_frameCount(frameCount),
	m_currentFrame(0),
	m_speed(speed),
	m_name(name),
	m_timer(0.0f)
{
	// initlaise the size of the frame
	m_size = Vec2(t.getSize().x / frameCount, t.getSize().y);

	// move origin of the sprite to be central with the entitiy potision
	m_sprite.setOrigin({ m_size.x / 2.f, m_size.y / 2.f });

	// set the sprites tecture rectangle with int rect
	m_sprite.setTextureRect(
		sf::IntRect(
			{ (int)(std::floor(m_currentFrame) * m_size.x), 0 },
			{ (int)m_size.x, (int)m_size.y }
		)
	);
}

void Animation::update(float dt)
{
	// If speed is 0, the animation is static
	if (m_speed == 0)
	{
		return;
	}

	// 1. Accumulate time using dt
	// You will need to add 'float m_timer = 0.0f;' to your Animation.h class members
	m_timer += dt;

	// 2. Check if enough time has passed to move to the next frame
	if (m_timer >= m_speed)
	{
		m_currentFrame++;
		m_timer = 0.0f; // Reset timer for the next frame
	}

	// 3. Calculate which frame to show (looping automatically)
	size_t animFrame = m_currentFrame % m_frameCount;

	// 4. Update the Sprite's texture rectangle
	m_sprite.setTextureRect(
		sf::IntRect(
			{ (int)(animFrame * m_size.x), 0 },
			{ (int)m_size.x, (int)m_size.y }
		)
	);
}

bool Animation::hasEnded() const
{
	return false;
}

const std::string& Animation::getName() const
{
	return m_name;
}

const Vec2& Animation::getSize() const
{
	return m_size;
}

sf::Sprite& Animation::getSprite()
{
	return m_sprite;
}

void Animation::setOrigin(float x, float y)
{
	m_sprite.setOrigin({x, y});
}
