/**
 * @file Toggle.hpp
 * @brief Defines the Toggle component and its properties.
 * @author Connor Sempf
 * @date 2025-11-15
 * @version 1.0
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
             * @brief Style properties specific to the Toggle component.
             */
            SFUI::Prop::Style::Toggle toggleStyle;
            
            /**
             * @brief State properties specific to the Toggle component.
             */
            SFUI::Prop::State::Toggle toggleState;
            
            /**
             * @brief Behavior properties specific to the Toggle component.
             */
            SFUI::Prop::Behavior::Toggle toggleBehavior;

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
             * @param togglePropGroup Group of toggle properties.
             * @param children Variadic list of child components.
             */
            template<typename... Children>
            Toggle(SFUI::String componentID, SFUI::PropGroup::Toggle togglePropGroup, Children&&... children) :
                Component(std::move(componentID), std::move(togglePropGroup.layout), std::move(togglePropGroup.style)),
                toggleStyle(std::move(togglePropGroup.toggleStyle)),
                toggleState(std::move(togglePropGroup.toggleState)),
                toggleBehavior(std::move(togglePropGroup.toggleBehavior)),
                toggle(componentID + "_InnerToggle") {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);

            }

            /**
             * @brief Handle input events for the toggle.
             * 
             * @param event Event to process.
             */
            SFUI::Void handleEvent(const SFUI::Event& event);
            
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

        private:
            /**
             * @brief The main button representing the toggle switch.
             */
            SFUI::Button toggle;

            /**
             * @brief Cached style properties that have changed and require recomputation.
             */
            SFUI::Prop::Style::Toggle dirtyToggleStyle;

            /**
             * @brief Cached state properties that have changed and require recomputation.
             */
            SFUI::Prop::State::Toggle dirtyToggleState;

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