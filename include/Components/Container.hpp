/**
 * @file Container.hpp
 * @brief Defines the Container component and its properties.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.2
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
            /**
             * @brief Layout properties specific to the Container component.
             */
            SFUI::PropGroup::Container::Layout layout;

            /**
             * @brief Style properties specific to the Container component.
             */
            SFUI::PropGroup::Container::Style style;

            /**
             * @brief State properties specific to the Container component.
             */
            SFUI::PropGroup::Container::State state;

            /**
             * @brief Behavior properties specific to the Container component.
             */
            SFUI::PropGroup::Container::Behavior behavior;

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
             * @param propSet Set of container property groups.
             * @param children Variadic list of child components.
             */
            template<typename... Children>
            Container(SFUI::String componentID, SFUI::PropSet::Container propSet, Children&&... children) :
                Component(componentID),
                layout(propSet.layout),
                style(propSet.style),
                state(propSet.state),
                behavior(propSet.behavior)
            {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);
            }

            /**
             * @brief Handle input events for the container.
             *
             * @param event Event to process.
             */
            SFUI::Void handleEvent(const SFUI::Event& event);

            /**
             * @brief Handle the pre updaate updates for the component.
             */
            SFUI::Void preUpdate();

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

            /**
             * @brief Draw the component or inner components on an overlay layer on top of the main UI tree to the render target.
             *
             * This is relevant for components that are actively animating and do not want their drawn geometry subject to
             * clipping by their parents' bounds. It is also useful for inner components like tooltips, context menus, modals,
             * and other special UI components. This meant to have a seperate second draw pass after the initial UI tree draw()
             * function calls to the components.
             *
             * @param drawTarget Target to draw on.
             * @param window Window reference.
             */
            SFUI::Void drawOverlay(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);
    };
}
