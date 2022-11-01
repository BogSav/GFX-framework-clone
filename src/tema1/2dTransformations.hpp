#pragma once

#include "utils/glm_utils.h"

namespace TranformUtils
{
    struct LogicSpace
    {
        LogicSpace() : x(0), y(0), width(1), height(1) {}
        LogicSpace(float x, float y, float width, float height)
            : x(x), y(y), width(width), height(height) {}
        float x;
        float y;
        float width;
        float height;
    };

    struct ViewportSpace
    {
        ViewportSpace() : x(0), y(0), width(1), height(1) {}
        ViewportSpace(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}
        int x;
        int y;
        int width;
        int height;
    };


    inline glm::mat3 Translate(float translateX, float translateY)
    {
        return glm::transpose(
            glm::mat3(
                1, 0, translateX,
                0, 1, translateY,
                0, 0, 1)
        );
    }

    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        return glm::transpose(
            glm::mat3(
                scaleX, 0, 0,
                0, scaleY, 0,
                0, 0, 1)
        );
    }

    inline glm::mat3 Rotate(float radians)
    {
        float sinp = glm::sinf(radians);
        float cosp = glm::cosf(radians);
        return glm::transpose(
            glm::mat3(
                cosp, -sinp, 0,
                sinp, cosp, 0,
                0, 0, 1)
        );
    }

    inline glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
    {
        float sx, sy, tx, ty;
        sx = viewSpace.width / logicSpace.width;
        sy = viewSpace.height / logicSpace.height;
        tx = viewSpace.x - sx * logicSpace.x;
        ty = viewSpace.y - sy * logicSpace.y;

        return glm::transpose(glm::mat3(
            sx, 0.0f, tx,
            0.0f, sy, ty,
            0.0f, 0.0f, 1.0f)
        );
    }

    inline  glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
    {
        float sx, sy, tx, ty, smin;
        sx = viewSpace.width / logicSpace.width;
        sy = viewSpace.height / logicSpace.height;
        if (sx < sy)
            smin = sx;
        else
            smin = sy;
        tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
        ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

        return glm::transpose(glm::mat3(
            smin, 0.0f, tx,
            0.0f, smin, ty,
            0.0f, 0.0f, 1.0f)
        );
    }
}