#include "MovementSystem.h"
#include "iostream"

MovementSystem::MovementSystem()
{
}

void MovementSystem::update(EntityManager& m) {
    for (auto& e : m.getEntities()) {
        if (!e->hasComponent<CTransform>()) continue;

        auto& transform = e->getComponent<CTransform>();

        // Process Input and Physics Logic
        if (e->hasComponent<CInput>() && e->hasComponent<CTransformSmooth>()) {
            handleInput(e);
        }

        // Process Aesthetic/Idle Logic
        if (e->hasComponent<CIdleBob>()) {
            handleIdleBob(e);
        }

        // Finalize Position (Movement or Hierarchy)
        updateTransform(e);
		
    }
}

// --- Private Helper Methods ---

void MovementSystem::handleInput(std::shared_ptr<Entity> e) {
    auto& input = e->getComponent<CInput>();
    auto& transform = e->getComponent<CTransform>();
    auto& smooth = e->getComponent<CTransformSmooth>();
    auto& state = e->getComponent<CState>();

    Vec2 inputDir(0, 0);
    if (input.up)    inputDir.y -= 1;
    if (input.down)  inputDir.y += 1;
    if (input.left)  inputDir.x -= 1;
    if (input.right) inputDir.x += 1;

    if (inputDir.length() > 0) {
        state.set(PlayerState::MOVE);
        inputDir = inputDir.normalized();
        transform.velocity += inputDir * smooth.acceleration;

        // Clamp speed
        if (transform.velocity.length() > smooth.maxSpeed) {
            transform.velocity = transform.velocity.normalized() * smooth.maxSpeed;
        }
    }
    else {
        state.set(PlayerState::IDLE);
        applyDamping(transform, smooth);
    }
	std::cout << inputDir.x << " , " << inputDir.y << std::endl;
}

void MovementSystem::applyDamping(CTransform& t, CTransformSmooth& s) {
    t.velocity *= s.damping;
    if (std::abs(t.velocity.x) < 0.01f) t.velocity.x = 0;
    if (std::abs(t.velocity.y) < 0.01f) t.velocity.y = 0;
}

void MovementSystem::handleIdleBob(std::shared_ptr<Entity> e) {
    auto& t = e->getComponent<CTransform>();
    auto& bob = e->getComponent<CIdleBob>();

    bob.time += bob.speed;
    t.pos.y += std::sin(bob.time) * bob.amplitude;
}

void MovementSystem::updateTransform(std::shared_ptr<Entity> e) {
    auto& t = e->getComponent<CTransform>();
    t.prevPos = t.pos;

    if (t.parent == nullptr) {
        t.pos += t.velocity;
    }
    else {
        resolveParentChild(e);
    }
}

void MovementSystem::resolveParentChild(std::shared_ptr<Entity> e) {
    auto& t = e->getComponent<CTransform>();
    auto& p = t.parent->getComponent<CTransform>();

    Vec2 offset = t.offSet;
    offset.x *= p.scale.x;
    offset.y *= p.scale.y;

    float rad = p.angle * 3.14159f / 180.f;
    Vec2 rotatedOffset = {
        offset.x * cos(rad) - offset.y * sin(rad),
        offset.x * sin(rad) + offset.y * cos(rad)
    };

    t.pos = p.pos + rotatedOffset;
    t.scale = p.scale;
}
