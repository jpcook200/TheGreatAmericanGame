#include "RenderSystem.h"

RenderSystem::RenderSystem()
{
}

void RenderSystem::update(EntityManager& entityManager, sf::RenderWindow& window, ParticleSystem& particleSystem, bool devMode, Vec2& gridSize)
{
	drawCheckerBoard(window);
    particleSystem.draw(window);
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

        if (devMode)
        {
            drawGrid(gridSize, window);

            if (e->hasComponent<CBoundingBox>())
            {
                drawBBox(e, window);
            }
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

void RenderSystem::drawCheckerBoard(sf::RenderWindow& window)
{
    sf::View view = window.getView();
    sf::Vector2f center = view.getCenter();
    sf::Vector2f size = view.getSize();

    float tileSize = 50.f;

    // Calculate bounds of visible area
    float left = center.x - size.x / 2.f;
    float right = center.x + size.x / 2.f;
    float top = center.y - size.y / 2.f;
    float bottom = center.y + size.y / 2.f;

    // Snap to grid so it doesn't "slide"
    int startX = static_cast<int>(left / tileSize) - 1;
    int endX = static_cast<int>(right / tileSize) + 1;
    int startY = static_cast<int>(top / tileSize) - 1;
    int endY = static_cast<int>(bottom / tileSize) + 1;

    sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));

    for (int y = startY; y <= endY; y++)
    {
        for (int x = startX; x <= endX; x++)
        {
            tile.setPosition({ x * tileSize, y * tileSize });

            bool isLight = (x + y) % 2 == 0;

            tile.setFillColor(isLight
                ? sf::Color(100, 150, 255)   // light blue
                : sf::Color(50, 100, 200));  // dark blue

            window.draw(tile);
        }
    }
}

void RenderSystem::drawBBox(std::shared_ptr<Entity> e, sf::RenderWindow& window)
{
    auto& b = e->getComponent<CBoundingBox>();
    auto& t = e->getComponent<CTransform>();
    sf::RectangleShape rect;
    rect.setOrigin({ b.halfSize.x, b.halfSize.y });
    rect.setPosition(sf::Vector2f(t.pos.x, t.pos.y));
    rect.setSize({ b.size.x, b.size.y });

    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(1.f);
    rect.setOutlineColor(sf::Color::Red);

    window.draw(rect);
}

void RenderSystem::drawGrid(Vec2& gridSize, sf::RenderWindow& window)
{
    float windowWidth = (float)window.getSize().x;
    float windowHeight = (float)window.getSize().y;
    float leftX = window.getView().getCenter().x - windowWidth / 2.0;
    float rightX = leftX + windowWidth + gridSize.x;
    float nextGridX = leftX - ((int)leftX % (int)gridSize.x);

    for (float x = nextGridX; x < rightX; x += gridSize.y)
    {
        drawLine(Vec2(x, 0), Vec2(x, windowHeight), window);
    }

    for (float y = 0; y < windowHeight; y += gridSize.y)
    {
        drawLine(Vec2(leftX, windowHeight - y), Vec2(rightX, windowHeight - y), window);

    }
}
void RenderSystem::drawLine(const Vec2& p1, const Vec2& p2, sf::RenderWindow& window)
{
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(p1.x, p1.y)),
        sf::Vertex(sf::Vector2f(p2.x, p2.y))
    };

    window.draw(line, 2, sf::PrimitiveType::Lines);
}
