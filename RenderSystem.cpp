#include "RenderSystem.h"

RenderSystem::RenderSystem()
{
}

void RenderSystem::update(EntityManager& entityManager, sf::RenderWindow& window)
{
    for (auto& e : entityManager.getEntities())
    {
        if (e->getTag() == "player" && e->hasComponent<CTransform>())
        {
            updateCamera(e, window);
            
        }

        if (e->hasComponent<CAnimation>() && e->hasComponent<CTransform>())
        {
            drawEntity(e, window);
        }
    }
}

// --- Private Helper Methods ---

void RenderSystem::updateCamera(std::shared_ptr<Entity> player, sf::RenderWindow& window)
{
    auto& transform = player->getComponent<CTransform>();
    sf::Vector2f playerPos(transform.pos.x, transform.pos.y);

    sf::View view = window.getView();
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();
    
    float zoomLevel = 0.8f; // Set this to 2.0f to see twice as much area
    sf::Vector2f baseSize = sf::Vector2f(window.getSize()); // The original window resolution
    view.setSize({baseSize.x * zoomLevel, baseSize.y * zoomLevel});

    // Dead zone calculations (1/5th of view size)
    float dzW = viewSize.x / 5.f;
    float dzH = viewSize.y / 5.f;

    float left = viewCenter.x - dzW / 2.f;
    float right = viewCenter.x + dzW / 2.f;
    float top = viewCenter.y - dzH / 2.f;
    float bottom = viewCenter.y + dzH / 2.f;

    // Shift target center if player exits dead zone
    if (playerPos.x < left)        viewCenter.x -= (left - playerPos.x);
    else if (playerPos.x > right)  viewCenter.x += (playerPos.x - right);

    if (playerPos.y < top)         viewCenter.y -= (top - playerPos.y);
    else if (playerPos.y > bottom) viewCenter.y += (playerPos.y - bottom);

    // Smooth Lerp
    float lerpSpeed = 0.1f;
    sf::Vector2f currentCenter = view.getCenter();
    view.setCenter(currentCenter + (viewCenter - currentCenter) * lerpSpeed);

    window.setView(view);
}

void RenderSystem::drawEntity(std::shared_ptr<Entity> e, sf::RenderWindow& window)
{
    auto& t = e->getComponent<CTransform>();
    auto& anim = e->getComponent<CAnimation>().animation;

    anim.getSprite().setPosition({ t.pos.x, t.pos.y });
    anim.getSprite().setScale({ t.scale.x, t.scale.y });
    anim.getSprite().setRotation(sf::degrees(t.angle));

    window.draw(anim.getSprite());
}
