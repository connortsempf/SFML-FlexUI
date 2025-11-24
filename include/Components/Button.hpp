/**
 * @file Button.hpp
 * @brief Defines the Button component and its properties.
 * @author Connor Sempf
 * @date 2025-11-15
 * @version 1.0
 *
 * This file contains the class definition, internal state,
 * and helper functions for the SFUI Button component.
 */


#pragma once
#include "Base/Component.hpp"
#include "Container.hpp"
#include "Label.hpp"


namespace SFUI {
    
    /**
     * @brief Button UI component that can handle user interactions.
     * 
     * The Button class extends the base Component class and provides
     * functionality for handling user interactions such as clicks, hovers,
     * and tool-tips. It manages its own state and style properties.
     */
    class Button : public Component {
        
        public:
            /**
             * @brief Style properties specific to the Button component.
             */
            SFUI::Prop::Style::Button buttonStyle;
            
            /**
             * @brief State properties specific to the Button component.
             */
            SFUI::Prop::State::Button buttonState;
            
            /**
             * @brief Behavior properties specific to the Button component.
             */
            SFUI::Prop::Behavior::Button buttonBehavior;

        public:
            Button() = default;
            Button(Button&&) = default;
            Button& operator=(Button&&) = default;

            /**
             * @brief Construct a Button with an ID.
             * 
             * @param componentID Unique identifier for this button.
             */
            Button(SFUI::String componentID);
            
            /**
             * @brief Construct a Button with ID, properties, and children.
             * 
             * @param componentID Unique identifier.
             * @param buttonPropGroup Group of button properties.
             * @param children Variadic list of child components.
             */
            template<typename... Children>
            Button(SFUI::String componentID, SFUI::PropGroup::Button buttonPropGroup, Children&&... children) :
                Component(std::move(componentID), std::move(buttonPropGroup.layout), std::move(buttonPropGroup.style)),
                buttonStyle(std::move(buttonPropGroup.buttonStyle)),
                buttonState(std::move(buttonPropGroup.buttonState)),
                buttonBehavior(std::move(buttonPropGroup.buttonBehavior)),
                focus(componentID + "Focus"),
                toolTip(componentID + "_ToolTip") {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);

            }

            /**
             * @brief Handle input events for the button.
             * 
             * @param event Event to process.
             */
            SFUI::Void handleEvent(const SFUI::Event& event);
            
            /**
             * @brief Update button state.
             * 
             * @param renderTargetSize Size of the render target.
             */
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            
            /**
             * @brief Draw the button to the render target.
             * 
             * @param drawTarget Target to draw on.
             * @param window Window reference.
             */
            SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);
            
            /**
             * @brief Get the shared pointer to the tool-tip font.
             * 
             * @return Shared pointer to the tool-tip font.
             */
            SFUI::SharedPointer<SFUI::Font> getToolTipFont();
            
        private:
            /**
             * @brief Time thresholds for double-click and tool-tip display.
             */
            static const SFUI::Time DOUBLE_PRESS_GAP_MS;
            
            /**
             * @brief Mouse-Hovering Time threshold for tool-tip display.
             */
            static const SFUI::Time TOOL_TIP_THRESHOLD_MS;

        private:
            /**
             * @brief Flag for tracking the mouse hovering the button.
             */
            SFUI::Bool isHovered = false;

            /**
             * @brief Flag for tracking the mouse left-pressing the button.
             */
            SFUI::Bool isLeftPressed = false;

            /**
             * @brief Flag for tracking the mouse right-pressing the button.
             */
            SFUI::Bool isRightPressed = false;
            
            /**
             * @brief Flag for tracking the mouse middle-pressing the button.
             */
            SFUI::Bool isMiddlePressed = false;
            
            /**
             * @brief Flag for tracking if the tool-tip is currently shown.
             */
            SFUI::Bool isShowingToolTip = false;
            
            /**
             * @brief Previous mouse hover position and timing for event handling.
             */
            SFUI::Vector2f previousHoverPosition;

            /**
             * @brief Previous mouse press position and timing for event handling.
             */
            SFUI::Vector2i previousPressPosition;

            /**
             * @brief Clock for double-press detection.
             */
            SFUI::Clock doublePressClock;

            /**
             * @brief Time of the previous press for double-press detection.
             */
            SFUI::Time previousPressTime;
            
            /**
             * @brief Clock for tracking tool-tip display.
             */
            SFUI::Clock toolTipClock;

            /**
             * @brief Time when the mouse started hovering for tool-tip display.
             */
            SFUI::Time toolTipTime;

            /**
             * @brief Focus component.
             */
            SFUI::Container focus;

            /**
             * @brief Tool-tip label component.
             */
            SFUI::Label toolTip;

            /**
             * @brief Dirty style properties for tracking changes for recomputation.
             */
            SFUI::Prop::Style::Button dirtyButtonStyle;
            
            /**
             * @brief Dirty state properties for tracking changes for recomputation.
             */
            SFUI::Prop::State::Button dirtyButtonState;

        private:
            /**
             * @brief Compute dynamically changing color style properties.
             */
            SFUI::Void computeDynamicColors();
            
            /**
             * @brief Compute focus and component.
             */
            SFUI::Void computeFocus();
            
            /**
             * @brief Compute tool-tip component.
             */
            SFUI::Void computeToolTip();
    };
}
