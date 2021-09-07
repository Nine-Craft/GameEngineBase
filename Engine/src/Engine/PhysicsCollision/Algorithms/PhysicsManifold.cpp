/************************************************************************************//*!
\file          PhysicsManifold.cpp
\project       <PROJECT_NAME>
\author        Chua Teck Lee, c.tecklee, 390008420
\par           email: c.tecklee\@digipen.edu
\date          September 2, 2021
\brief

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "PhysicsManifold.h"

#include <oom/oom.hpp>

#include "Collision.h"
#include "Engine/PhysicsCollision/Manifold.h"

#include "Engine/PhysicsCollision/Colliders.h"
#include "Engine/PhysicsCollision/RigidBody.h"

namespace engine
{
    Manifold2D PhysicsManifold::GenerateManifold2D(CircleCollider2D const& circle, CircleCollider2D const& circle2)
    {
        Manifold2D result {};

        auto circleA = circle.GetGlobalBounds();
        auto circleB = circle2.GetGlobalBounds();

        vec2 dirVec = circleB.center - circleA.center;
        // find distance
        float dist = oom::length(dirVec);

        // if the two circles are placed onto each other
        if (dist == 0.f)
        {
            // Choose random (but consistent) values
            result.PenetrationDepth = circleA.radius;
            result.Normal = vec2{ 1, 0 };
            result.Position = circleA.center;
        }
        else
        {
            result.PenetrationDepth = (circleA.radius + circleB.radius) - dist;
            // Utilize our d since we performed sqrt on it already
            result.Normal = dirVec / dist;

            // Take A's position and move it along the contact normal
            // the distance of A's radius
            //c->position = c->normal * A->r + a->pos;
            result.Position = result.Normal * circleA.radius + circleA.center;
        }

        result.HasCollision = true;

        return result;

    }

    Manifold2D PhysicsManifold::GenerateManifold2D(CircleCollider2D const& circle, BoxCollider2D const& aabb)
    {
        Manifold2D result{};

        // Need to update proeprly

        auto circleA = circle.GetGlobalBounds();
        auto boxB = aabb.GetGlobalBounds();

        auto boxCenter = (boxB.max + boxB.min) * 0.5f;
        vec2 dirVec = boxCenter - circleA.center;
        
        auto closest = dirVec;
        float x_extent = (boxB.max.x - boxB.min.x) * 0.5f;
        float y_extent = (boxB.max.y - boxB.min.y) * 0.5f;

        closest.x = std::clamp(closest.x, -x_extent, x_extent);
        closest.y = std::clamp(closest.y, -y_extent, y_extent);

        bool inside = false;

        if (closest == dirVec)
        {
            inside = true;
            if (oom::abs(dirVec.x) > oom::abs(dirVec.y))
            {
                if (closest.x > 0) closest.x = x_extent;
                else closest.x = -x_extent;
            }
            else
            {
                if (closest.y > 0) closest.y = y_extent;
                else closest.y = -y_extent;
            }
        }

        result.Normal = oom::normalize(boxCenter - closest);
        result.PenetrationDepth = circleA.radius - oom::length(boxCenter - closest);

        if (inside)
        {
            result.Normal *= -1;
        }

        result.HasCollision = true;

        return result;
    }

    Manifold2D PhysicsManifold::GenerateManifold2D(BoxCollider2D const& aabb, CircleCollider2D const& circle)
    {
        Manifold2D result{};

        auto boxA = aabb.GetGlobalBounds();
        auto circleB = circle.GetGlobalBounds();

        auto boxCenter = (boxA.max + boxA.min) * 0.5f;
        vec2 dirVec = circleB.center - boxCenter;

        auto closest = dirVec;
        float x_extent = (boxA.max.x - boxA.min.x) * 0.5f;
        float y_extent = (boxA.max.y - boxA.min.y) * 0.5f;

        closest.x = std::clamp(closest.x, -x_extent, x_extent);
        closest.y = std::clamp(closest.y, -y_extent, y_extent);

        bool inside = false;

        if (closest == dirVec)
        {
            inside = true;
            if (oom::abs(dirVec.x) > oom::abs(dirVec.y))
            {
                if (closest.x > 0) closest.x = x_extent;
                else closest.x = -x_extent;
            }
            else
            {
                if (closest.y > 0) closest.y = y_extent;
                else closest.y = -y_extent;
            }
        }

        oom::vec2 normal = dirVec - closest;
        float d = oom::length(normal);

        result.Normal = oom::normalize(normal);//oom::normalize(closest - circleB.center);//oom::normalize(boxCenter - closest);
        result.PenetrationDepth = circleB.radius - d;

        if (inside)
        {
            result.Normal *= -1;
        }

        result.HasCollision = true;

        return result;
    }

    Manifold2D PhysicsManifold::GenerateManifold2D(BoxCollider2D const& aabb, BoxCollider2D const& aabb2)
    {
        Manifold2D result{};

        auto boxA = aabb.GetGlobalBounds();
        auto boxB = aabb2.GetGlobalBounds();

        oom::vec2 n = (boxB.min + boxB.max) * 0.5f - (boxA.min + boxA.max) * 0.5f;

        float ax_extent = (boxA.max.x - boxA.min.x) * 0.5f;
        float bx_extent = (boxB.max.x - boxB.min.x) * 0.5f;

        float x_overlap = ax_extent + bx_extent - std::abs(n.x);

        //assumes overlap
        float ay_extent = (boxA.max.y - boxA.min.y) * 0.5f;
        float by_extent = (boxB.max.y - boxB.min.y) * 0.5f;

        float y_overlap = ay_extent + by_extent - std::abs(n.y);

        // find axis of least penetration
        if (x_overlap < y_overlap)
        {
            if (n.x < 0) result.Normal = { -1, 0 };
            else result.Normal = { 1, 0 };
            result.PenetrationDepth = x_overlap;
        }
        else
        {
            if (n.y < 0) result.Normal = { 0, -1 };
            else result.Normal = { 0, 1 };
            result.PenetrationDepth = y_overlap;
        }

        result.HasCollision = true;

        return result;
    }

}