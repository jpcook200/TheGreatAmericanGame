#include "AnimationSystem.h"
#include "iostream"

AnimationSystem::AnimationSystem()
{
}

void AnimationSystem::update(EntityManager& entityManager, const AssetManager& assetManager, float dt)
{
	// Update all entity animations
	for (auto& e : entityManager.getEntities())
	{
		if (e->hasComponent<CAnimation>())
		{
			if (e->getTag() == "player")
			{
				// First take the player velocity and set the player state based on the direction of movement, also set the facing and scale for the player sprite
				auto& transform = e->getComponent<CTransform>();
				auto& state = e->getComponent<CState>();
				auto& input = e->getComponent<CInput>();

				// handle changing animations 
				if (state.changeAnimate)
				{
					std::string aniName;

					switch (state.state)
					{
					case PlayerState::IDLE:
						aniName = "subIdle";
						break;
					case PlayerState::MOVE:
						aniName = "subMove";
						break;
					default:
						aniName = "subIdle";
						break;
					}

					// Add or replace the CAnimation component
					e->addComponent<CAnimation>(
						assetManager.getAnimation(aniName),
						true
					);

					state.changeAnimate = false;
				}
			}
			e->getComponent<CAnimation>().animation.update(dt);
		}
	}
}
