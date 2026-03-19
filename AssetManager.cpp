#include "AssetManager.h"
#include <fstream>
#include <iostream>

void AssetManager::addTexture(const std::string& name, const std::string& path)
{
	sf::Texture texture;
	if (!texture.loadFromFile(path))
	{
		std::cerr << "Couldn't load texture" << std::endl;
		exit(-1);
	}
	m_textures[name] = texture;
}

void AssetManager::addAnimation(const std::string& name, Animation animation)
{
	m_animations[name] = animation;
}

void AssetManager::addFont(const std::string& name, const std::string& path)
{
	sf::Font font;
	if (!font.openFromFile(path))
	{
		std::cerr << "Couldn't load font" << std::endl;
		exit(-1);

	}

	m_fonts[name] = font;
}

const sf::Texture& AssetManager::getTexture(const std::string& name) const
{
	if (m_textures.find(name) == m_textures.end())
	{
		std::cerr << "Texture not found: " << name << std::endl;
		exit(-1);
	}
	return m_textures.at(name);
}

const Animation& AssetManager::getAnimation(const std::string& name) const
{
	return m_animations.at(name);
}

const sf::Font& AssetManager::getFont(const std::string& name) const
{
	return m_fonts.at(name);
}

void AssetManager::loadFromFile(const std::string& path)
{
	std::ifstream file(path);
	if (!file)
	{
		std::cerr << "Could not load assets config" << std::endl;
		exit(-1);
	}

	std::string head;

	while (file >> head)
	{
		if (head == "Font")
		{
			std::string font_name, font_path;
			file >> font_name >> font_path;
			addFont(font_name, "./" + font_path);
		}
		else if (head == "Texture")
		{
			std::string tex_name;
			std::string tex_path;

			file >> tex_name >> tex_path;

			std::string full_path = "./" + tex_path;

			addTexture(tex_name, full_path);

		}
		else if (head == "Animation")
		{
			std::string anim_name;
			std::string tex_name;
			size_t frames;
			float speed;

			file >> anim_name >> tex_name >> frames >> speed;
			addAnimation(anim_name, Animation(anim_name, getTexture(tex_name), frames, speed));
		}
	}

}
