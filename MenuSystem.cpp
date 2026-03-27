#include "MenuSystem.h"
#include <SFML/Graphics.hpp>

void MenuSystem::drawMenu(std::shared_ptr<Entity> e, GameEngine* game)
{
    if (!e) return;

    std::string eName = e->getTag();
    ImGui::Begin(eName.c_str());

    // --- Sprite ---
    if (e->hasComponent<CAnimation>())
    {
        auto& anim = e->getComponent<CAnimation>();
        sf::Sprite& sprite = anim.animation.getSprite();

        const sf::Texture& texRef = sprite.getTexture();
        const sf::Texture* texture = &texRef;

        if (texture)
        {
            ImTextureID texID = (ImTextureID)texture->getNativeHandle();

            auto rect = sprite.getTextureRect();
            auto texSize = texture->getSize();

            ImVec2 uv0(
                (float)rect.position.x / texSize.x,
                (float)rect.position.y / texSize.y
            );

            ImVec2 uv1(
                (float)(rect.position.x + rect.size.x) / texSize.x,
                (float)(rect.position.y + rect.size.y) / texSize.y
            );

            float previewSize = 128.0f;

            float scaleX = previewSize / rect.size.x;
            float scaleY = previewSize / rect.size.y;
            float scale = std::min(scaleX, scaleY);

            ImVec2 imageSize(rect.size.x * scale, rect.size.y * scale);

            ImGui::Text("Sprite Preview");
            ImGui::Image(texID, imageSize, uv0, uv1);

            ImGui::Dummy(ImVec2(0.0f, 20.0f));
        }
    }

    // --- Transform ---
    if (e->hasComponent<CTransform>())
    {
        auto& t = e->getComponent<CTransform>();
        ImGui::Text("CTransform");
        ImGui::SliderFloat("Pos X", &t.pos.x, -1000.f, 1000.f);
        ImGui::SliderFloat("Pos Y", &t.pos.y, -1000.f, 1000.f);
        ImGui::SliderFloat("Offset X", &t.offSet.x, -300.f, 300.f);
        ImGui::SliderFloat("Offset Y", &t.offSet.y, -300.f, 300.f);
        ImGui::SliderFloat("Scale X", &t.scale.x, 0.1f, 10.f);
        ImGui::SliderFloat("Scale Y", &t.scale.y, 0.1f, 10.f);
        ImGui::SliderFloat("Velocity X", &t.velocity.x, -500.f, 500.f);
        ImGui::SliderFloat("Velocity Y", &t.velocity.y, -500.f, 500.f);
        ImGui::SliderFloat("Angle", &t.angle, -360.f, 360.f);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
    }

    // --- CTransformSmooth ---
    if (e->hasComponent<CTransformSmooth>())
    {
        auto& ts = e->getComponent<CTransformSmooth>();
        ImGui::Text("CTransformSmooth");
        ImGui::SliderFloat("Acceleration", &ts.acceleration, 0.f, 1000.f);
        ImGui::SliderFloat("Max Speed", &ts.maxSpeed, 0.f, 1000.f);
        ImGui::SliderFloat("Damping", &ts.damping, 0.f, 1.f);
        ImGui::SliderFloat("Flip Speed", &ts.flipSpeed, 0.f, 100.f);
        ImGui::SliderFloat("Tilt Speed", &ts.tiltSpeed, 0.f, 100.f);
        ImGui::SliderFloat("Tilt Max Angle", &ts.tiltMaxAngle, 0.f, 200.f);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
    }

    // --- CAim ---
    if (e->hasComponent<CAim>())
    {
        auto& aim = e->getComponent<CAim>();
        ImGui::Text("CAim");
        ImGui::SliderFloat("Aim Angle", &aim.angle, -360.f, 360.f);
        ImGui::Checkbox("Follow Mouse", &aim.followMouse);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
    }

    // --- CIdleBob ---
    if (e->hasComponent<CIdleBob>())
    {
        auto& bob = e->getComponent<CIdleBob>();
        ImGui::Text("CIdleBob");
        ImGui::SliderFloat("Speed", &bob.speed, 0.f, 20.f);
        ImGui::SliderFloat("Amplitude", &bob.amplitude, 0.f, 100.f);
        ImGui::SliderFloat("Time", &bob.time, 0.f, 100.f);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
    }

    // --- CInput ---
    if (e->hasComponent<CInput>())
    {
        auto& input = e->getComponent<CInput>();
        ImGui::Text("CInput");
        ImGui::Checkbox("Up", &input.up);
        ImGui::Checkbox("Down", &input.down);
        ImGui::Checkbox("Left", &input.left);
        ImGui::Checkbox("Right", &input.right);
        ImGui::Checkbox("Shoot", &input.shoot);
        ImGui::Checkbox("Can Shoot", &input.canShoot);
        ImGui::Checkbox("Can Jump", &input.canJump);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
    }

    // --- CState ---
    if (e->hasComponent<CState>())
    {
        auto& state = e->getComponent<CState>();
        ImGui::Text("CState");
        ImGui::SliderInt("State", reinterpret_cast<int*>(&state.state), 0, 10);
        ImGui::SliderInt("Previous State", reinterpret_cast<int*>(&state.preState), 0, 10);
        ImGui::Checkbox("Change Animate", &state.changeAnimate);
        // facing
        ImGui::SliderInt("Facing", reinterpret_cast<int*>(&state.facing), 0, 1);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
    }

    // --- CBoundingBox ---
    if (e->hasComponent<CBoundingBox>())
    {
        auto& bb = e->getComponent<CBoundingBox>();
        ImGui::Text("CBoundingBox");
        ImGui::SliderFloat("Width", &bb.size.x, 0.f, 500.f);
        ImGui::SliderFloat("Height", &bb.size.y, 0.f, 500.f);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
    }

    // --- CLifespan ---
    if (e->hasComponent<CLifespan>())
    {
        auto& life = e->getComponent<CLifespan>();
        ImGui::Text("CLifespan");
        ImGui::SliderInt("Lifespan", &life.lifespan, 0, 10000);
        ImGui::SliderInt("Frame Created", &life.frameCreated, 0, 10000);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
    }

    // --- CGravity ---
    if (e->hasComponent<CGravity>())
    {
        auto& grav = e->getComponent<CGravity>();
        ImGui::Text("CGravity");
        ImGui::SliderFloat("Gravity", &grav.gravity, -1000.f, 1000.f);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
    }

    ImGui::End();
}

void MenuSystem::update(sf::RenderWindow& window, float dt, std::shared_ptr<Entity> selectedEntity, GameEngine* game)
{
	if (selectedEntity != nullptr)
	{
		drawMenu(selectedEntity, game);
	}
}
