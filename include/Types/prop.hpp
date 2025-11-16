/**
 * @file prop.hpp
 * @brief Defines the main component property structures for layout and style in SFUI.
 * @author Connor Sempf
 * @date 2025-11-15
 * @version 1.0
 *
 * This file contains layout and style property definitions for
 * all SFUI components, including optional and variant types
 * for flexible property assignment.
 */


#pragma once
#include "base.hpp"
#include "subprop.hpp"


namespace SFUI {

    /**
     * @brief Component property definitions.
     *
     * This namespace contains all the property types used for
     * configuring components in SFML-FlexUI, including layout,
     * style, state, and behavior properties.
     */
    namespace Prop {

        /**
         * @brief Layout-related properties for components.
         *
         * This namespace contains structs and types that define
         * how components are positioned and aligned within their
         * parent container, including size, padding, margin, and
         * alignment settings.
         */
        namespace Layout {

            /**
             * @brief Layout properties for every component.
             */
            struct Component {
                /**
                 * @brief Primary alignment direction within the parent container for its child components.
                 */
                SFUI::SubProp::Keyword alignDirection = "vertical";

                /**
                 * @brief On-Axis alignment of children within the parent container.
                 */
                SFUI::SubProp::Keyword alignPrimary = "start";
                
                /**
                 * @brief Cross-Axis alignment of children within the parent container.
                 */
                SFUI::SubProp::Keyword alignSecondary = "start";
                
                /**
                 * @brief Width of the component.
                 */
                SFUI::SubProp::Dimension width = 0.0f;
                
                /**
                 * @brief Height of the component.
                 */
                SFUI::SubProp::Dimension height = 0.0f;
                
                /**
                 * @brief Padding inside the component.
                 */
                SFUI::SubProp::UniQuad padding = 0.0f;
                
                /**
                 * @brief Margin outside the component.
                 */
                SFUI::SubProp::UniQuad margin = 0.0f;
                
                /**
                 * @brief X position offset of the component.
                 */
                SFUI::Optional<SFUI::SubProp::Numeric> xPosition;
                
                /**
                 * @brief Y position offset of the component.
                 */
                SFUI::Optional<SFUI::SubProp::Numeric> yPosition;

                /**
                 * @brief Equality operator for Component layout properties.
                 */
                bool operator==(const Component&) const = default;
                
                /**
                 * @brief Inequality operator for Component layout properties.
                 */
                bool operator!=(const Component&) const = default;
            };
        }

        /**
         * @brief Style-related properties for components.
         *
         * This namespace contains structs and types that define
         * the visual appearance of components, including colors,
         * borders, shadows, and other stylistic attributes.
         */
        namespace Style {

            /**
             * @brief Style properties for every component.
             */
            struct Component {
                /**
                 * @brief Width of the component's border.
                 */
                SFUI::SubProp::Dimension borderWidth = 0.0f;
                
                /**
                 * @brief Corner radius of the component.
                 */
                SFUI::SubProp::UniQuad cornerRadius;
                
                /**
                 * @brief Fill color of the component.
                 */
                SFUI::SubProp::Color fillColor = SFUI::Color(0, 0, 0, 0);
                
                /**
                 * @brief Border color of the component.
                 */
                SFUI::SubProp::Color borderColor = SFUI::Color(0, 0, 0, 0);
                
                /**
                 * @brief Shadow offset of the component.
                 */
                SFUI::SubProp::Duplex shadowOffset = {0.0f, 0.0f};
                
                /**
                 * @brief Shadow radius of the component.
                 */
                SFUI::SubProp::Numeric shadowRadius = 1.0f;
                
                /**
                 * @brief Shadow fill color of the component.
                 */
                SFUI::SubProp::Color shadowFillColor = SFUI::Color(0, 0, 0, 0);

                /**
                 * @brief Equality operator for Component style properties.
                 */
                bool operator==(const Component&) const = default;
                
                /**
                 * @brief Inequality operator for Component style properties.
                 */
                bool operator!=(const Component&) const = default;
            };

            /**
             * @brief Style properties specific to Label components.
             */
            struct ScrollContainer {
                /**
                 * @brief Scroll direction of the container.
                 */
                SFUI::SubProp::Keyword scrollDirection = "vertical";
                
                /**
                 * @brief Scroll speed factor of the container.
                 */
                SFUI::SubProp::Numeric scrollSpeedFactor = 15.0f;

                /**
                 * @brief Equality operator for ScrollContainer style properties.
                 */
                bool operator==(const ScrollContainer&) const = default;
                
                /**
                 * @brief Inequality operator for ScrollContainer style properties.
                 */
                bool operator!=(const ScrollContainer&) const = default;
            };
            
            /**
             * @brief Style properties specific to Label components.
             */
            struct Label {
                /**
                 * @brief Text content of the label.
                 */
                SFUI::SubProp::Keyword text;
                
                /**
                 * @brief Font used for the label text.
                 */
                SFUI::SubProp::Font font = std::make_shared<SFUI::Font>();
                
                /**
                 * @brief Size of the label text.
                 */
                SFUI::SubProp::Numeric textSize = 12.0f;
                
                /**
                 * @brief Style of the label text.
                 */
                SFUI::SubProp::TextStyle textStyle = "regular";
                
                /**
                 * @brief Optional letter spacing for the label text.
                 */
                SFUI::Optional<SFUI::SubProp::Numeric> letterSpacing;
                
                /**
                 * @brief Optional line spacing for the label text.
                 */
                SFUI::Optional<SFUI::SubProp::Numeric> lineSpacing;
                
                /**
                 * @brief Thickness of the text outline.
                 */
                SFUI::SubProp::Numeric textOutlineThickness = 0.0f;
                
                /**
                 * @brief Horizontal alignment of the label text.
                 */
                SFUI::SubProp::Keyword textAlignHorizontal = "left";
                
                /**
                 * @brief Vertical alignment of the label text.
                 */
                SFUI::SubProp::Keyword textAlignVertical = "center";
                
                /**
                 * @brief Offset of the label text.
                 */
                SFUI::SubProp::Duplex textOffset = {0.0f, 0.0f};
                
                /**
                 * @brief Color of the label text.
                 */
                SFUI::SubProp::Color textColor = SFUI::Color(0, 0, 0, 255);
                
                /**
                 * @brief Color of the label text outline.
                 */
                SFUI::SubProp::Color textOutlineColor = SFUI::Color(255, 255, 255, 255);

                /**
                 * @brief Equality operator for Label style properties.
                 */
                bool operator==(const Label&) const = default;
                
                /**
                 * @brief Inequality operator for Label style properties.
                 */
                bool operator!=(const Label&) const = default;
            };
    
            /**
             * @brief Style properties specific to Button components.
             */
            struct Button {
                /**
                 * @brief Fill color when the button is in its normal state.
                 */
                SFUI::Optional<SFUI::SubProp::Color> hoveredFillColor;
                
                /**
                 * @brief Border color when the button is in its normal state.
                 */
                SFUI::Optional<SFUI::SubProp::Color> hoveredBorderColor;
                
                /**
                 * @brief Fill color when the button is pressed.
                 */
                SFUI::Optional<SFUI::SubProp::Color> pressedFillColor;
                
                /**
                 * @brief Border color when the button is pressed.
                 */
                SFUI::Optional<SFUI::SubProp::Color> pressedBorderColor;
                
                /**
                 * @brief Fill color when the button is disabled.
                 */
                SFUI::Optional<SFUI::SubProp::Color> disabledFillColor;
                
                /**
                 * @brief Border color when the button is disabled.
                 */
                SFUI::Optional<SFUI::SubProp::Color> disabledBorderColor;
                
                /**
                 * @brief Width of the focus outline.
                 */
                SFUI::SubProp::Dimension focusWidth = 10.0f;
                
                /**
                 * @brief Offset of the focus outline away from the edge of the component geometry.
                 */
                SFUI::SubProp::Dimension focusOffset = 0.0f;
                
                /**
                 * @brief Corner radius of the focus outline.
                 */
                SFUI::SubProp::UniQuad focusCornerRadius;
                
                /**
                 * @brief Fill color of the focus outline.
                 */
                SFUI::SubProp::Color focusFillColor = SFUI::Color(0, 0, 0, 255);
                
                /**
                 * @brief Padding for the tooltip.
                 */
                SFUI::SubProp::UniQuad toolTipPadding = 10.0f;
                
                /**
                 * @brief Corner radius for the tooltip.
                 */
                SFUI::SubProp::UniQuad toolTipCornerRadius;
                
                /**
                 * @brief Text content of the tooltip.
                 */
                SFUI::SubProp::Keyword toolTipText = "";
                
                /**
                 * @brief Font used for the tooltip text.
                 */
                SFUI::SubProp::Font toolTipFont = std::make_shared<SFUI::Font>();
                
                /**
                 * @brief Size of the tooltip text.
                 */
                SFUI::SubProp::Numeric toolTipTextSize = 10.0f;
                
                /**
                 * @brief Fill color of the tooltip.
                 */
                SFUI::SubProp::Color toolTipFillColor = SFUI::Color(150, 150, 150, 255);
                
                /**
                 * @brief Text color of the tooltip.
                 */
                SFUI::SubProp::Color toolTipTextColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Equality operator for Button style properties.
                 */
                bool operator==(const Button&) const = default;
                
                /**
                 * @brief Inequality operator for Button style properties.
                 */
                bool operator!=(const Button&) const = default;
            };

            /**
             * @brief Style properties specific to Toggle components.
             */
            struct Toggle {
                /**
                 * @brief Fill color when the toggle is off.
                 */
                SFUI::SubProp::Color offFillColor = SFUI::Color(0, 0, 0, 255);
                
                /**
                 * @brief Border color when the toggle is off.
                 */
                SFUI::SubProp::Color offBorderColor = SFUI::Color(0, 0, 0, 255);
                
                /**
                 * @brief Fill color when the toggle is on.
                 */
                SFUI::SubProp::Color onFillColor = SFUI::Color(0, 0, 0, 255);
                
                /**
                 * @brief Border color when the toggle is on.
                 */
                SFUI::SubProp::Color onBorderColor = SFUI::Color(0, 0, 0, 255);
                
                /**
                 * @brief Fill color when the toggle is hovered and off.
                 */
                SFUI::Optional<SFUI::SubProp::Color> hoveredOffFillColor;
                
                /**
                 * @brief Border color when the toggle is hovered and off.
                 */
                SFUI::Optional<SFUI::SubProp::Color> hoveredOffBorderColor;
                
                /**
                 * @brief Fill color when the toggle is hovered and on.
                 */
                SFUI::Optional<SFUI::SubProp::Color> hoveredOnFillColor;
                
                /**
                 * @brief Border color when the toggle is hovered and on.
                 */
                SFUI::Optional<SFUI::SubProp::Color> hoveredOnBorderColor;
                
                /**
                 * @brief Fill color when the toggle is pressed and off.
                 */
                SFUI::Optional<SFUI::SubProp::Color> pressedOffFillColor;
                
                /**
                 * @brief Border color when the toggle is pressed and off.
                 */
                SFUI::Optional<SFUI::SubProp::Color> pressedOffBorderColor;
                
                /**
                 * @brief Fill color when the toggle is pressed and on.
                 */
                SFUI::Optional<SFUI::SubProp::Color> pressedOnFillColor;
                
                /**
                 * @brief Border color when the toggle is pressed and on.
                 */
                SFUI::Optional<SFUI::SubProp::Color> pressedOnBorderColor;
                
                /**
                 * @brief Fill color when the toggle is disabled.
                 */
                SFUI::Optional<SFUI::SubProp::Color> disabledFillColor;
                
                /**
                 * @brief Border color when the toggle is disabled.
                 */
                SFUI::Optional<SFUI::SubProp::Color> disabledBorderColor;
                
                /**
                 * @brief Width of the focus outline.
                 */
                SFUI::SubProp::Dimension focusWidth = 10.0f;
                
                /**
                 * @brief Offset of the focus outline.
                 */
                SFUI::SubProp::Dimension focusOffset = 0.0f;
                
                /**
                 * @brief Corner radius of the focus outline.
                 */
                SFUI::SubProp::UniQuad focusCornerRadius;
                
                /**
                 * @brief Fill color of the focus outline.
                 */
                SFUI::SubProp::Color focusFillColor = SFUI::Color(0, 0, 0, 255);
                
                /**
                 * @brief Padding for the tooltip.
                 */
                SFUI::SubProp::UniQuad toolTipPadding = 10.0f;
                
                /**
                 * @brief Corner radius for the tooltip.
                 */
                SFUI::SubProp::UniQuad toolTipCornerRadius;
                
                /**
                 * @brief Text content of the tooltip.
                 */
                SFUI::SubProp::Keyword toolTipText = "";
                
                /**
                 * @brief Font used for the tooltip text.
                 */
                SFUI::SubProp::Font toolTipFont = std::make_shared<SFUI::Font>();
                
                /**
                 * @brief Size of the tooltip text.
                 */
                SFUI::SubProp::Numeric toolTipTextSize = 10.0f;
                
                /**
                 * @brief Fill color of the tooltip.
                 */
                SFUI::SubProp::Color toolTipFillColor = SFUI::Color(150, 150, 150, 255);
                
                /**
                 * @brief Text color of the tooltip.
                 */
                SFUI::SubProp::Color toolTipTextColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Equality operator for Toggle style properties.
                 */
                bool operator==(const Toggle&) const = default;
                
                /**
                 * @brief Inequality operator for Toggle style properties.
                 */
                bool operator!=(const Toggle&) const = default;
            };

            /**
             * @brief Style properties specific to Graphic components.
             */
            struct Graphic {
                /**
                 * @brief Loaded graphic resource.
                 */
                SFUI::Optional<SFUI::SubProp::Texture> loadedGraphic;
                
                /**
                 * @brief Path to the graphic resource.
                 */
                SFUI::Optional<SFUI::SubProp::Keyword> graphicPath;
                
                /**
                 * @brief Alignment of the graphic within its container.
                 */
                SFUI::SubProp::Keyword graphicAlign;

                /**
                 * @brief Color tint applied to the graphic.
                 */
                bool operator==(const Graphic&) const = default;
                
                /**
                 * @brief Inequality operator for Graphic style properties.
                 */
                bool operator!=(const Graphic&) const = default;
            };

            /**
             * @brief Style properties specific to TextField components.
             */
            struct TextField {
                /**
                 * @brief Line mode of the text field.
                 */
                SFUI::SubProp::Keyword lineMode = "single";
                
                /**
                 * @brief Inset padding for the text content.
                 */
                SFUI::SubProp::UniQuad textInset = 0.0f;
                
                /**
                 * @brief Text content of the text field.
                 */
                SFUI::SubProp::Keyword text;
                
                /**
                 * @brief Placeholder text when the field is empty.
                 */
                SFUI::SubProp::Keyword placeholderText;
                
                /**
                 * @brief Font used for the text field.
                 */
                SFUI::SubProp::Font font = std::make_shared<SFUI::Font>();
                
                /**
                 * @brief Size of the text field text.
                 */
                SFUI::SubProp::Numeric textSize = 12.0f;
                
                /**
                 * @brief Style of the text field text.
                 */
                SFUI::SubProp::TextStyle textStyle = "regular";
                
                /**
                 * @brief Optional letter spacing for the text field text.
                 */
                SFUI::Optional<SFUI::SubProp::Numeric> letterSpacing;
                
                /**
                 * @brief Optional line spacing for the text field text.
                 */
                SFUI::Optional<SFUI::SubProp::Numeric> lineSpacing;
                
                /**
                 * @brief Thickness of the text outline.
                 */
                SFUI::SubProp::Numeric textOutlineThickness = 0.0f;
                
                /**
                 * @brief Horizontal alignment of the text field text.
                 */
                SFUI::SubProp::Keyword textAlignHorizontal = "left";
                
                /**
                 * @brief Vertical alignment of the text field text.
                 */
                SFUI::SubProp::Color textColor = SFUI::Color(0, 0, 0, 255);
                
                /**
                 * @brief Color of the text outline.
                 */
                SFUI::SubProp::Color textOutlineColor = SFUI::Color(255, 255, 255, 255);
                
                /**
                 * @brief Color of the placeholder text.
                 */
                SFUI::SubProp::Color placeholderTextColor = SFUI::Color(100, 100, 100, 255);
                
                /**
                 * @brief Width of the focus outline.
                 */
                SFUI::Optional<SFUI::SubProp::Color> disabledFillColor;
                
                /**
                 * @brief Border color when the text field is disabled.
                 */
                SFUI::Optional<SFUI::SubProp::Color> disabledBorderColor;
                
                /**
                 * @brief Width of the focus outline.
                 */
                SFUI::SubProp::UniQuad toolTipPadding = 10.0f;
                
                /**
                 * @brief Corner radius for the tooltip.
                 */
                SFUI::SubProp::UniQuad toolTipCornerRadius;
                
                /**
                 * @brief Text content of the tooltip.
                 */
                SFUI::SubProp::Keyword toolTipText = "";
                
                /**
                 * @brief Font used for the tooltip text.
                 */
                SFUI::SubProp::Font toolTipFont = std::make_shared<SFUI::Font>();
                
                /**
                 * @brief Size of the tooltip text.
                 */
                SFUI::SubProp::Numeric toolTipTextSize = 10.0f;
                
                /**
                 * @brief Fill color of the tooltip.
                 */
                SFUI::SubProp::Color toolTipFillColor = SFUI::Color(150, 150, 150, 255);
                
                /**
                 * @brief Text color of the tooltip.
                 */
                SFUI::SubProp::Color toolTipTextColor = SFUI::Color(0, 0, 0, 255);
                
                /**
                 * @brief Shape of the caret.
                 */
                SFUI::SubProp::Keyword caretShape = "line";
                
                /**
                 * @brief Width of the caret.
                 */
                SFUI::SubProp::Numeric caretBlinkTime = 500.0f;
                
                /**
                 * @brief Blink ratio of the caret.
                 */
                SFUI::SubProp::Numeric caretBlinkRatio = 1.0f;
                
                /**
                 * @brief Color of the caret.
                 */
                SFUI::SubProp::Color caretFillColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Equality operator for TextField style properties.
                 */
                bool operator==(const TextField&) const = default;
                
                /**
                 * @brief Inequality operator for TextField style properties.
                 */
                bool operator!=(const TextField&) const = default;
            };

            /**
             * @brief Style properties specific to Slider components.
             */
            struct Slider {
                /**
                 * @brief Shadow offset of the slider.
                 */
                SFUI::SubProp::Duplex shadowOffset = {0.0f, 0.0f};
                
                /**
                 * @brief Shadow radius of the slider.
                 */
                SFUI::SubProp::Numeric shadowRadius = 1.0f;
                
                /**
                 * @brief Shadow fill color of the slider.
                 */
                SFUI::SubProp::Color shadowFillColor = SFUI::Color(0, 0, 0, 0);
                
                /**
                 * @brief Alignment of the slider track.
                 */
                SFUI::SubProp::Keyword trackAlign = "horizontal";
                
                /**
                 * @brief Width of the slider track.
                 */
                SFUI::SubProp::Dimension trackWidth = 8.0f;
                
                /**
                 * @brief Corner radius of the slider track.
                 */
                SFUI::SubProp::UniQuad trackCornerRadius = 4.0f;
                
                /**
                 * @brief Optional width of the unprogressed portion of the track.
                 */
                SFUI::Optional<SFUI::SubProp::Dimension> trackProgressedWidth;
                
                /**
                 * @brief Width of the slider thumb.
                 */
                SFUI::SubProp::Dimension thumbWidth = 15.0f;
                
                /**
                 * @brief Height of the slider thumb.
                 */
                SFUI::SubProp::Dimension thumbHeight = 15.0f;
                
                /**
                 * @brief Corner radius of the slider thumb.
                 */
                SFUI::SubProp::UniQuad thumbCornerRadius = 7.5f;
                
                /**
                 * @brief Fill color of the slider track.
                 */
                SFUI::SubProp::Color trackFillColor = SFUI::Color(175, 175, 175, 255);
                
                /**
                 * @brief Border color of the slider track.
                 */
                SFUI::SubProp::Color trackBorderColor = SFUI::Color(175, 175, 175, 255);
                
                /**
                 * @brief Fill color of the slider track when hovered.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedFillColor;
                
                /**
                 * @brief Border color of the slider track when hovered.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedBorderColor;
                
                /**
                 * @brief Fill color of the slider track when pressed.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedHoveredFillColor;
                
                /**
                 * @brief Border color of the slider track when pressed.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedHoveredBorderColor;
                
                /**
                 * @brief Fill color of the slider track when pressed.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedPressedFillColor;
                
                /**
                 * @brief Border color of the slider track when pressed.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedPressedBorderColor;
                
                /**
                 * @brief Fill color of the progressed portion of the slider track.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedFillColor;
                
                /**
                 * @brief Border color of the progressed portion of the slider track.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedBorderColor;
                
                /**
                 * @brief Fill color of the progressed portion of the slider track when hovered.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedHoveredFillColor;
                
                /**
                 * @brief Border color of the progressed portion of the slider track when hovered.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedHoveredBorderColor;
                
                /**
                 * @brief Fill color of the progressed portion of the slider track when pressed.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedPressedFillColor;
                
                /**
                 * @brief Border color of the progressed portion of the slider track when pressed.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedPressedBorderColor;
                
                /**
                 * @brief Fill color of the slider track when disabled.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedDisabledFillColor;
                
                /**
                 * @brief Border color of the slider track when disabled.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedDisabledBorderColor;
                
                /**
                 * @brief Fill color of the progressed portion of the slider track when disabled.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedDisabledFillColor;
                
                /**
                 * @brief Border color of the progressed portion of the slider track when disabled.
                 */
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedDisabledBorderColor;
                
                /**
                 * @brief Fill color of the slider thumb.
                 */
                SFUI::SubProp::Color thumbFillColor = SFUI::Color(0, 0, 0, 255);
                
                /**
                 * @brief Border color of the slider thumb.
                 */
                SFUI::SubProp::Color thumbBorderColor = SFUI::Color(0, 0, 0, 255);
                
                /**
                 * @brief Fill color of the slider thumb when hovered.
                 */
                SFUI::Optional<SFUI::SubProp::Color> thumbHoveredFillColor;
                
                /**
                 * @brief Border color of the slider thumb when hovered.
                 */
                SFUI::Optional<SFUI::SubProp::Color> thumbHoveredBorderColor;
                
                /**
                 * @brief Fill color of the slider thumb when pressed.
                 */
                SFUI::Optional<SFUI::SubProp::Color> thumbPressedFillColor;
                
                /**
                 * @brief Border color of the slider thumb when pressed.
                 */
                SFUI::Optional<SFUI::SubProp::Color> thumbPressedBorderColor;
                
                /**
                 * @brief Fill color of the slider thumb when disabled.
                 */
                SFUI::Optional<SFUI::SubProp::Color> thumbDisabledFillColor;
                
                /**
                 * @brief Border color of the slider thumb when disabled.
                 */
                SFUI::Optional<SFUI::SubProp::Color> thumbDisabledBorderColor;
                
                /**
                 * @brief Width of the focus outline.
                 */
                SFUI::SubProp::Dimension focusWidth = 5.0f;
                
                /**
                 * @brief Offset of the focus outline.
                 */
                SFUI::SubProp::Dimension focusOffset = 0.0f;
                
                /**
                 * @brief Corner radius of the focus outline.
                 */
                SFUI::SubProp::UniQuad focusCornerRadius;
                
                /**
                 * @brief Fill color of the focus outline.
                 */
                SFUI::SubProp::Color focusFillColor = SFUI::Color(0, 0, 0, 255);
                
                /**
                 * @brief Padding for the tooltip.
                 */
                SFUI::SubProp::UniQuad toolTipPadding = 10.0f;
                
                /**
                 * @brief Corner radius for the tooltip.
                 */
                SFUI::SubProp::UniQuad toolTipCornerRadius;
                
                /**
                 * @brief Text content of the tooltip.
                 */
                SFUI::SubProp::Keyword toolTipText = "";
                
                /**
                 * @brief Font used for the tooltip text.
                 */
                SFUI::SubProp::Font toolTipFont = std::make_shared<SFUI::Font>();
                
                /**
                 * @brief Size of the tooltip text.
                 */
                SFUI::SubProp::Numeric toolTipTextSize = 10.0f;
                
                /**
                 * @brief Fill color of the tooltip.
                 */
                SFUI::SubProp::Color toolTipFillColor = SFUI::Color(150, 150, 150, 255);
                
                /**
                 * @brief Text color of the tooltip.
                 */
                SFUI::SubProp::Color toolTipTextColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Equality operator for Slider style properties.
                 */
                bool operator==(const Slider&) const = default;
                
                /**
                 * @brief Inequality operator for Slider style properties.
                 */
                bool operator!=(const Slider&) const = default;
            };
        }

        /**
         * @brief State-related properties for components.
         *
         * This namespace contains structs and types that define
         * the current state of components, such as whether they
         * are disabled, focused, or toggled on/off.
         */
        namespace State {

            /**
             * @brief State properties for a Button component.
             */
            struct Button {
                /**
                 * @brief Indicates whether the button is disabled.
                 */
                SFUI::SubProp::Binary isDisabled = false;
                
                /**
                 * @brief Indicates whether the button is currently focused.
                 */
                SFUI::SubProp::Binary isFocused = false;

                /**
                 * @brief Equality operator for Button state properties.
                 */
                bool operator==(const Button&) const = default;
                
                /**
                 * @brief Inequality operator for Button state properties.
                 */
                bool operator!=(const Button&) const = default;
            };

            /**
             * @brief State properties specific to Toggle components.
             */
            struct Toggle {
                /**
                 * @brief Indicates whether the toggle is disabled.
                 */
                SFUI::SubProp::Binary isDisabled = false;
                
                /**
                 * @brief Indicates whether the toggle is currently focused.
                 */
                SFUI::SubProp::Binary isFocused = false;
                
                /**
                 * @brief Indicates whether the toggle is in the "on" state.
                 */
                SFUI::SubProp::Binary isOn = false;

                /**
                 * @brief Equality operator for Toggle state properties.
                 */
                bool operator==(const Toggle&) const = default;
                
                /**
                 * @brief Inequality operator for Toggle state properties.
                 */
                bool operator!=(const Toggle&) const = default;
            };

            /**
             * @brief State properties specific to Graphic components.
             */
            struct TextField {
                /**
                 * @brief Indicates whether the text field is disabled.
                 */
                SFUI::SubProp::Binary isDisabled = false;
                
                /**
                 * @brief Indicates whether the text field is currently focused.
                 */
                SFUI::SubProp::Binary isFocused = false;

                /**
                 * @brief Current text content of the text field.
                 */
                bool operator==(const TextField&) const = default;
                
                /**
                 * @brief Inequality operator for TextField state properties.
                 */
                bool operator!=(const TextField&) const = default;
            };

            /**
             * @brief State properties specific to Slider components.
             */
            struct Slider {
                /**
                 * @brief Indicates whether the slider is disabled.
                 */
                SFUI::SubProp::Binary isDisabled = false;
                
                /**
                 * @brief Indicates whether the slider is currently focused.
                 */
                SFUI::SubProp::Binary isFocused = false;
                
                /**
                 * @brief Indicates whether the slider's direction is inverted.
                 */
                SFUI::SubProp::Binary isInverted = false;
                
                /**
                 * @brief Current value of the slider.
                 */
                SFUI::SubProp::Numeric value = 0.0f;
                
                /**
                 * @brief Minimum value of the slider.
                 */
                SFUI::SubProp::Numeric minimumValue = 0.0f;
                
                /**
                 * @brief Maximum value of the slider.
                 */
                SFUI::SubProp::Numeric maximumValue = 20.0f;
                
                /**
                 * @brief Step increment for the slider value.
                 */
                SFUI::SubProp::Numeric step = 1.0f;

                /**
                 * @brief Equality operator for Slider state properties.
                 */
                bool operator==(const Slider&) const = default;
                
                /**
                 * @brief Inequality operator for Slider state properties.
                 */
                bool operator!=(const Slider&) const = default;
            };
        }

        /**
         * @brief Behavior-related properties for components.
         *
         * This namespace contains structs and types that define
         * the interactive behavior of components, including
         * callback functions for various user input events.
         */
        namespace Behavior {
            
            /**
             * @brief Behavior properties for a generic component.
             */
            struct ScrollContainer {
                /**
                 * @brief Callback function triggered on scroll events.
                 */
                SFUI::SubProp::Callback onScroll;
            };

            /**
             * @brief Behavior properties specific to Button components.
             */
            struct Button {
                /**
                 * @brief Callback function triggered when the button is enabled.
                 */
                SFUI::SubProp::Callback onEnable;
                
                /**
                 * @brief Callback function triggered when the button is disabled.
                 */
                SFUI::SubProp::Callback onDisable;
                
                /**
                 * @brief Callback function triggered when the button gains focus.
                 */
                SFUI::SubProp::Callback onFocus;
                
                /**
                 * @brief Callback function triggered when the button loses focus.
                 */
                SFUI::SubProp::Callback onBlur;
                
                /**
                 * @brief Callback function triggered when the mouse hovers over the button.
                 */
                SFUI::SubProp::Callback onHoverIn;
                
                /**
                 * @brief Callback function triggered when the mouse stops hovering over the button.
                 */
                SFUI::SubProp::Callback onHoverOut;
                
                /**
                 * @brief Callback function triggered when the left mouse button is pressed down on the button.
                 */
                SFUI::SubProp::Callback onLeftPressIn;
                
                /**
                 * @brief Callback function triggered when the left mouse button is released on the button.
                 */
                SFUI::SubProp::Callback onLeftPress;
                
                /**
                 * @brief Callback function triggered when the right mouse button is pressed down on the button.
                 */
                SFUI::SubProp::Callback onRightPressIn;
                
                /**
                 * @brief Callback function triggered when the right mouse button is released on the button.
                 */
                SFUI::SubProp::Callback onRightPress;
                
                /**
                 * @brief Callback function triggered when the middle mouse button is pressed down on the button.
                 */
                SFUI::SubProp::Callback onMiddlePressIn;
                
                /**
                 * @brief Callback function triggered when the middle mouse button is released on the button.
                 */
                SFUI::SubProp::Callback onMiddlePress;
                
                /**
                 * @brief Callback function triggered when the button is double-clicked.
                 */
                SFUI::SubProp::Callback onDoublePress;
                
                /**
                 * @brief Callback function triggered when the mouse button is released outside the button.
                 */
                SFUI::SubProp::CallbackKey onKeyPress;
            };

            /**
             * @brief Behavior properties specific to Toggle components.
             */
            struct Toggle {
                /**
                 * @brief Callback function triggered when the toggle is enabled.
                 */
                SFUI::SubProp::Callback onEnable;
                
                /**
                 * @brief Callback function triggered when the toggle is disabled.
                 */
                SFUI::SubProp::Callback onDisable;
                
                /**
                 * @brief Callback function triggered when the toggle gains focus.
                 */
                SFUI::SubProp::Callback onFocus;
                
                /**
                 * @brief Callback function triggered when the toggle loses focus.
                 */
                SFUI::SubProp::Callback onBlur;
                
                /**
                 * @brief Callback function triggered when the mouse hovers over the toggle.
                 */
                SFUI::SubProp::Callback onHoverIn;
                
                /**
                 * @brief Callback function triggered when the mouse stops hovering over the toggle.
                 */
                SFUI::SubProp::Callback onHoverOut;
                
                /**
                 * @brief Callback function triggered when the left mouse button is pressed down on the toggle.
                 */
                SFUI::SubProp::Callback onLeftPressIn;
                
                /**
                 * @brief Callback function triggered when the left mouse button is released on the toggle.
                 */
                SFUI::SubProp::Callback onLeftPress;
                
                /**
                 * @brief Callback function triggered when the right mouse button is pressed down on the toggle.
                 */
                SFUI::SubProp::Callback onRightPressIn;
                
                /**
                 * @brief Callback function triggered when the right mouse button is released on the toggle.
                 */
                SFUI::SubProp::Callback onRightPress;
                
                /**
                 * @brief Callback function triggered when the middle mouse button is pressed down on the toggle.
                 */
                SFUI::SubProp::Callback onMiddlePressIn;
                
                /**
                 * @brief Callback function triggered when the middle mouse button is released on the toggle.
                 */
                SFUI::SubProp::Callback onMiddlePress;
                
                /**
                 * @brief Callback function triggered when the toggle is double-clicked.
                 */
                SFUI::SubProp::Callback onDoublePress;
                
                /**
                 * @brief Callback function triggered when the mouse button is released outside the toggle.
                 */
                SFUI::SubProp::CallbackKey onKeyPress;
                
                /**
                 * @brief Callback function triggered when the toggle state changes.
                 */
                SFUI::SubProp::CallbackBinary onToggledState;
            };

            /**
             * @brief Behavior properties specific to Graphic components.
             */
            struct Graphic {
                SFUI::SubProp::Callback onLoad;
                SFUI::SubProp::Callback onLoadError;
            };

            /**
             * @brief Behavior properties specific to TextField components.
             */
            struct TextField {
                /**
                 * @brief Callback function triggered when the text field is enabled.
                 */
                SFUI::SubProp::Callback onEnable;
                
                /**
                 * @brief Callback function triggered when the text field is disabled.
                 */
                SFUI::SubProp::Callback onDisable;
                
                /**
                 * @brief Callback function triggered when the text field gains focus.
                 */
                SFUI::SubProp::Callback onFocus;
                
                /**
                 * @brief Callback function triggered when the text field loses focus.
                 */
                SFUI::SubProp::Callback onBlur;
                
                /**
                 * @brief Callback function triggered when the mouse hovers over the text field.
                 */
                SFUI::SubProp::Callback onHoverIn;
                
                /**
                 * @brief Callback function triggered when the mouse stops hovering over the text field.
                 */
                SFUI::SubProp::Callback onHoverOut;
                
                /**
                 * @brief Callback function triggered when the left mouse button is pressed down on the text field.
                 */
                SFUI::SubProp::Callback onLeftPressIn;
                
                /**
                 * @brief Callback function triggered when the left mouse button is released on the text field.
                 */
                SFUI::SubProp::Callback onLeftPress;
                
                /**
                 * @brief Callback function triggered when the right mouse button is pressed down on the text field.
                 */
                SFUI::SubProp::Callback onRightPressIn;
                
                /**
                 * @brief Callback function triggered when the right mouse button is released on the text field.
                 */
                SFUI::SubProp::Callback onRightPress;
                
                /**
                 * @brief Callback function triggered when the middle mouse button is pressed down on the text field.
                 */
                SFUI::SubProp::Callback onMiddlePressIn;
                
                /**
                 * @brief Callback function triggered when the middle mouse button is released on the text field.
                 */
                SFUI::SubProp::Callback onMiddlePress;
                
                /**
                 * @brief Callback function triggered when the text field is double-clicked.
                 */
                SFUI::SubProp::Callback onDoublePress;
                
                /**
                 * @brief Callback function triggered when the mouse button is released outside the text field.
                 */
                SFUI::SubProp::Callback onPressOut;
                
                /**
                 * @brief Callback function triggered when a key is pressed while the text field is focused.
                 */
                SFUI::SubProp::CallbackKey onKeyPress;
                
                /**
                 * @brief Callback function triggered when the text in the text field changes.
                 */
                SFUI::SubProp::CallbackKeyword onTextChange;
                
                /**
                 * @brief Callback function triggered when the text field is submitted (e.g., Enter key pressed).
                 */
                SFUI::SubProp::CallbackKeyword onSubmit;
            };

            /**
             * @brief Behavior properties specific to Slider components.
             */
            struct Slider {
                /**
                 * @brief Callback function triggered when the slider is enabled.
                 */
                SFUI::SubProp::Callback onEnable;
                
                /**
                 * @brief Callback function triggered when the slider is disabled.
                 */
                SFUI::SubProp::Callback onDisable;
                
                /**
                 * @brief Callback function triggered when the slider gains focus.
                 */
                SFUI::SubProp::Callback onFocus;
                
                /**
                 * @brief Callback function triggered when the slider loses focus.
                 */
                SFUI::SubProp::Callback onBlur;
                
                /**
                 * @brief Callback function triggered when the mouse hovers over the slider track.
                 */
                SFUI::SubProp::Callback onTrackHoverIn;
                
                /**
                 * @brief Callback function triggered when the mouse stops hovering over the slider track.
                 */
                SFUI::SubProp::Callback onTrackHoverOut;
                
                /**
                 * @brief Callback function triggered when the left mouse button is pressed down on the slider track.
                 */
                SFUI::SubProp::Callback onTrackLeftPressIn;
                
                /**
                 * @brief Callback function triggered when the left mouse button is released on the slider track.
                 */
                SFUI::SubProp::Callback onTrackLeftPress;
                
                /**
                 * @brief Callback function triggered when the right mouse button is pressed down on the slider track.
                 */
                SFUI::SubProp::Callback onTrackRightPressIn;
                
                /**
                 * @brief Callback function triggered when the right mouse button is released on the slider track.
                 */
                SFUI::SubProp::Callback onTrackRightPress;
                
                /**
                 * @brief Callback function triggered when the middle mouse button is pressed down on the slider track.
                 */
                SFUI::SubProp::Callback onTrackMiddlePressIn;
                
                /**
                 * @brief Callback function triggered when the middle mouse button is released on the slider track.
                 */
                SFUI::SubProp::Callback onTrackMiddlePress;
                
                /**
                 * @brief Callback function triggered when the mouse hovers over the slider thumb.
                 */
                SFUI::SubProp::Callback onThumbHoverIn;
                
                /**
                 * @brief Callback function triggered when the mouse stops hovering over the slider thumb.
                 */
                SFUI::SubProp::Callback onThumbHoverOut;
                
                /**
                 * @brief Callback function triggered when the left mouse button is pressed down on the slider thumb.
                 */
                SFUI::SubProp::Callback onThumbLeftPressIn;
                
                /**
                 * @brief Callback function triggered when the left mouse button is released on the slider thumb.
                 */
                SFUI::SubProp::Callback onThumbLeftPress;
                
                /**
                 * @brief Callback function triggered when the right mouse button is pressed down on the slider thumb.
                 */
                SFUI::SubProp::Callback onThumbRightPressIn;
                
                /**
                 * @brief Callback function triggered when the right mouse button is released on the slider thumb.
                 */
                SFUI::SubProp::Callback onThumbRightPress;
                
                /**
                 * @brief Callback function triggered when the middle mouse button is pressed down on the slider thumb.
                 */
                SFUI::SubProp::Callback onThumbMiddlePressIn;
                
                /**
                 * @brief Callback function triggered when the middle mouse button is released on the slider thumb.
                 */
                SFUI::SubProp::Callback onThumbMiddlePress;
                
                /**
                 * @brief Callback function triggered when the slider is double-clicked.
                 */
                SFUI::SubProp::CallbackKey onKeyPress;
                
                /**
                 * @brief Callback function triggered when sliding starts.
                 */
                SFUI::SubProp::CallbackNumeric onSlidingStart;
                
                /**
                 * @brief Callback function triggered when sliding ends.
                 */
                SFUI::SubProp::CallbackNumeric onSlidingEnd;
                
                /**
                 * @brief Callback function triggered when the slider value changes.
                 */
                SFUI::SubProp::CallbackNumeric onValueChange;
            };
        }
    }
}
