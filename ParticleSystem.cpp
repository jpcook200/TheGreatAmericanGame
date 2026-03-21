#include "ParticleSystem.h"
#include <iostream>

ParticleSystem::ParticleSystem()
{
    // Initializing with 100 particles and a size of 2.0
    resetParticles(20, 1.5f);
}

void ParticleSystem::resetParticles(size_t count, float size)
{
    m_particles = std::vector<Particle>(count);
    m_vertices = sf::VertexArray(sf::PrimitiveType::Triangles, count * 6);
    m_size = size;

    for (size_t p = 0; p < m_particles.size(); p++)
    {
       resetParticle(p, sf::Vector2f(0, 0), true);
    }
}

void ParticleSystem::resetParticle(std::size_t index, sf::Vector2f pos, bool first)
{
    // Hardcoded starting point
    float mx = pos.x;
    float my = pos.y;
    float size = m_size;

    // Base index (6 vertices per quad/particle)
    std::size_t i = index * 6;

    // Define corners relative to mx, my
    sf::Vector2f topLeft = { mx - size, my - size };
    sf::Vector2f topRight = { mx + size, my - size };
    sf::Vector2f bottomRight = { mx + size, my + size };
    sf::Vector2f bottomLeft = { mx - size, my + size };

    // Triangle 1
    m_vertices[i + 0].position = topLeft;
    m_vertices[i + 1].position = topRight;
    m_vertices[i + 2].position = bottomRight;

    // Triangle 2
    m_vertices[i + 3].position = topLeft;
    m_vertices[i + 4].position = bottomRight;
    m_vertices[i + 5].position = bottomLeft;

    // Light blue "bubble" color with random transparency
    sf::Color color(173, 216, 230, 50 + rand() % 120);
    
    if (first) { color.a = 0; }
    
    for (int v = 0; v < 6; v++)
    {
        m_vertices[i + v].color = color;
    }

    float xSpread = 0.2f;
    float intensity = 0.5f;
    float minIntensity = 0.5f;


    float rx = (((float)rand() / RAND_MAX) * xSpread) - (xSpread / 2.0f);
    float ry = -(((float)rand() / RAND_MAX) * intensity + minIntensity);

    m_particles[index].velocity = sf::Vector2f(rx, ry );

    m_particles[index].lifetime = 30 + rand() % 20;
}

void ParticleSystem::update(EntityManager& entityManager, float dt)
{
    
    // --- STEP 1: SPAWNING ---
    for (auto& e : entityManager.getEntities("emitter"))
    {
        if (!e->hasComponent<CTransform>() || !e->hasComponent<CParticles>()) continue;

        std::cout << e->getComponent<CTransform>().pos.x << ", " << e->getComponent<CTransform>().pos.y << std::endl;

        auto& pComp = e->getComponent<CParticles>();
        if (!pComp.active) continue;

        pComp.timer += dt;

        while (pComp.timer >= pComp.spawnRate)
        {
            auto& pos = e->getComponent<CTransform>().pos;

 
            for (std::size_t p = 0; p < m_particles.size(); ++p)
            {
                if (m_particles[p].lifetime <= 0)
                {
                    resetParticle(p, sf::Vector2f(pos.x, pos.y), false);
                    break;
                }
            }

            pComp.timer -= pComp.spawnRate;
        }
    }

    // --- STEP 2: PHYSICS (Update all particles) ---
    for (std::size_t p = 0; p < m_particles.size(); ++p)
    {
        if (m_particles[p].lifetime <= 0) 
        {
            std::size_t i = p * 6;
            for (int v = 0; v < 6; v++)
            {
                m_vertices[i + v].position = sf::Vector2f(-100.f, -100.f);
                m_vertices[i + v].color.a = 0;
            }
            continue; 
        }

        // Normal movement logic
        std::size_t i = p * 6;
        for (int v = 0; v < 6; v++)
        {
            m_vertices[i + v].position += m_particles[p].velocity * dt * 70.f;
        
        }

        m_particles[p].lifetime--;
    }
}

void ParticleSystem::draw(sf::RenderWindow& window) const
{
    window.draw(m_vertices);
}
