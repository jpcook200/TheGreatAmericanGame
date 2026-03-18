#include "AimSystem.h"
#include <iostream>

AimSystem::AimSystem()
{
}

void AimSystem::update(EntityManager& m, Vec2 mousePos)
{
    for (auto& e : m.getEntities())
    {
        if (!e->hasComponent<CAim>() || !e->hasComponent<CTransform>()) continue;

        auto& transform = e->getComponent<CTransform>();
        auto& aim = e->getComponent<CAim>();

        // 1. Common Calculation: Vector from entity to mouse
        Vec2 dir = mousePos - transform.pos;
        float targetAngle = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;

        // 2. Handle Body Orientation (The "Shoulder" flip)
        if (aim.shouldFlip)
        {
            updateEntityFlip(transform, dir);
        }

        // 3. Handle Aim Direction (The "Weapon" rotation)
        if (aim.followMouse)
        {
            updateAimRotation(e, transform, targetAngle);
        }
    }
}

// --- Private Helpers ---

void AimSystem::updateEntityFlip(CTransform& transform, const Vec2& dir)
{
    // Smoothly interpolate scale.x to create a "turning" effect
    float targetScale = (dir.x < 0) ? -1.f : 1.f;
    transform.scale.x += (targetScale - transform.scale.x) * 0.1f;
}

void AimSystem::updateAimRotation(std::shared_ptr<Entity> e, CTransform& transform, float angle)
{
    // Check if this entity is a "Child" (like a gun attached to a player)
    if (transform.parent && transform.parent->hasComponent<CTransform>())
    {
        auto& pTransform = transform.parent->getComponent<CTransform>();

        if (pTransform.scale.x < 0)
        {
            transform.angle = angle + 180.0f;
        }
        else
        {
            transform.angle = angle;
        }
    }
    else
    {
        // Standalone entity (like a turret)
        transform.angle = angle;
    }
}