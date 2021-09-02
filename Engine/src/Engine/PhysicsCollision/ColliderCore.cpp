/************************************************************************************//*!
\file          ColliderCore.cpp
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
#include "ColliderCore.h"

#include "Colliders.h"
#include "Engine/ECS/GameObject.h"

#include <rttr/registration>

namespace engine
{
    RTTR_REGISTRATION
    {
        using namespace rttr;
        registration::class_<Collider2D>("Collider2D")
            .property("IsTrigger", &Collider2D::IsTrigger);
    }

    Collider2D::Collider2D(Entity entity, bool active)
        : Component{ entity, active }
        //, collider{ BoxCollider2D{ GetComponent<Transform3D>() } }
    {
    };

    void Collider2D::SetNarrowPhaseCollider(ColliderType narrowPhaseCollider)
    {
        if (m_narrowPhaseCollider == narrowPhaseCollider) return;

        /*switch (m_narrowPhaseCollider)
        {
        case ColliderType::BOX: RemoveComponent<BoxCollider2D>();
            break;
        case ColliderType::CIRCLE: RemoveComponent<CircleCollider2D>();
            break;
        };*/

        m_narrowPhaseCollider = narrowPhaseCollider;

        /*switch (m_narrowPhaseCollider)
        {
        case ColliderType::BOX: AddComponent<BoxCollider2D>();
            break;
        case ColliderType::CIRCLE: AddComponent<CircleCollider2D>();
            break;
        };*/
    }

    void Collider2D::Update()
    {
        if (IsTrigger)
        {
            if (!m_previous && m_current)
            {
                OnTriggerEnter(m_triggers);
            }
            else if (m_previous && m_current)
            {
                OnTriggerStay(m_triggers);
            }
            else if (m_previous && !m_current)
            {
                OnTriggerExit(m_triggers);
            }
            m_triggers.clear();
        }
        else
        {
            if (!m_previous && m_current)
            {
                OnCollisionEnter(m_collisions);
            }
            else if (m_previous && m_current)
            {
                OnCollisionStay(m_collisions);
            }
            else if (m_previous && !m_current)
            {
                OnCollisionExit(m_collisions);
            }
            m_collisions.clear();
        }
        m_previous = m_current;
        m_current = false;
    }

}
