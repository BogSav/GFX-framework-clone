#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Lab4 : public gfxc::SimpleScene
    {
     public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Lab4();
        ~Lab4();

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

        void RenderScene();
     protected:
        glm::mat4 modelMatrix;
        float translateX, translateY, translateZ;
        float scaleX, scaleY, scaleZ;
        float angularStepOX, angularStepOY, angularStepOZ;
        GLenum polygonMode;
        ViewportArea miniViewportArea;

        glm::vec3 m_cube1MovingDirection;
        float m_cube1Speed;

        float m_cube2ScaleSpeed;
        int m_cube2ScaleingDirection;

        glm::vec3 m_cube3RotationDirection;
        float m_cube3RotationSpeed;

        glm::vec2 m_viewPortMovingDirection;
        int m_viewPortScalingDirection;
        float m_viewPortSpeed;
        float m_viewPortX, m_viewPortY;
        float m_viewPortScalingSpeed;
        float m_viewPortWidth, m_viewPortHeight;
    };
}   // namespace m1

