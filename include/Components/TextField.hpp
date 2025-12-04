/**
 * @file TextField.hpp
 * @brief Defines the TextField component and its properties.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.0
 *
 * This file contains the class definition, internal state,
 * and helper functions for the SFUI TextField component.
 */


#pragma once
#include "Base/Component.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "Container.hpp"


namespace SFUI {

    /**
     * @brief TextField UI component that allows user text input.
     *
     * The TextField class extends the base Component class and provides
     * functionality for user text input, caret management, and text editing.
     */
    class TextField : public Component {

        public:
            /**
             * @brief Layout properties specific to the TextField component.
             */
            SFUI::PropGroup::TextField::Layout layout;

            /**
             * @brief Style properties specific to the TextField component.
             */
            SFUI::PropGroup::TextField::Style style;

            /**
             * @brief State properties specific to the TextField component.
             */
            SFUI::PropGroup::TextField::State state;

            /**
             * @brief Behavior properties specific to the TextField component.
             */
            SFUI::PropGroup::TextField::Behavior behavior;

        public:
            TextField() = default;
            TextField(TextField&&) = default;
            TextField& operator=(TextField&&) = default;

            /**
             * @brief Construct a TextField with an ID.
             *
             * @param componentID Unique identifier for this text field.
             */
            TextField(SFUI::String componentID);

            /**
             * @brief Construct a TextField with ID, properties, and children.
             *
             * @param componentID Unique identifier.
             * @param propSet Set of container property groups.
             * @param children Variadic list of child components.
             */
            template<typename... Children>
            TextField(SFUI::String componentID, SFUI::PropSet::TextField propSet, Children&&... children) :
                Component(componentID),
                layout(propSet.layout),
                style(propSet.style),
                state(propSet.state),
                behavior(propSet.behavior),
                // style(std::move(propSet.style)),
                // state(std::move(propSet.state)),
                // behavior(std::move(propSet.behavior)),
                background(componentID + "_Background"),
                inputText(componentID + "_InputText"),
                caret(componentID + "_Caret")
            {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);
            }

            /**
             * @brief Handle input events for the text field.
             *
             * @param event Event to process.
             */
            SFUI::Void handleEvent(const SFUI::Event& event);

            /**
             * @brief Handle the pre updaate updates for the component.
             */
            SFUI::Void preUpdate();

            /**
             * @brief Update the text field, caret blinking, and text layout.
             *
             * @param renderTargetSize Size of the render target.
             */
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);

            /**
             * @brief Draw the text field to the render target.
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
             * @brief Factor controlling the vertical offset when centering text.
             */
            static const SFUI::Float CENTER_TEXT_OFFSET_FACTOR;

            /**
             * @brief Factor controlling the vertical offset for the caret in line mode.
             */
            static const SFUI::Float CARET_LINE_VERTICAL_OFFSET_FACTOR;

            /**
             * @brief Factor controlling the vertical offset for the caret in box mode.
             */
            static const SFUI::Float CARET_BOX_VERTICAL_OFFSET_FACTOR;

            /**
             * @brief Factor controlling the vertical offset for the caret in underline mode.
             */
            static const SFUI::Float CARET_UNDERLINE_VERTICAL_OFFSET_FACTOR;

            /**
             * @brief Factor controlling the caret width in line mode.
             */
            static const SFUI::Float CARET_LINE_WIDTH_FACTOR;

            /**
             * @brief Factor controlling the caret width in box mode.
             */
            static const SFUI::Float CARET_BOX_WIDTH_FACTOR;

            /**
             * @brief Factor controlling the caret width in underline mode.
             */
            static const SFUI::Float CARET_UNDERLINE_WIDTH_FACTOR;

            /**
             * @brief Factor controlling the height of the underline caret.
             */
            static const SFUI::Float CARET_UNDERLINE_HEIGHT_FACTOR;

            /**
             * @brief Character group recognized as whitespace for CTRL-mod actions.
             */
            static const SFUI::String CTRL_WHITESPACE_GROUP;

            /**
             * @brief Character group recognized as alphanumeric for CTRL-mod actions.
             */
            static const SFUI::String CTRL_ALPHANUMERIC_GROUP;

            /**
             * @brief Character group recognized as symbols for CTRL-mod actions.
             */
            static const SFUI::String CTRL_SYMBOL_GROUP;

        private:
            /**
             * @brief Whether the caret is currently visible (blinking state).
             */
            SFUI::Bool caretVisible = false;

            /**
             * @brief Index in the text string where the caret is positioned.
             */
            SFUI::UnsignedInt caretIndex = 0;

            /**
             * @brief Dynamic pixel offset used to scroll text horizontally or vertically.
             */
            SFUI::Vector2f dynamicTextOffset = {0.0f, 0.0f};

            /**
             * @brief Clock used to control caret blink timing.
             */
            SFUI::Clock caretClock;

            /**
             * @brief Background visual element of the text field.
             */
            SFUI::Button background;

            /**
             * @brief Label displaying the user-entered text.
             */
            SFUI::Label inputText;

            /**
             * @brief Graphical caret element (line/box/underline).
             */
            SFUI::Container caret;

            /**
             * @brief Cached and fully computed text-field style.
             */
            SFUI::ComputedProp::Style::TextField computedTextFieldStyle;

        private:
            /**
             * @brief Compute the line mode (single/multi) and adjust related properties.
             */
            SFUI::Void computeLineMode();

            /**
             * @brief Compute the background appearance of the text field.
             */
            SFUI::Void computeBackground();

            /**
             * @brief Compute the input text layout and positioning.
             */
            SFUI::Void computeInputText();

            /**
             * @brief Compute the caret shape, size, and position.
             */
            SFUI::Void computeCaret();

            /**
             * @brief Compute the dynamic text offset for scrolling behavior.
             */
            SFUI::Void computeDynamicTextOffset();

            /**
             * @brief Update the caret visibility based on blink timing.
             */
            SFUI::Void insertText(const char32_t newAppendedText);

            /**
             * @brief Remove text at the current caret position.
             */
            SFUI::Void editText(const SFUI::Event::KeyPressed* keyPressedEvent);

            /**
             * @brief Get the character group for a given character.
             *
             * @param character Character to evaluate.
             *
             * @return Character group identifier. 0 for whitespace, 1 for alphanumeric, 2 for symbols.
             */
            SFUI::UnsignedInt getCharacterGroup(const char32_t character);
    };
}
