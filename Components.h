#pragma once

#include "Animation.h"
class Entity;

enum struct PlayerState 
{
    IDLE,
    MOVE
};

enum struct PlayerFacing
{
    RIGHT,
    LEFT
};



class Component
{
public:
    bool has = false;
};

class CTransform : public Component
{
public:
	std::shared_ptr<Entity> parent = nullptr;
	Vec2 offSet = { 0.0, 0.0 };
    Vec2 pos = { 0.0, 0.0 };
    Vec2 prevPos = { 0.0, 0.0 };
    Vec2 scale = { 1.0, 1.0 };
    Vec2 velocity = { 0.0, 0.0 };
    float angle = 0;

    CTransform() {};
    CTransform(const Vec2& p) : pos(p) {};
    CTransform(const Vec2& p, const Vec2& sp, const Vec2& sc, float a)
        : pos(p), prevPos(p), scale(sc), velocity(sp), angle(a) {
    };

};

class CTransformSmooth : public Component
{
public:
    float acceleration;
    float maxSpeed;
    float damping;

	CTransformSmooth() {};
    CTransformSmooth(float a, float ms, float d)
        : acceleration(a), maxSpeed(ms), damping(d) {
	}

};

class CAim : public Component
{
public:
    float angle = 0;
    bool shouldFlip = false;
    bool followMouse = false;

	CAim() {};
    CAim(float a, bool shouldFlip, bool followMouse) : angle(a), shouldFlip(shouldFlip), followMouse(followMouse) {};
};

class CIdleBob : public Component
{
public:
    float speed;
    float amplitude;
    float time;

    CIdleBob() {};
    CIdleBob(float speed, float amplitude, float time) : speed(speed), amplitude(amplitude), time(time) {};
};

class CInput : public Component
{
public:
    bool up = false;
    bool left = false;
    bool right = false;
    bool down = false;
    bool shoot = false;
    bool canShoot = false;
    bool canJump = false;

    CInput() {};
};

class CAnimation : public Component
{
public:
    Animation animation;
    bool repeat = false;

    CAnimation() {};
    CAnimation(const Animation& animation, bool r)
        : animation(animation), repeat(r) {
    };
};

class CState : public Component
{
public:
    PlayerState state;
    PlayerState preState;
    PlayerFacing facing = PlayerFacing::LEFT;
    bool changeAnimate = false;

    void set(PlayerState newState)
    {
        if (state != newState)
        {
            state = newState;
            changeAnimate = true;
        }
    }

    CState() {};
    CState(const PlayerState s) : state(s), preState(s) {};
};

class CBoundingBox : public Component
{
public:
    Vec2 size;
    Vec2 halfSize;
    CBoundingBox() {}
    CBoundingBox(const Vec2& s)
        : size(s), halfSize(s.x / 2.0, s.y / 2.0) {
    }
};

class CLifespan : public Component
{
public:
    int lifespan = 0;
    int frameCreated = 0;
    CLifespan() {}
    CLifespan(int duration, int frame)
        : lifespan(duration), frameCreated(frame) {
    }
};

class CGravity : public Component
{
public:
    float gravity = 0;
    CGravity() {}
    CGravity(float g) : gravity(g) {}
};