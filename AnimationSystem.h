#pragma once
#include "EntityManager.h"
#include "AssetManager.h"

/**
 * @class AnimationSystem
 * @brief Synchronizes entity states with their visual representations.
 * * The AnimationSystem is responsible for:
 * 1. Detecting state changes (e.g., IDLE to MOVE).
 * 2. Swapping the CAnimation component's texture sheet when states flip.
 * 3. Calling the internal update() on every active Animation to advance frames.
 */
class AnimationSystem
{
public:
    /**
     * @brief Default constructor for the Animation System.
     */
    AnimationSystem();

    /**
     * @brief Processes all entities and updates their current animation frame.
     * * This method performs a two-pass check:
     * - First, it checks if the entity's CState::changeAnimate flag is set. If so,
     * it retrieves the correct Animation from the AssetManager and replaces
     * the current CAnimation component.
     * - Second, it calls the update() method on the CAnimation component to
     * handle frame timing and texture rectangle shifting.
     * * @param entityManager The manager containing all entities to be updated.
     * @param assetManager The library where pre-loaded Animations are stored.
     * * @note This should be called once per frame, ideally after the MovementSystem
     * but before the RenderSystem.
     */
    void update(EntityManager& entityManager, const AssetManager& assetManager, float dt);
};