#pragma once

#include "Engine/ECS/Component.h"
#include "Engine/Asset/AssetTypes.h"

#include <limits>

#include <glm/glm.hpp>

#include <rttr/type>

namespace engine
{

    /********************************************************************************//*!
     @brief    Defines a 2D transform component. A Transform Component is used to
               identify a position in space and how much to rotate and scale the
               object by.
               Refer to this as a template of how to implement a component.
    *//*********************************************************************************/
    class Sprite2D final : public Component
    {
    public:

      /*-----------------------------------------------------------------------------*/
      /* Constructors and Destructors                                                */
      /*-----------------------------------------------------------------------------*/
        Sprite2D() = delete;
        Sprite2D(Sprite2D const&) = default;
        Sprite2D(Sprite2D&&) = default;
        Sprite2D& operator=(Sprite2D const&) = default;
        Sprite2D& operator=(Sprite2D&&) = default;
        virtual ~Sprite2D() override = default;

        /****************************************************************************//*!
         @brief    Overloaded constructor for transform component

         @param[in]    _entityID
            The id that is attached to the entity.
         @param[in]    _active
            Determines if the component is active or not.
        *//*****************************************************************************/
        Sprite2D(Entity _entityID, bool _active = true);


        /*-----------------------------------------------------------------------------*/
        /* Getter Functions                                                            */
        /*-----------------------------------------------------------------------------*/
        ooTexID GetTexture() { return m_texture; }
        glm::vec4 GetColor() { return m_color; }

        /*-----------------------------------------------------------------------------*/
        /* Setter Functions                                                            */
        /*-----------------------------------------------------------------------------*/
        void SetTexture(ooTexID textureID);
        void SetTexture(Texture tex); 
        void SetColor(glm::vec4 col) { m_color = col; }
        void SetColor(float r, float g, float b, float alpha = 1.0f) { m_color.r = r; m_color.g = g; m_color.b = b; m_color.a = alpha;}

        //const bool   IsDirty()           const { return m_dirty; }
        //const bool   HasChanged()        const { return m_hasChanged; }



        /*-----------------------------------------------------------------------------*/
        /* Member Functions                                                            */
        /*-----------------------------------------------------------------------------*/



        //RTTR_ENABLE();

    private:

        uint32_t m_texture{ std::numeric_limits<uint32_t>::max() };

        glm::vec4 m_color{1.0f,1.0f,1.0f,1.0f};

    };

} // namespace Engine


