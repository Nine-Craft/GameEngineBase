#include "pch.h"
#include "PhysicsUtils.h"

#include "Colliders.h"

#include "Manifold.h"
#include "Algorithms/PhysicsCollision.h"
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

        /*switch (first.GetNarrowPhaseCollider())
        {
        case ColliderType::CIRCLE:
        {
            auto const& circle = first.GetComponent<CircleCollider2D>().GetGlobalBounds();

            switch (second.GetNarrowPhaseCollider())
            {
            case ColliderType::CIRCLE:
                return Collision::Test2DCircleCircle(circle, second.GetComponent<CircleCollider2D>().GetGlobalBounds());


            case ColliderType::BOX:
                return Collision::Test2DCircleAABB(circle, second.GetComponent<BoxCollider2D>().GetGlobalBounds());
            }
        }

        case ColliderType::BOX:
        {
            auto const& box = first.GetComponent<BoxCollider2D>().GetGlobalBounds();
            switch (second.GetNarrowPhaseCollider())
            {
            case ColliderType::BOX:
                return Collision::Test2DAABBAABB(box, second.GetComponent<BoxCollider2D>().GetGlobalBounds());


            case ColliderType::CIRCLE:
                return Collision::Test2DCircleAABB(second.GetComponent<CircleCollider2D>().GetGlobalBounds(), box);

            }
        }
        }*/
    }

    Manifold2D PhysicsUtils::GenerateManifold2D(Collider2D const& first, Collider2D const& second)
    {
        return m_manifoldMap[std::make_pair(first.GetNarrowPhaseCollider(), second.GetNarrowPhaseCollider())](first, second);

        //switch (first.GetNarrowPhaseCollider())
        //{
        //case ColliderType::CIRCLE:
        //    switch (second.GetNarrowPhaseCollider())
        //    {
        //    case ColliderType::CIRCLE:
        //        return PhysicsCollision::Test2DCollision(first.GetComponent<CircleCollider2D>(), second.GetComponent<CircleCollider2D>());

        //    case ColliderType::BOX:
        //        return PhysicsCollision::Test2DCollision(first.GetComponent<CircleCollider2D>(), second.GetComponent<BoxCollider2D>());
        //    }
        //case ColliderType::BOX:
        //    switch (second.GetNarrowPhaseCollider())
        //    {
        //    case ColliderType::CIRCLE:
        //        return PhysicsCollision::Test2DCollision(first.GetComponent<BoxCollider2D>(), second.GetComponent<CircleCollider2D>());

        //    case ColliderType::BOX:
        //        return PhysicsCollision::Test2DCollision(first.GetComponent<BoxCollider2D>(), second.GetComponent<BoxCollider2D>());
        //    }
        //}
    }



    bool PhysicsUtils::TestCollision2D_BOX_BOX(Collider2D boxA, Collider2D boxB)
    {
        return Collision::Test2DAABBAABB(boxA.GetComponent<BoxCollider2D>().GetGlobalBounds(), boxB.GetComponent<BoxCollider2D>().GetGlobalBounds());
    }

    bool PhysicsUtils::TestCollision2D_CIRCLE_CIRCLE(Collider2D circleA, Collider2D circleB)
    {
        return Collision::Test2DCircleCircle(circleA.GetComponent<CircleCollider2D>().GetGlobalBounds(), circleB.GetComponent<CircleCollider2D>().GetGlobalBounds());
    }

    bool PhysicsUtils::TestCollision2D_CIRCLE_BOX(Collider2D circleA, Collider2D boxB)
    {
        return Collision::Test2DCircleAABB(circleA.GetComponent<CircleCollider2D>().GetGlobalBounds(), boxB.GetComponent<BoxCollider2D>().GetGlobalBounds());
    }

    bool PhysicsUtils::TestCollision2D_BOX_CIRCLE(Collider2D boxA, Collider2D circleB)
    {
        return Collision::Test2DCircleAABB(circleB.GetComponent<CircleCollider2D>().GetGlobalBounds(), boxA.GetComponent<BoxCollider2D>().GetGlobalBounds());
    }



    Manifold2D PhysicsUtils::GenerateManifold2D_BOX_BOX(Collider2D boxA, Collider2D boxB)
    {
        return PhysicsCollision::GenerateManifold2D(boxA.GetComponent<BoxCollider2D>(), boxB.GetComponent<BoxCollider2D>());
    }

    Manifold2D PhysicsUtils::GenerateManifold2D_CIRCLE_CIRCLE(Collider2D circleA, Collider2D circleB)
    {
        return PhysicsCollision::GenerateManifold2D(circleA.GetComponent<CircleCollider2D>(), circleB.GetComponent<CircleCollider2D>());
    }

    Manifold2D PhysicsUtils::GenerateManifold2D_CIRCLE_BOX(Collider2D circleA, Collider2D boxB)
    {
        return PhysicsCollision::GenerateManifold2D(circleA.GetComponent<CircleCollider2D>(), boxB.GetComponent<BoxCollider2D>());
    }

    Manifold2D PhysicsUtils::GenerateManifold2D_BOX_CIRCLE(Collider2D boxA, Collider2D circleB)
    {
        return PhysicsCollision::GenerateManifold2D(boxA.GetComponent<BoxCollider2D>(), circleB.GetComponent<CircleCollider2D>());
    }

}
