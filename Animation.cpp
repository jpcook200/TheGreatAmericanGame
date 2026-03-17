#include "Animation.h"

Animation::Animation() {}

Animation::Animation(const std::string& name, const sf::Texture& t)
	: Animation(name, t, 1, 0)
{
}

Animation::Animation(
	const std::string& name,
	const sf::Texture& t,
	size_t frameCount,
	size_t speed)
	: m_sprite(t),
	m_frameCount(frameCount),
	m_currentFrame(0),
	m_speed(speed),
	m_name(name)
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

void Animation::update()
{

	if (m_speed == 0)
	{
		return;
	}

	// increment the current frame
	m_currentFrame++;

	//frame number is the left over after taking the mod of the framecount and incorporate speed to slow it down or speed up
	size_t animFrame = (m_currentFrame / m_speed) % m_frameCount;

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
