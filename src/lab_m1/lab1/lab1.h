#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Lab1 : public gfxc::SimpleScene
    {
     public:
        Lab1();
        ~Lab1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        // TODO(student): Class variables go here
    private:
        std::vector<float> m_clearColor;

        size_t m_currentMeshIdx;
        std::vector<std::string> m_meshNames;

        const glm::vec3 m_initialPosition;

        float m_speed;
        bool m_movementKeyBeingPressed = false;
        int m_multipleDirectionsCount = 0;
        glm::vec3 m_velocity;
        glm::vec3 m_totalMovement;

        float m_initialHeight;
        float m_jumpHeight;
        float m_jumpSpeed;
        float m_gravitationalDeceleration;
        bool m_jumpUp = false;
        bool m_jumpDown = false;
    };
}   // namespace m1
