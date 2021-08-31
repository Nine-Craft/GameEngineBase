#pragma once

#include "Engine/ECS/Component.h"

#include "Shapes.h"

namespace engine
{
    //// forward declaration
    //class BoxCollider2D;
    //class CircleCollider2D;

    enum class ColliderType
    {
        CIRCLE,
        BOX
    };

    struct CollisionMap
    {
        static Manifold2D TestCollision2D(Collider2D const& first, Collider2D const& second);
    };

    class Collider2D : public Component
    {
    /*public:
        virtual Manifold2D TestCollision(Collider2D const* collider) const = 0;
        virtual Manifold2D TestCollision(BoxCollider2D const* boxCollider2D) const = 0;
        virtual Manifold2D TestCollision(CircleCollider2D const* circleCollider2D) const = 0;*/

        // third attempt : double components - function map collision
        ColliderType m_broadphaseCollider   = ColliderType::BOX;
        ColliderType m_narrowPhaseCollider  = ColliderType::CIRCLE;

    public:
        bool IsTrigger;
        vec2 Offset;

        explicit Collider2D(Entity entity, bool active = true);
        
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        Collider2D()                            = delete;
        Collider2D(Collider2D const&)           = default;
        Collider2D(Collider2D &&)               = default;
        Collider2D& operator=(Collider2D const&)= default;
        Collider2D& operator=(Collider2D &&)    = default;
        virtual ~Collider2D()                   = default;

        // third attempt : double components - function map collision
        ColliderType GetBroadPhaseCollider() const { return m_broadphaseCollider; }

        ColliderType GetNarrowPhaseCollider() const { return m_narrowPhaseCollider; }

        void SetNarrowPhaseCollider(ColliderType narrowPhaseCollider);

        /*Manifold2D TestCollision(Collider2D other) const
        {
            return CollisionMap::TestCollision(*this, other);
        }*/

        glm::vec2 WorldScale() const;
        glm::vec2 WorldPosition() const;

        // second attempt : using std::variant for collision
        //std::variant<BoxCollider2D, CircleCollider2D> collider;

        /* original first attempt: inheritence double dispatch
        virtual void OnTriggerEnter(Collider2D collider) {};
        virtual void OnTriggerStay(Collider2D collider) {};
        virtual void OnTriggerExit(Collider2D collider) {};*/

    };
}
