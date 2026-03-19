#pragma once

#include <string>
#include <memory>

#include "EntityManager.h"
#include "Vec2.h";
#include <SFML/Graphics.hpp>
#include "Physics.h"
#include "Action.h"
#include "Components.h"
#include "GameEngine.h"

#include "MovementSystem.h"
#include "AimSystem.h"
#include "AnimationSystem.h"
#include "RenderSystem.h"

class Scene_Play : public Scene
{
    struct PlayerConfig
    {
        float X, Y, CX, CY, SPEED, MAXSPEED;
        std::string WEAPON;
    };

protected:
    std::shared_ptr<Entity> m_player;
    std::string m_levelPath;
    PlayerConfig m_playerConfig;

    bool m_drawTextures = true;
    bool m_drawCollision = false;
    bool m_drawGrid = false;

    const Vec2 m_gridSize = { 64, 64 };
    /*sf::Text m_gridText;*/
    //Physics m_worldPhysics;

    void init(const std::string&);
    Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);
    void loadLevel(const std::string& filename);
    void sRender();
    void spawnPlayer();
    void spawnBullet(std::shared_ptr<Entity> entity);
    void sLifespan();
    void sCollision();
    void sDoAction(const Action& action);
    void onEnd();
    void setPaused(bool);

    //systems
    MovementSystem MovementSystem;
    AimSystem AimSystem;
	AnimationSystem AnimationSystem;
    RenderSystem RenderSystem;
	    
    float toolMountOffsetX = 18.f;
	float toolMountOffsetY = 14.4f;

public:
    Scene_Play(GameEngine* gameEngine, const std::string& levelPath);
    void update(float dt);
};