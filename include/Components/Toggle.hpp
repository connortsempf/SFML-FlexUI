/**
 * @file Toggle.hpp
 * @brief Defines the Toggle component and its properties.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.2
 *
 * This file contains the class definition, internal state,
 * and helper functions for the SFUI Toggle component.
 */


#pragma once
#include "Base/Component.hpp"
#include "Button.hpp"


namespace SFUI {

    /**
     * @brief Toggle UI component that can switch between on and off states.
     *
     * The Toggle class extends the base Component class and provides
     * functionality for toggling between two states, managing its own
     * state and style properties.
     */
    class Toggle : public Component {

        public:
            /**
             * @brief Layout properties specific to the Toggle component.
             */
            SFUI::PropGroup::Toggle::Layout layout;

            /**
             * @brief Style properties specific to the Toggle component.
             */
            SFUI::PropGroup::Toggle::Style style;

            /**
             * @brief State properties specific to the Toggle component.
             */
            SFUI::PropGroup::Toggle::State state;

            /**
             * @brief Behavior properties specific to the Toggle component.
             */
            SFUI::PropGroup::Toggle::Behavior behavior;

        public:
            Toggle() = default;
            Toggle(Toggle&&) = default;
            Toggle& operator=(Toggle&&) = default;

            /**
             * @brief Construct a Toggle with an ID.
             *
             * @param componentID Unique identifier for this toggle.
             */
            Toggle(SFUI::String componentID);

            /**
             * @brief Construct a Toggle with ID, properties, and children.
             *
             * @param componentID Unique identifier.
             * @param propSet Set of toggle property groups
             * @param children Variadic list of child components.
             */
            template<typename... Children>
            Toggle(SFUI::String componentID, SFUI::PropSet::Toggle propSet, Children&&... children) :
                Component(componentID),
                layout(propSet.layout),
                style(propSet.style),
                state(propSet.state),
                behavior(propSet.behavior),
                // style(std::move(propSet.style)),
                // state(std::move(propSet.state)),
                // behavior(std::move(propSet.behavior)),
                toggle(componentID + "_InnerToggle")
            {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);
            }

            /**
             * @brief Handle input events for the toggle.
             *
             * @param event Event to process.
             */
            SFUI::Void handleEvent(const SFUI::Event& event);

            /**
             * @brief Handle the pre updaate updates for the component.
             */
            SFUI::Void preUpdate();

            /**
             * @brief Update toggle state.
             *
             * @param renderTargetSize Size of the render target.
             */
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);

            /**
             * @brief Draw the toggle to the render target.
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

        private:
            /**
             * @brief The main button representing the toggle switch.
             */
            SFUI::Button toggle;

        private:
            /**
             * @brief Compute dynamic colors based on state and style.
             */
            SFUI::Void computeDynamicColors();

            /**
             * @brief Compute the toggle's visual representation.
             */
            SFUI::Void computeToggle();
    };
}
