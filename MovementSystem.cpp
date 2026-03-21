#include "MovementSystem.h"
#include "iostream"

MovementSystem::MovementSystem()
{
}

//-------------------------------MAIN UPDATE METHOD-------------------------------------------

void MovementSystem::update(EntityManager& m, float dt, Vec2 mousePos) {
    for (auto& e : m.getEntities()) {
        if (!e->hasComponent<CTransform>()) continue;

        auto& transform = e->getComponent<CTransform>();

        // Process Input and Physics Logic
        if ((e->hasComponent<CInput>() || e->getComponent<CTransform>().parent->hasComponent<CInput>()) 
            && (e->hasComponent<CTransformSmooth>() || e->getComponent<CTransform>().parent->hasComponent<CTransformSmooth>()))
        {
            handleInput(e, dt);
            auto& smooth = e->getComponent<CTransformSmooth>();
            
            if (e->getTag() == "player")
            {
                Vec2 dirToMouse = mousePos - transform.pos;
                flipEntity(transform, smooth.flipSpeed, dirToMouse, dt);
            }
        }

        // Process Aesthetic/Idle Logic
        if (e->hasComponent<CIdleBob>()) {
            handleIdleBob(e, dt);
        }

        // Finalize Position (Movement or Hierarchy)
        
        updateTransform(e, dt);
		
    }
}

// --- Private Helper Methods ---

void MovementSystem::handleInput(std::shared_ptr<Entity> e, float dt) {
    auto& input = e->getComponent<CInput>();
    if (e->getComponent<CTransform>().parent != nullptr)
    {
        input = e->getComponent<CTransform>().parent->getComponent<CInput>();
    }
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
        if (e->hasComponent<CParticles>())
        {
            e->getComponent<CParticles>().active = true;
        }
        inputDir = inputDir.normalized();
        transform.velocity += inputDir * smooth.acceleration * dt;
        
        // Clamp speed
        if (transform.velocity.length() > smooth.maxSpeed) {
            transform.velocity = transform.velocity.normalized() * smooth.maxSpeed;
        }
    }
    else {
        state.set(PlayerState::IDLE);
        applyDamping(transform, smooth, dt);
        if (e->hasComponent<CParticles>())
        {
            e->getComponent<CParticles>().active = false;
        }
    }


}

void MovementSystem::applyDamping(CTransform& t, CTransformSmooth& s, float dt) {
    t.velocity *= std::pow(s.damping, dt);

    if (abs(t.velocity.x) < 0.001f) t.velocity.x = 0;
	if (abs(t.velocity.y) < 0.001f) t.velocity.y = 0;
}

void MovementSystem::handleIdleBob(std::shared_ptr<Entity> e, float dt) {
    auto& t = e->getComponent<CTransform>();
    auto& bob = e->getComponent<CIdleBob>();

    bob.time += dt * bob.speed;
    float offset = std::sin(bob.time) * bob.amplitude;
    t.pos.y += offset * dt * 10.0f;
}

void MovementSystem::updateTransform(std::shared_ptr<Entity> e, float dt) {
    auto& t = e->getComponent<CTransform>();
    t.prevPos = t.pos;

    if (t.parent == nullptr) {
        t.pos += t.velocity * dt;
    }
    else {
        resolveParentChild(e);
    }
}

void MovementSystem::resolveParentChild(std::shared_ptr<Entity> e) {
    auto& t = e->getComponent<CTransform>();
    auto& p = t.parent->getComponent<CTransform>();

    Vec2 offset = t.offSet;

    // convert offset on parent flip
    offset.x *= p.scale.x;
    offset.y *= p.scale.y;

    float rad = p.angle * 3.14159f / 180.0f;

    // create new offset using arc math
    Vec2 rotatedOffset = {
        (offset.x * cos(rad)) - (offset.y * sin(rad)),
        (offset.x * sin(rad)) + (offset.y * cos(rad))
    };

    t.pos = p.pos + rotatedOffset;
    t.angle = p.angle;
    t.scale = p.scale;
}

void MovementSystem::flipEntity(CTransform& transform, float flipSpeed, const Vec2& dir, float dt)
{
    float targetScale = (dir.x < 0) ? -1.f : 1.f;
 

    if (transform.scale.x < targetScale)
    {
        transform.scale.x = std::min(targetScale, transform.scale.x + flipSpeed * dt);
    }
    else if (transform.scale.x > targetScale)
    {
        transform.scale.x = std::max(targetScale, transform.scale.x - flipSpeed * dt);
    }
}

void MovementSystem::updateTilt(CTransform& transform, CTransformSmooth& smooth, float inputY, float dt)
{
    float maxAngle = smooth.tiltMaxAngle;
    float tiltSpeed = smooth.tiltSpeed;
    float targetAngle = 0.0f;

    // 1. Get the current facing direction (1.0 for Right, -1.0 for Left)
    float facingDir = (transform.scale.x < 0) ? -1.0f : 1.0f;

    // 2. Determine the Target Angle
    if (inputY != 0)
    {
        targetAngle = (inputY * maxAngle * facingDir);
    }
    else
    {
        targetAngle = 0.0f;
        tiltSpeed = 12.0f;
    }

    float angleDiff = targetAngle - transform.angle;
    transform.angle += angleDiff * tiltSpeed * dt;

    if (std::abs(angleDiff) < 0.1f) {
        transform.angle = targetAngle;
    }

}
