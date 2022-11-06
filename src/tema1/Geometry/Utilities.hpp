#pragma once

#include "utils/glm_utils.h"
#include <numbers>

namespace TranformUtils
{
    template<typename T>
    struct VLSpace
    {
        VLSpace(T x, T y, T width, T height)
            : 
            x(x), 
            y(y), 
            width(width), 
            height(height),
            rightX(x + width),
            upperY(y + height)
        {}
        VLSpace()
            :
            VLSpace(0, 0, 1, 1)
        {}
    public:
        void SetX(T x)
        {
            this->x = x;
            UpdateParams();
        }
        void SetY(T y)
        {
            this->y = y;
            UpdateParams();
        }
        void SetWidth(T width)
        {
            this->width = width;
            UpdateParams();
        }
        void SetHeight(T height)
        {
            this->height = height;
            UpdateParams();
        }
        T GetRightX() const
        {
            return rightX;
        }
        T GetUpperY() const
        {
            return upperY;
        }
        T GetX() const
        {
            return x;
        }
        T GetY() const
        {
            return y;
        }
        T GetWidth() const
        {
            return width;
        }
        T GetHeight() const
        {
            return height;
        }
    private:
        void UpdateParams()
        {
            rightX = x + width;
            upperY = y + height;
        }
    private:
        T x;
        T y;
        T width;
        T height;
        T rightX;
        T upperY;
    };
    typedef VLSpace<float> LogicSpace;
    typedef VLSpace<int> ViewportSpace;


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

    inline glm::mat3 ReflectionMatrixOY(int direction)
    {
        return glm::transpose(
            glm::mat3(
                direction, 0, 0,
                0, 1, 0,
                0, 0, 1)
        );
    }

    inline glm::mat3 ReflectionMatrixOX(int direction)
    {
        return glm::transpose(
            glm::mat3(
                1, 0, 0,
                0, direction, 0,
                0, 0, 1)
        );
    }

    inline glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
    {
        float sx, sy, tx, ty;
        sx = viewSpace.GetWidth() / logicSpace.GetWidth();
        sy = viewSpace.GetHeight() / logicSpace.GetHeight();
        tx = viewSpace.GetX() - sx * logicSpace.GetX();
        ty = viewSpace.GetY() - sy * logicSpace.GetY();

        return glm::transpose(glm::mat3(
            sx, 0.0f, tx,
            0.0f, sy, ty,
            0.0f, 0.0f, 1.0f)
        );
    }

    inline  glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
    {
        float sx, sy, tx, ty, smin;
        sx = viewSpace.GetWidth() / logicSpace.GetWidth();
        sy = viewSpace.GetHeight() / logicSpace.GetHeight();
        if (sx < sy)
            smin = sx;
        else
            smin = sy;
        tx = viewSpace.GetX() - smin * logicSpace.GetX() + (viewSpace.GetWidth() - smin * logicSpace.GetWidth()) / 2;
        ty = viewSpace.GetX() - smin * logicSpace.GetY() + (viewSpace.GetHeight() - smin * logicSpace.GetHeight()) / 2;

        return glm::transpose(glm::mat3(
            smin, 0.0f, tx,
            0.0f, smin, ty,
            0.0f, 0.0f, 1.0f)
        );
    }
}

namespace CollisionUtils
{
    enum CollisionType
    {
        RIGHT_WALL = 0,
        LEFT_WALL = 1,
        UPPER_WALL = 2,
        BOTTOM_WALL = 3,
        NO_COLLISION = 4
    };

    struct CollInfo
    {
        bool collisionDetected = false;
        float collisionAngle = 0.f;
        CollisionType collisionType;
    };
}

namespace MySafeGeometry
{
    inline float MyNorm(const glm::vec3 vec)
    {
        return glm::sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    }
    inline glm::vec3 MyGetNormalized(const glm::vec3 vec)
    {
        return vec / MyNorm(vec);
    }
    inline void MyNormalize(glm::vec3& vec)
    {
        vec = vec / MyNorm(vec);
    }
    inline float MyDot(const glm::vec3 v1, const glm::vec3 v2)
    {
        return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
    }
    inline float MyGetAngleBetween(glm::vec3 v1, glm::vec3 v2)
    {
        return glm::acos(MyDot(v1, v2) / (MyNorm(v1) * MyNorm(v2)));
    }
    inline float MyRad2Deg(const float angle)
    {
        return angle * 180.0f / std::numbers::pi_v<float>;
    }
    inline float MyDeg2Rad(const float angle)
    {
        return angle * std::numbers::pi_v<float> / 180.f;
    }
    inline glm::vec3 MyReflectByTheNormal(glm::vec3 n, const glm::vec3 v)
    {
        MyNormalize(n);
        return v - 2 * MyDot(n, v) * n;
    }
    inline void MyCounterClockwiseRotateVectorWithAngle(glm::vec3& v, float theta)
    {
        const float y = v[1];
        const float cos_f = glm::cosf(theta);
        const float sin_f = glm::sinf(theta);

        v[0] = v[0] * cos_f - v[1] * sin_f;
        v[1] = y * sin_f + v[1] * cos_f;
    }
    inline void MyClockwiseRotateVectorWithAngle(glm::vec3& v, float theta)
    {
        MyCounterClockwiseRotateVectorWithAngle(v, -theta);
    }
    inline glm::vec3 MyGetCounterClockwiseRotatedVector(glm::vec3 v, float theta)
    {
        glm::vec3 arr = v;
        MyCounterClockwiseRotateVectorWithAngle(arr, theta);
        return arr;
    }
    inline glm::vec3 MyVectorMatrixProduct(glm::vec3 v, glm::mat3 mat)
    {
        return 
        {
            v[0] * mat[0][0] + v[1] * mat[1][0] + v[2] * mat[2][0],
            v[1] * mat[0][1] + v[1] * mat[1][1] + v[2] * mat[2][1],
            v[1] * mat[0][2] + v[1] * mat[1][2] + v[2] * mat[2][2],
        };
    }
    inline glm::vec3 MyVectorMatrixProduct(glm::vec2 v, glm::mat3 mat)
    {
        return
        {
            v[0] * mat[0][0] + v[1] * mat[1][0],
            v[1] * mat[0][1] + v[1] * mat[1][1],
            0,
        };
    }
}