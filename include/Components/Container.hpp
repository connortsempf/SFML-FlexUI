/**
 * @file Container.hpp
 * @brief Defines the Container component and its properties.
 * @author Connor Sempf
 * @date 2025-11-15
 * @version 1.0
 *
 * This file contains the class definition, internal state,
 * and helper functions for the SFUI Container component.
 */


#pragma once
#include "Base/Component.hpp"


namespace SFUI {

    /**
     * @brief Container UI component that can hold and arrange child components.
     * 
     * The Container class extends the base Component class and provides
     * functionality for layout and rendering of its child components.
     */
    class Container : public Component {

        public:
            Container() = default;
            Container(Container&&) = default;
            Container& operator=(Container&&) = default;

            /**
             * @brief Construct a Container with an ID.
             * 
             * @param componentID Unique identifier for this container.
             */
            Container(SFUI::String componentID);
            
            /**
             * @brief Construct a Container with ID, properties, and children.
             * 
             * @param componentID Unique identifier.
             * @param containerPropGroup Group of container properties.
             * @param children Variadic list of child components.
             */
            template<typename... Children>
            Container(SFUI::String componentID, SFUI::PropGroup::Container containerPropGroup, Children&&... children) :
                Component(std::move(componentID), std::move(containerPropGroup.layout), std::move(containerPropGroup.style)) {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);

            }

            /**
             * @brief Handle input events for the container.
             * 
             * @param event Event to process.
             */
            SFUI::Void handleEvent(const SFUI::Event& event);
            
            /**
             * @brief Update the container and its children.
             * 
             * @param renderTargetSize Size of the render target.
             */
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            
            /**
             * @brief Draw the container and its children to the render target.
             * 
             * @param drawTarget Target to draw on.
             * @param window Window reference.
             */
            SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);
    };
}
