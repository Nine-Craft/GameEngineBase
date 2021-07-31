/************************************************************************************//*!
\file           Transform3D.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jul 22, 2021
\brief          Defines a 3D transform component. A Transform Component is used to
                identify a position in space and how much to rotate and scale the 
                object by.
                Refer to this as a template of how to implement a component.
                Implements RTTR as well.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/ECS/Component.h"
#include "Engine/ECS/ComponentMk2.h"
#include <glm/glm.hpp>

#include <rttr/type>

namespace engine 
{
    class Transform3D final : public ComponentMk2<>
    {
      public:
        using Component = ComponentMk2<>;
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        Transform3D()                               = delete;
        Transform3D(Transform3D const&)             = default;
        Transform3D(Transform3D &&)                 = default;
        Transform3D& operator=(Transform3D const&)  = default;
        Transform3D& operator=(Transform3D &&)      = default;
        virtual ~Transform3D() override             = default;

        /****************************************************************************//*!
         @brief     Overloaded constructor for transform component
         
         @param     entityID 
                The id that is attached to the entity.
         @param     active 
                Determines if the component is active or not.
        *//*****************************************************************************/
        Transform3D(Entity entityID, bool active = true);

        /*-----------------------------------------------------------------------------*/
        /* Getter Functions                                                            */
        /*-----------------------------------------------------------------------------*/
        float     const& GetRotationAngle()    const    { return m_rotationAngle; }
        glm::vec3 const& GetPosition()         const    { return m_position; }
        glm::vec3 const& GetRotationAxis()     const    { return m_rotationAxis; }
        glm::vec3 const& GetScale()            const    { return m_scale;    }
        glm::mat4 const& GetLocalMatrix()      const    { return m_localTransform;}
        glm::mat4 const& GetGlobalMatrix()     const    { return m_globalTransform; }
        int       const& GetChildCount()       const    { return m_childCount; }
        Entity    const& GetParentId()         const    { return m_parentId; }
        bool      const& IsDirty()             const    { return m_dirty; }
        bool      const& HasChanged()          const    { return m_hasChanged; }

        /*-----------------------------------------------------------------------------*/
        /* Setter Functions                                                            */
        /*-----------------------------------------------------------------------------*/
        float&      RotationAngle()                     { m_dirty = true; return m_rotationAngle; }
        glm::vec3&  Position()                          { m_dirty = true; return m_position; }
        glm::vec3&  RotationAxis()                      { m_dirty = true; return m_rotationAxis; }
        glm::vec3&  Scale()                             { m_dirty = true; return m_scale; }
        void        Reset()                             { m_hasChanged = false; }

        void SetPosition(glm::vec3 const& pos)          { m_dirty = true; m_position = pos; }
        void SetRotationAxis(glm::vec3 const& rotAxis)  { m_dirty = true; m_rotationAxis = rotAxis; }
        void SetRotationAngle(float const& angle)       { m_dirty = true; m_rotationAngle = angle; }
        void SetScale(glm::vec3 const& scale)           { m_dirty = true; m_scale = scale; }


        /****************************************************************************//*!
         @brief     Retrieves the global position of this Component from the global
                    transformation matrix.

         @warning   Take note that in most cases, unless being called after the Transform
                    Runtime has been executed, this will be the position from the previous
                    frame.

         @return    An glm::vec3 that represents the previous position of this Component in
                    global coordinates.
        *//*****************************************************************************/
        glm::vec3 GetGlobalPosition() const { return m_globalTransform[3] ; }

        /****************************************************************************//*!
         @brief     Retrieves the global rotation matrix of this object from the global
                    transformation matrix.
                    Link to explaination : https://tinyurl.com/deriveTRS

         @warning   Take note that in most cases, unless being called after the Transform
                    Runtime has been executed, this will be the rotation matrix 
                    from the previous frame.

         @return    An glm::mat4 that represents the rotation matrix from the previous frame
                    of this Component in global coordinates.
        *//*****************************************************************************/
        glm::mat4 GetGlobalRotationMatrix() const;

        /****************************************************************************//*!
         @brief    Retrieves the global rotation of this object in radians 
                   from the global transformation matrix.
                   link to explaination : https://tinyurl.com/DeriveTRSfrom2dMat

         @warning  Take note that in most cases, unless being called after the Transform
                   Runtime has been executed, this will be the rotation from the previous
                   frame.

         @return   A float in radians that represents the rotation angle of the previous 
                   frame of this Component in global coordinates.
        *//*****************************************************************************/
        float GetGlobalRotationRad() const;

        /****************************************************************************//*!
         @brief    Retrieves the global rotation of this object in degress
                   from the global transformation matrix.
                   link to explaination : https://tinyurl.com/DeriveTRSfrom2dMat

         @warning  Take note that in most cases, unless being called after the Transform
                   Runtime has been executed, this will be the rotation from the previous
                   frame.

         @return   A float in degrees that represents the rotation angle of the previous 
                   frame of this Component in global coordinates.
        *//*****************************************************************************/
        float GetGlobalRotationDeg() const;

        /****************************************************************************//*!
         @brief     Retrieves the global scale of this object from the global
                    transformation matrix.
                    Link to explaination : https://tinyurl.com/deriveTRS

         @warning   Take note that in most cases, unless being called after the Transform
                    Runtime has been executed, this will be the scale from the previous
                    frame.

         @return    An glm::vec3 that represents the previous scale of this GameObject in
                    global coordinates.
        *//*****************************************************************************/
        glm::vec3    GetGlobalScale()   const;


        /*-----------------------------------------------------------------------------*/
        /* Member Functions                                                            */
        /*-----------------------------------------------------------------------------*/

        /****************************************************************************//*!
         @brief     Recalculate the local transform matrix.
                    Should only be called by the transform runtime
        *//*****************************************************************************/
        void Recalculate();

        /****************************************************************************//*!
         @brief     Sets the Global transform matrix.
                    Should only be called by the transform runtime
        *//*****************************************************************************/
        void SetGlobalMatrix();

        /****************************************************************************//*!
         @brief     Tells the Transform to do coordinate conversion instead of 
                    position transformation when attaching to a parent that is not
                    the root gameobject.
                    Call this function when you want to attach a new parent to prevent
                    the object from being geometrically moved in space.

         @note      Conversion will automatically be set back to false once there is
                    the conversion matrix has been calculated.
        *//*****************************************************************************/
        void ConvertCoordinates() { m_dirty = true; m_conversion = true; }
        

        // Temporary way to get the id of the transform component : should delete
        // and find better fix.[TODO]
        Entity GetID() { return GetEntity(); }

        RTTR_ENABLE();
    
    private:
        
        friend class TransformSystem;
        /****************************************************************************//*!
         @brief     Assign this transform parent class

         @param     parent
                The new parent of this transform class
        *//*****************************************************************************/
        void SetParent(Transform3D& parent);

        /****************************************************************************//*!
         @brief     Helper function to recursively decrement child count of all
                    parent nodes.

         @param     childCount
                the number of children to increment the new parent's child count by.
        *//*****************************************************************************/
        void IncrementChildCount(int childCount);

        /*********************************************************************************//*!
         @brief     Helper function to recursively decrement child count of all
                    parent nodes.

         @param     childCount
                the number of children to decrement the new parent's child count by.
        *//**********************************************************************************/
        void DecrementChildCount(int childCount);

        int m_childCount;
        Entity m_parentId;

        glm::mat4 m_globalTransform;
        glm::mat4 m_localTransform;

        float m_rotationAngle;

        glm::vec3 m_position;
        glm::vec3 m_rotationAxis;
        glm::vec3 m_scale;

        glm::mat4 m_conversionMatrix;
        bool m_conversion; 

        bool m_dirty;
        bool m_hasChanged;
    };

} // namespace Engine