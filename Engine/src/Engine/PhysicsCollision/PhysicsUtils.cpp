/************************************************************************************//*!
\file          PhysicsUtils.cpp
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
#include "PhysicsUtils.h"

#include "Colliders.h"

#include "Manifold.h"
#include "Algorithms/PhysicsManifold.h"
#include "Algorithms/Collision.h"

#include "Engine/ECS/WorldManager.h"
#include "Engine/ECS/GameObject.h"

namespace engine
{
    std::map<PhysicsUtils::CombinationKey, PhysicsUtils::StaticTriggerCollisionFnc> PhysicsUtils::m_collisionMap
    {
        { {ColliderType::CIRCLE, ColliderType::CIRCLE}, TestCollision2D_CIRCLE_CIRCLE},
        { {ColliderType::CIRCLE, ColliderType::BOX}, TestCollision2D_CIRCLE_BOX},
        { {ColliderType::BOX, ColliderType::CIRCLE}, TestCollision2D_BOX_CIRCLE},
        { {ColliderType::BOX, ColliderType::BOX}, TestCollision2D_BOX_BOX}
    };

    std::map<PhysicsUtils::CombinationKey, PhysicsUtils::GenerateManifoldFnc> PhysicsUtils::m_manifoldMap
    {
        { {ColliderType::CIRCLE, ColliderType::CIRCLE}, GenerateManifold2D_CIRCLE_CIRCLE},
        { {ColliderType::CIRCLE, ColliderType::BOX}, GenerateManifold2D_CIRCLE_BOX},
        { {ColliderType::BOX, ColliderType::CIRCLE}, GenerateManifold2D_BOX_CIRCLE},
        { {ColliderType::BOX, ColliderType::BOX}, GenerateManifold2D_BOX_BOX}
    };


    bool PhysicsUtils::TestCollision2D(Collider2D const& first, Collider2D const& second)
    {
        return m_collisionMap[std::make_pair(first.GetNarrowPhaseCollider(), second.GetNarrowPhaseCollider())](first, second);
    }

    Manifold2D PhysicsUtils::GenerateManifold2D(Collider2D const& first, Collider2D const& second)
    {
        return m_manifoldMap[std::make_pair(first.GetNarrowPhaseCollider(), second.GetNarrowPhaseCollider())](first, second);
    }




    bool PhysicsUtils::TestCollision2D_BOX_BOX(Collider2D boxA, Collider2D boxB)
    {
        return Collision::Test2DAABBAABB(
            ColliderUtil::GetGlobalBounds(boxA.GetComponent<BoxCollider2D>(), boxA.GetComponent<Transform3D>())
            , ColliderUtil::GetGlobalBounds(boxB.GetComponent<BoxCollider2D>(), boxB.GetComponent<Transform3D>())
        );
    }

    bool PhysicsUtils::TestCollision2D_CIRCLE_CIRCLE(Collider2D circleA, Collider2D circleB)
    {
        return Collision::Test2DCircleCircle(
            ColliderUtil::GetGlobalBounds(circleA.GetComponent<CircleCollider2D>(), circleA.GetComponent<Transform3D>())
            , ColliderUtil::GetGlobalBounds(circleB.GetComponent<CircleCollider2D>(), circleB.GetComponent<Transform3D>()));
    }

    bool PhysicsUtils::TestCollision2D_CIRCLE_BOX(Collider2D circleA, Collider2D boxB)
    {

        return Collision::Test2DCircleAABB(
            ColliderUtil::GetGlobalBounds(circleA.GetComponent<CircleCollider2D>(), circleA.GetComponent<Transform3D>())
            , ColliderUtil::GetGlobalBounds(boxB.GetComponent<BoxCollider2D>(), boxB.GetComponent<Transform3D>()));
    }

    bool PhysicsUtils::TestCollision2D_BOX_CIRCLE(Collider2D boxA, Collider2D circleB)
    {
        return Collision::Test2DCircleAABB(
            ColliderUtil::GetGlobalBounds(circleB.GetComponent<CircleCollider2D>(), circleB.GetComponent<Transform3D>())
            , ColliderUtil::GetGlobalBounds(boxA.GetComponent<BoxCollider2D>(), boxA.GetComponent<Transform3D>()));
    }



    Manifold2D PhysicsUtils::GenerateManifold2D_BOX_BOX(Collider2D boxA, Collider2D boxB)
    {
        return PhysicsManifold::GenerateManifold2D(boxA.GetComponent<BoxCollider2D>(), boxB.GetComponent<BoxCollider2D>());
    }

    Manifold2D PhysicsUtils::GenerateManifold2D_CIRCLE_CIRCLE(Collider2D circleA, Collider2D circleB)
    {
        return PhysicsManifold::GenerateManifold2D(circleA.GetComponent<CircleCollider2D>(), circleB.GetComponent<CircleCollider2D>());
    }

    Manifold2D PhysicsUtils::GenerateManifold2D_CIRCLE_BOX(Collider2D circleA, Collider2D boxB)
    {
        return PhysicsManifold::GenerateManifold2D(circleA.GetComponent<CircleCollider2D>(), boxB.GetComponent<BoxCollider2D>());
    }

    Manifold2D PhysicsUtils::GenerateManifold2D_BOX_CIRCLE(Collider2D boxA, Collider2D circleB)
    {
        return PhysicsManifold::GenerateManifold2D(boxA.GetComponent<BoxCollider2D>(), circleB.GetComponent<CircleCollider2D>());
    }

}
