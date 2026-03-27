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
#include "ParticleSystem.h"
#include "MenuSystem.h"

class Scene_Play : public Scene
{
    struct PlayerConfig
    {
        float X, Y, CX, CY, SPEED, MAXSPEED;
        std::string WEAPON;
    };

protected:
    std::shared_ptr<Entity> m_player;
    std::shared_ptr<Entity> m_selectedEntity = nullptr;
    std::string m_levelPath;
    PlayerConfig m_playerConfig;

    bool m_drawTextures = true;
    bool m_drawCollision = false;
    bool m_drawGrid = false;
    bool m_devMode = false;

    const Vec2 m_gridSize = { 32, 32 };
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

    void sMouseInput();

    //systems
    MovementSystem movementSystem;
    AimSystem aimSystem;
	AnimationSystem animationSystem;
    RenderSystem renderSystem;
    ParticleSystem particleSystem;
    MenuSystem menuSystem;
	    
    float toolMountOffsetX = 18.f;
	float toolMountOffsetY = 14.4f;

public:
    Scene_Play(GameEngine* gameEngine, const std::string& levelPath);
    void update(float dt);
};