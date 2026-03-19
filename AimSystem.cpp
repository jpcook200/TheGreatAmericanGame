#include "AimSystem.h"
#include <iostream>
#include <cmath>
#include <numbers>
#include "Utils.h"

AimSystem::AimSystem()
{
}

void AimSystem::update(EntityManager& m, Vec2 mousePos, float dt)
{
    for (auto& e : m.getEntities())
    {
        if (!e->hasComponent<CAim>() || !e->hasComponent<CTransform>()) continue;

        auto& transform = e->getComponent<CTransform>();
        auto& aim = e->getComponent<CAim>();

        //Common Calculation: Vector from entity to mouse
        Vec2 dir = mousePos - transform.pos;

		//get target angle in degrees using atan2
        float targetAngle = utils::angleBetween(transform.pos, mousePos);

        // Handle Aim Direction
        if (aim.followMouse)
        {
            updateAimRotation(e, transform, targetAngle);
        }
    }
}

// --- Private Helpers ---

void AimSystem::updateAimRotation(std::shared_ptr<Entity> e, CTransform& transform, float angle)
{
    // Check if this entity has a parent
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
        // Standalone entity
        transform.angle = angle;
    }
}