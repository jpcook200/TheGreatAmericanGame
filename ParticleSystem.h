#pragma once

#include "Vec2.h"
#include "EntityManager.h"
#include <vector>
#include <SFML/Graphics.hpp>

class ParticleSystem
{
    struct Particle
    {
        sf::Vector2f velocity = { 0, 0 };
        int lifetime = 0;
    };

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    float m_size = 2.0f;

    void resetParticles(size_t count, float size);
    void resetParticle(std::size_t index, sf::Vector2f pos, bool first = false);

public:
    ParticleSystem();
    void update(EntityManager& entityManager, float dt);
    void draw(sf::RenderWindow& window) const;
};

