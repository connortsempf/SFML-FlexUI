/**
 * @file Button.hpp
 * @brief Defines the Button component and its properties.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.1
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
             * @brief Layout properties specific to the Button component.
             */
            SFUI::PropGroup::Button::Layout layout;
            /**
             * @brief Style properties specific to the Button component.
             */
            SFUI::PropGroup::Button::Style style;

            /**
             * @brief State properties specific to the Button component.
             */
            SFUI::PropGroup::Button::State state;

            /**
             * @brief Behavior properties specific to the Button component.
             */
            SFUI::PropGroup::Button::Behavior behavior;

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
             * @param propSet Set of button property groups.
             * @param children Variadic list of child components.
             */
            template<typename... Children>
            Button(SFUI::String componentID, SFUI::PropSet::Button propSet, Children&&... children) :
                Component(componentID),
                layout(std::move(propSet.layout)),
                style(std::move(propSet.style)),
                state(std::move(propSet.state)),
                behavior(std::move(propSet.behavior)),
                focus(componentID + "Focus"),
                toolTip(componentID + "_ToolTip")
            {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);
            }

            /**
             * @brief Handle input events for the button.
             *
             * @param event Event to process.
             */
            SFUI::Void handleEvent(const SFUI::Event& event);

            /**
             * @brief Handle the pre updaate updates for the component.
             */
            SFUI::Void preUpdate();

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
