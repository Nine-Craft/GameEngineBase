/************************************************************************************//*!
\file           LayerStack.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 24, 2021
\brief          Layerstack holds a vector of layers and provides the interface to 
                interact and control the individual layers. 
                Read layers.h for more explaination on what a layer is.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Core/Layer.h"

#include <vector>

namespace engine
{
    /********************************************************************************//*!
     @brief     Layerstack holds a vector of layers and provides the interface to 
                interact and control the individual layers. 
                Read layers.h for more explaination on what a layer is.
    *//*********************************************************************************/
    class LayerStack final
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Type aliases                                                                */
        /*-----------------------------------------------------------------------------*/
        using value_type                = Layer*;
        using container_type            = std::vector<value_type>;
        using iterator                  = container_type::iterator;
        using reverse_iterator          = container_type::reverse_iterator;
        using const_iterator            = container_type::const_iterator;
        using const_reverse_iterator    = container_type::const_reverse_iterator;

        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        LayerStack() = default;
        ~LayerStack();

        /*-----------------------------------------------------------------------------*/
        /* Functions                                                                   */
        /*-----------------------------------------------------------------------------*/
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        /*-----------------------------------------------------------------------------*/
        /* Iterator definitions                                                        */
        /*-----------------------------------------------------------------------------*/
        iterator begin() { return m_layers.begin(); }
        iterator end() { return m_layers.end(); }
        reverse_iterator rbegin() { return m_layers.rbegin(); }
        reverse_iterator rend() { return m_layers.rend(); }

        const_iterator begin() const { return m_layers.cbegin(); }
        const_iterator end() const { return m_layers.cend(); }
        const_reverse_iterator rbegin() const { return m_layers.crbegin(); }
        const_reverse_iterator crend() const { return m_layers.crend(); }

    private:
        std::vector<Layer*> m_layers;
        unsigned int m_layerInsertIndex = 0;
    };
}