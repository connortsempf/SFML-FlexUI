/**
 * @file Label.hpp
 * @brief Defines the Label component and its properties.
 * @author Connor Sempf
 * @date 2025-11-15
 * @version 1.0
 *
 * This file contains the class definition, internal state,
 * and helper functions for the SFUI Label component.
 */


#pragma once
#include "Base/Component.hpp"


namespace SFUI {

    /**
     * @brief Label UI component that displays text.
     * 
     * The Label class extends the base Component class and provides
     * functionality for displaying and managing text labels.
     */
    class Label : public Component {

        public:
            /**
             * @brief Style properties specific to the Label component.
             */
            SFUI::Prop::Style::Label labelStyle;

        public:
            Label() = default;
            Label(Label&&) = default;
            Label& operator=(Label&&) = default;
            
            /**
             * @brief Construct a Label with an ID.
             * 
             * @param componentID Unique identifier for this label.
             */
            Label(SFUI::String componentID);
            
            /**
             * @brief Construct a Label with ID, properties, and children.
             * 
             * @param componentID Unique identifier.
             * @param labelPropGroup Group of label properties.
             * @param children Variadic list of child components.
             */
            template<typename... Children>
            Label(SFUI::String componentID, SFUI::PropGroup::Label labelPropGroup, Children&&... children) :
                Component(std::move(componentID), std::move(labelPropGroup.layout), std::move(labelPropGroup.style)),
                labelStyle(std::move(labelPropGroup.labelStyle)),
                textObject(*(labelStyle.font), labelStyle.text, labelStyle.textSize) {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);

            }

            /**
             * @brief Handle input events for the label.
             * 
             * @param event Event to process.
             */
            SFUI::Void handleEvent(const SFUI::Event& event);
            
            /**
             * @brief Update the label and its text layout.
             * 
             * @param renderTargetSize Size of the render target.
             */
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            
            /**
             * @brief Draw the label and its text to the render target.
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
            
            /**
             * @brief Get text properties and metrics.
             * 
             * @return Font used by the label.
             */
            SFUI::SharedPointer<SFUI::Font> getFont();
            
            /**
             * @brief Get the size of the text.
             */
            SFUI::Float getTextSize();

            /**
             * @brief Get the bounds of the text.
             */
            SFUI::FloatRect getTextBounds();
            
            /**
             * @brief Get the position of a specific character in the text.
             * 
             * @param charIndex Index of the character.
             * @return Position of the character.
             */
            SFUI::Vector2f getCharacterPosition(SFUI::Size charIndex);

        private:
            /**
             * @brief Offset factor for vertically-centered text alignment.
             */
            static const SFUI::Float VERTICAL_CENTER_OFFSET_FACTOR;
            
            /**
             * @brief Offset factors for vertical-bottom text alignment.
             */
            static const SFUI::Float BOTTOM_OFFSET_FACTOR;

        private:
            /**
             * @brief SFML Text object for rendering the label's text.
             */
            SFUI::Text textObject;
            
            /**
             * @brief Cached dirty style properties for change detection.
             */
            SFUI::Prop::Style::Label dirtyLabelStyle;

        private:

            /**
             * @brief Compute all necessary properties for the label's text.
             */
            SFUI::Void computeTextCore();
            
            /**
             * @brief Compute style-related properties for the label's text.
             */
            SFUI::Void computeTextStyles();
            
            /**
             * @brief Compute layout and geometry for the label's text.
             */
            SFUI::Void computeTextLayout();
    };
}
