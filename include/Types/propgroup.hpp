/**
 * @file propgroup.hpp
 * @brief Defines the main component property structures for layout, style, state, and behavior in SFUI.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.2
 *
 * This file contains layout, style, state, and behavior property group
 * definitions for all SFUI components, including optional and variant
 * types for flexible property assignment.
 */


#pragma once
#include "base.hpp"
#include "prop.hpp"


namespace SFUI {

    /**
     * @brief Component property group definitions.
     *
     * This namespace contains all the base property group types used
     * by all child component classes, it is used for configuring components
     * in SFML-FlexUI with layout, style, state, and behavior properties.
     */
    namespace PropGroup {

        /**
         * @brief Component property group definitions.
         */
        namespace Component {

            /**
             * @brief Layout prop-group definitions with layout-specific props.
             *
             * These layout props are associated with the base component parent
             * class and will belong to every single sub-component child type.
             */
            struct Layout {

                /**
                 * @brief Primary alignment direction within the parent container for its child components.
                 */
                SFUI::Prop::Keyword alignDirection = "vertical";

                /**
                 * @brief On-Axis alignment of children within the parent container.
                 */
                SFUI::Prop::Keyword alignPrimary = "start";

                /**
                 * @brief Cross-Axis alignment of children within the parent container.
                 */
                SFUI::Prop::Keyword alignSecondary = "start";

                /**
                 * @brief Width of the component.
                 */
                SFUI::Prop::Dimension width = 0.0f;

                /**
                 * @brief Height of the component.
                 */
                SFUI::Prop::Dimension height = 0.0f;

                /**
                 * @brief Padding inside the component.
                 */
                SFUI::Prop::UniQuad padding = 0.0f;

                /**
                 * @brief Margin outside the component.
                 */
                SFUI::Prop::UniQuad margin = 0.0f;

                /**
                 * @brief X position offset of the component.
                 */
                SFUI::Optional<SFUI::Prop::Numeric> xPosition;

                /**
                 * @brief Y position offset of the component.
                 */
                SFUI::Optional<SFUI::Prop::Numeric> yPosition;

                /**
                 * @brief Equality operator for Component layout properties.
                 */
                bool operator==(const Layout&) const = default;

                /**
                 * @brief Inequality operator for Component layout properties.
                 */
                bool operator!=(const Layout&) const = default;
            };

            /**
             * @brief Style prop-group definitions with style-specific props.
             *
             * These style props are associated with the base component parent
             * class and will belong to every single sub-component child type.
             */
            struct Style {

                /**
                 * @brief Width of the component's border.
                 */
                SFUI::Prop::Dimension borderWidth = 0.0f;

                /**
                 * @brief Corner radius of the component.
                 */
                SFUI::Prop::UniQuad cornerRadius;

                /**
                 * @brief Fill color of the component.
                 */
                SFUI::Prop::Color fillColor = SFUI::Color(0, 0, 0, 0);

                /**
                 * @brief Border color of the component.
                 */
                SFUI::Prop::Color borderColor = SFUI::Color(0, 0, 0, 0);

                /**
                 * @brief Shadow offset of the component.
                 */
                SFUI::Prop::Duplex shadowOffset = {0.0f, 0.0f};

                /**
                 * @brief Shadow radius of the component.
                 */
                SFUI::Prop::Numeric shadowRadius = 1.0f;

                /**
                 * @brief Shadow fill color of the component.
                 */
                SFUI::Prop::Color shadowFillColor = SFUI::Color(0, 0, 0, 0);

                /**
                 * @brief Equality operator for Component style properties.
                 */
                bool operator==(const Style&) const = default;

                /**
                 * @brief Inequality operator for Component style properties.
                 */
                bool operator!=(const Style&) const = default;
            };

            /**
             * @brief State prop-group definitions with state-specific props.
             *
             * These state props are associated with the base component parent
             * class and will belong to every single sub-component child type.
             */
            struct State {

                /**
                 * @brief Equality operator for Component state properties.
                 */
                bool operator==(const State&) const = default;

                /**
                 * @brief Inequality operator for Componenty state properties.
                 */
                bool operator!=(const State&) const = default;
            };

            /**
             * @brief Behavior prop-group definitions with behavior-specific props.
             *
             * These behavior props are associated with the base component parent
             * class and will belong to every single sub-component child type.
             */
            struct Behavior {

            };
        }

        /**
         * @brief Container property group definitions.
         */
        namespace Container {

            /**
             * @brief Container layout property group definitions.
             */
            struct Layout : public SFUI::PropGroup::Component::Layout {

            };

            /**
             * @brief Container style property group definitions.
             */
            struct Style : public SFUI::PropGroup::Component::Style {

            };

            /**
             * @brief Container state property group definitions.
             */
            struct State : public SFUI::PropGroup::Component::State {

            };

            /**
             * @brief Container behavior property group definitions.
             */
            struct Behavior : public SFUI::PropGroup::Component::Behavior {

            };
        }

        /**
         * @brief Scroll Component property group definitions.
         */
        namespace ScrollContainer {

            /**
             * @brief Scroll Container layout property group definitions.
             */
            struct Layout : public SFUI::PropGroup::Component::Layout {

            };

            /**
             * @brief Container style property group definitions.
             */
            struct Style : public SFUI::PropGroup::Component::Style {

                /**
                 * @brief Scroll direction of the container.
                 */
                SFUI::Prop::Keyword scrollDirection = "vertical";

                /**
                 * @brief Scroll speed factor of the container.
                 */
                SFUI::Prop::Numeric scrollSpeedFactor = 15.0f;

                /**
                 * @brief Equality operator for ScrollContainer style properties.
                 */
                bool operator==(const Style&) const = default;

                /**
                 * @brief Inequality operator for ScrollContainer style properties.
                 */
                bool operator!=(const Style&) const = default;
            };

            /**
             * @brief Container state property group definitions.
             */
            struct State : public SFUI::PropGroup::Component::State {

            };

            /**
             * @brief Container behavior property group definitions.
             */
            struct Behavior : public SFUI::PropGroup::Component::Behavior {

                /**
                 * @brief Callback function triggered on scroll events.
                 */
                SFUI::Prop::Callback onScroll;
            };
        }

        /**
         * @brief Label property group definitions.
         */
        namespace Label {

            /**
             * @brief Label layout property group definitions.
             */
            struct Layout : public SFUI::PropGroup::Component::Layout {

            };

            /**
             * @brief Label style property group definitions.
             */
            struct Style : public SFUI::PropGroup::Component::Style {

                /**
                 * @brief Text content of the label.
                 */
                SFUI::Prop::Keyword text;

                /**
                 * @brief Font used for the label text.
                 */
                SFUI::Prop::Font font = std::make_shared<SFUI::Font>();

                /**
                 * @brief Size of the label text.
                 */
                SFUI::Prop::Numeric textSize = 12.0f;

                /**
                 * @brief Style of the label text.
                 */
                SFUI::Prop::TextStyle textStyle = "regular";

                /**
                 * @brief Optional letter spacing for the label text.
                 */
                SFUI::Optional<SFUI::Prop::Numeric> letterSpacing;

                /**
                 * @brief Optional line spacing for the label text.
                 */
                SFUI::Optional<SFUI::Prop::Numeric> lineSpacing;

                /**
                 * @brief Thickness of the text outline.
                 */
                SFUI::Prop::Numeric textOutlineThickness = 0.0f;

                /**
                 * @brief Horizontal alignment of the label text.
                 */
                SFUI::Prop::Keyword textAlignHorizontal = "left";

                /**
                 * @brief Vertical alignment of the label text.
                 */
                SFUI::Prop::Keyword textAlignVertical = "center";

                /**
                 * @brief Offset of the label text.
                 */
                SFUI::Prop::Duplex textOffset = {0.0f, 0.0f};

                /**
                 * @brief Color of the label text.
                 */
                SFUI::Prop::Color textColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Color of the label text outline.
                 */
                SFUI::Prop::Color textOutlineColor = SFUI::Color(255, 255, 255, 255);

                /**
                 * @brief Equality operator for Label style properties.
                 */
                bool operator==(const Style&) const = default;

                /**
                 * @brief Inequality operator for Label style properties.
                 */
                bool operator!=(const Style&) const = default;
            };

            /**
             * @brief Container state property group definitions.
             */
            struct State : public SFUI::PropGroup::Component::State {

            };

            /**
             * @brief Container behavior property group definitions.
             */
            struct Behavior : public SFUI::PropGroup::Component::Behavior {

            };
        }

        /**
         * @brief Graphic property group definitions.
         */
        namespace Graphic {

            /**
             * @brief Graphic layout property group definitions.
             */
            struct Layout : public SFUI::PropGroup::Component::Layout {

            };

            /**
             * @brief Graphic style property group definitions.
             */
            struct Style : public SFUI::PropGroup::Component::Style {

                /**
                 * @brief Loaded graphic resource.
                 */
                SFUI::Optional<SFUI::Prop::Texture> loadedGraphic;

                /**
                 * @brief Path to the graphic resource.
                 */
                SFUI::Optional<SFUI::Prop::Keyword> graphicPath;

                /**
                 * @brief Alignment of the graphic within its container.
                 */
                SFUI::Prop::Keyword graphicAlign;

                /**
                 * @brief Color tint applied to the graphic.
                 */
                bool operator==(const Style&) const = default;

                /**
                 * @brief Inequality operator for Graphic style properties.
                 */
                bool operator!=(const Style&) const = default;
            };

            /**
             * @brief Graphic state property group definitions.
             */
            struct State : public SFUI::PropGroup::Component::State {

            };

            /**
             * @brief Graphic behavior property group definitions.
             */
            struct Behavior : public SFUI::PropGroup::Component::Behavior {

                /**
                 * @brief Callback function triggered when the self-loaded image is loaded.
                 */
                SFUI::Prop::Callback onLoad;

                /**
                 * @brief Callback function triggered when the self-loaded image errors when loading.
                 */
                SFUI::Prop::Callback onLoadError;
            };
        }

        /**
         * @brief Button property group definitions.
         */
        namespace Button {

            /**
             * @brief Button layout property group definitions.
             */
            struct Layout : public SFUI::PropGroup::Component::Layout {

            };

            /**
             * @brief Button style property group definitions.
             */
            struct Style : public SFUI::PropGroup::Component::Style {
                /**
                 * @brief Fill color when the button is in its normal state.
                 */
                SFUI::Optional<SFUI::Prop::Color> hoveredFillColor;

                /**
                 * @brief Border color when the button is in its normal state.
                 */
                SFUI::Optional<SFUI::Prop::Color> hoveredBorderColor;

                /**
                 * @brief Fill color when the button is pressed.
                 */
                SFUI::Optional<SFUI::Prop::Color> pressedFillColor;

                /**
                 * @brief Border color when the button is pressed.
                 */
                SFUI::Optional<SFUI::Prop::Color> pressedBorderColor;

                /**
                 * @brief Fill color when the button is disabled.
                 */
                SFUI::Optional<SFUI::Prop::Color> disabledFillColor;

                /**
                 * @brief Border color when the button is disabled.
                 */
                SFUI::Optional<SFUI::Prop::Color> disabledBorderColor;

                /**
                 * @brief Width of the focus outline.
                 */
                SFUI::Prop::Dimension focusWidth = 10.0f;

                /**
                 * @brief Offset of the focus outline away from the edge of the component geometry.
                 */
                SFUI::Prop::Dimension focusOffset = 0.0f;

                /**
                 * @brief Corner radius of the focus outline.
                 */
                SFUI::Prop::UniQuad focusCornerRadius;

                /**
                 * @brief Fill color of the focus outline.
                 */
                SFUI::Prop::Color focusFillColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Padding for the tooltip.
                 */
                SFUI::Prop::UniQuad toolTipPadding = 10.0f;

                /**
                 * @brief Corner radius for the tooltip.
                 */
                SFUI::Prop::UniQuad toolTipCornerRadius;

                /**
                 * @brief Text content of the tooltip.
                 */
                SFUI::Prop::Keyword toolTipText = "";

                /**
                 * @brief Font used for the tooltip text.
                 */
                SFUI::Prop::Font toolTipFont = std::make_shared<SFUI::Font>();

                /**
                 * @brief Size of the tooltip text.
                 */
                SFUI::Prop::Numeric toolTipTextSize = 10.0f;

                /**
                 * @brief Fill color of the tooltip.
                 */
                SFUI::Prop::Color toolTipFillColor = SFUI::Color(150, 150, 150, 255);

                /**
                 * @brief Text color of the tooltip.
                 */
                SFUI::Prop::Color toolTipTextColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Equality operator for Button style properties.
                 */
                bool operator==(const Style&) const = default;

                /**
                 * @brief Inequality operator for Button style properties.
                 */
                bool operator!=(const Style&) const = default;
            };

            /**
             * @brief Button state property group definitions.
             */
            struct State : public SFUI::PropGroup::Component::State {

                /**
                 * @brief Indicates whether the button is disabled.
                 */
                SFUI::Prop::Binary isDisabled = false;

                /**
                 * @brief Indicates whether the button is currently focused.
                 */
                SFUI::Prop::Binary isFocused = false;

                /**
                 * @brief Equality operator for Button state properties.
                 */
                bool operator==(const State&) const = default;

                /**
                 * @brief Inequality operator for Button state properties.
                 */
                bool operator!=(const State&) const = default;
            };

            /**
             * @brief Button behavior property group definitions.
             */
            struct Behavior : public SFUI::PropGroup::Component::Behavior {

                /**
                 * @brief Callback function triggered when the button is enabled.
                 */
                SFUI::Prop::Callback onEnable;

                /**
                 * @brief Callback function triggered when the button is disabled.
                 */
                SFUI::Prop::Callback onDisable;

                /**
                 * @brief Callback function triggered when the button gains focus.
                 */
                SFUI::Prop::Callback onFocus;

                /**
                 * @brief Callback function triggered when the button loses focus.
                 */
                SFUI::Prop::Callback onBlur;

                /**
                 * @brief Callback function triggered when the mouse hovers over the button.
                 */
                SFUI::Prop::Callback onHoverIn;

                /**
                 * @brief Callback function triggered when the mouse stops hovering over the button.
                 */
                SFUI::Prop::Callback onHoverOut;

                /**
                 * @brief Callback function triggered when the left mouse button is pressed down on the button.
                 */
                SFUI::Prop::Callback onLeftPressIn;

                /**
                 * @brief Callback function triggered when the left mouse button is released on the button.
                 */
                SFUI::Prop::Callback onLeftPress;

                /**
                 * @brief Callback function triggered when the right mouse button is pressed down on the button.
                 */
                SFUI::Prop::Callback onRightPressIn;

                /**
                 * @brief Callback function triggered when the right mouse button is released on the button.
                 */
                SFUI::Prop::Callback onRightPress;

                /**
                 * @brief Callback function triggered when the middle mouse button is pressed down on the button.
                 */
                SFUI::Prop::Callback onMiddlePressIn;

                /**
                 * @brief Callback function triggered when the middle mouse button is released on the button.
                 */
                SFUI::Prop::Callback onMiddlePress;

                /**
                 * @brief Callback function triggered when the button is double-clicked.
                 */
                SFUI::Prop::Callback onDoublePress;

                /**
                 * @brief Callback function triggered when the mouse button is released outside the button.
                 */
                SFUI::Prop::CallbackKey onKeyPress;
            };
        }

        /**
         * @brief Toggle property group definitions.
         */
        namespace Toggle {

            /**
             * @brief Toggle layout property group definitions.
             */
            struct Layout : public SFUI::PropGroup::Component::Layout {

            };

            /**
             * @brief Toggle style property group definitions.
             */
            struct Style : public SFUI::PropGroup::Component::Style {

                /**
                 * @brief Fill color when the toggle is off.
                 */
                SFUI::Prop::Color offFillColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Border color when the toggle is off.
                 */
                SFUI::Prop::Color offBorderColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Fill color when the toggle is on.
                 */
                SFUI::Prop::Color onFillColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Border color when the toggle is on.
                 */
                SFUI::Prop::Color onBorderColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Fill color when the toggle is hovered and off.
                 */
                SFUI::Optional<SFUI::Prop::Color> hoveredOffFillColor;

                /**
                 * @brief Border color when the toggle is hovered and off.
                 */
                SFUI::Optional<SFUI::Prop::Color> hoveredOffBorderColor;

                /**
                 * @brief Fill color when the toggle is hovered and on.
                 */
                SFUI::Optional<SFUI::Prop::Color> hoveredOnFillColor;

                /**
                 * @brief Border color when the toggle is hovered and on.
                 */
                SFUI::Optional<SFUI::Prop::Color> hoveredOnBorderColor;

                /**
                 * @brief Fill color when the toggle is pressed and off.
                 */
                SFUI::Optional<SFUI::Prop::Color> pressedOffFillColor;

                /**
                 * @brief Border color when the toggle is pressed and off.
                 */
                SFUI::Optional<SFUI::Prop::Color> pressedOffBorderColor;

                /**
                 * @brief Fill color when the toggle is pressed and on.
                 */
                SFUI::Optional<SFUI::Prop::Color> pressedOnFillColor;

                /**
                 * @brief Border color when the toggle is pressed and on.
                 */
                SFUI::Optional<SFUI::Prop::Color> pressedOnBorderColor;

                /**
                 * @brief Fill color when the toggle is disabled.
                 */
                SFUI::Optional<SFUI::Prop::Color> disabledFillColor;

                /**
                 * @brief Border color when the toggle is disabled.
                 */
                SFUI::Optional<SFUI::Prop::Color> disabledBorderColor;

                /**
                 * @brief Width of the focus outline.
                 */
                SFUI::Prop::Dimension focusWidth = 10.0f;

                /**
                 * @brief Offset of the focus outline.
                 */
                SFUI::Prop::Dimension focusOffset = 0.0f;

                /**
                 * @brief Corner radius of the focus outline.
                 */
                SFUI::Prop::UniQuad focusCornerRadius;

                /**
                 * @brief Fill color of the focus outline.
                 */
                SFUI::Prop::Color focusFillColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Padding for the tooltip.
                 */
                SFUI::Prop::UniQuad toolTipPadding = 10.0f;

                /**
                 * @brief Corner radius for the tooltip.
                 */
                SFUI::Prop::UniQuad toolTipCornerRadius;

                /**
                 * @brief Text content of the tooltip.
                 */
                SFUI::Prop::Keyword toolTipText = "";

                /**
                 * @brief Font used for the tooltip text.
                 */
                SFUI::Prop::Font toolTipFont = std::make_shared<SFUI::Font>();

                /**
                 * @brief Size of the tooltip text.
                 */
                SFUI::Prop::Numeric toolTipTextSize = 10.0f;

                /**
                 * @brief Fill color of the tooltip.
                 */
                SFUI::Prop::Color toolTipFillColor = SFUI::Color(150, 150, 150, 255);

                /**
                 * @brief Text color of the tooltip.
                 */
                SFUI::Prop::Color toolTipTextColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Equality operator for Toggle style properties.
                 */
                bool operator==(const Style&) const = default;

                /**
                 * @brief Inequality operator for Toggle style properties.
                 */
                bool operator!=(const Style&) const = default;
            };

            /**
             * @brief Toggle state property group definitions.
             */
            struct State : public SFUI::PropGroup::Component::State {

                /**
                 * @brief Indicates whether the toggle is disabled.
                 */
                SFUI::Prop::Binary isDisabled = false;

                /**
                 * @brief Indicates whether the toggle is currently focused.
                 */
                SFUI::Prop::Binary isFocused = false;

                /**
                 * @brief Indicates whether the toggle is in the "on" state.
                 */
                SFUI::Prop::Binary isOn = false;

                /**
                 * @brief Equality operator for Toggle state properties.
                 */
                bool operator==(const State&) const = default;

                /**
                 * @brief Inequality operator for Toggle state properties.
                 */
                bool operator!=(const State&) const = default;
            };

            /**
             * @brief Toggle behavior property group definitions.
             */
            struct Behavior : public SFUI::PropGroup::Component::Behavior {

                /**
                 * @brief Callback function triggered when the toggle is enabled.
                 */
                SFUI::Prop::Callback onEnable;

                /**
                 * @brief Callback function triggered when the toggle is disabled.
                 */
                SFUI::Prop::Callback onDisable;

                /**
                 * @brief Callback function triggered when the toggle gains focus.
                 */
                SFUI::Prop::Callback onFocus;

                /**
                 * @brief Callback function triggered when the toggle loses focus.
                 */
                SFUI::Prop::Callback onBlur;

                /**
                 * @brief Callback function triggered when the mouse hovers over the toggle.
                 */
                SFUI::Prop::Callback onHoverIn;

                /**
                 * @brief Callback function triggered when the mouse stops hovering over the toggle.
                 */
                SFUI::Prop::Callback onHoverOut;

                /**
                 * @brief Callback function triggered when the left mouse button is pressed down on the toggle.
                 */
                SFUI::Prop::Callback onLeftPressIn;

                /**
                 * @brief Callback function triggered when the left mouse button is released on the toggle.
                 */
                SFUI::Prop::Callback onLeftPress;

                /**
                 * @brief Callback function triggered when the right mouse button is pressed down on the toggle.
                 */
                SFUI::Prop::Callback onRightPressIn;

                /**
                 * @brief Callback function triggered when the right mouse button is released on the toggle.
                 */
                SFUI::Prop::Callback onRightPress;

                /**
                 * @brief Callback function triggered when the middle mouse button is pressed down on the toggle.
                 */
                SFUI::Prop::Callback onMiddlePressIn;

                /**
                 * @brief Callback function triggered when the middle mouse button is released on the toggle.
                 */
                SFUI::Prop::Callback onMiddlePress;

                /**
                 * @brief Callback function triggered when the toggle is double-clicked.
                 */
                SFUI::Prop::Callback onDoublePress;

                /**
                 * @brief Callback function triggered when the mouse button is released outside the toggle.
                 */
                SFUI::Prop::CallbackKey onKeyPress;

                /**
                 * @brief Callback function triggered when the toggle state changes.
                 */
                SFUI::Prop::CallbackBinary onToggledState;
            };
        }

        /**
         * @brief Slider property group definitions.
         */
        namespace Slider {

            /**
             * @brief Slider layout property group definitions.
             */
            struct Layout : public SFUI::PropGroup::Component::Layout {

            };

            /**
             * @brief Slider style property group definitions.
             */
            struct Style : public SFUI::PropGroup::Component::Style {

                /**
                 * @brief Shadow offset of the slider.
                 */
                SFUI::Prop::Duplex shadowOffset = {0.0f, 0.0f};

                /**
                 * @brief Shadow radius of the slider.
                 */
                SFUI::Prop::Numeric shadowRadius = 1.0f;

                /**
                 * @brief Shadow fill color of the slider.
                 */
                SFUI::Prop::Color shadowFillColor = SFUI::Color(0, 0, 0, 0);

                /**
                 * @brief Alignment of the slider track.
                 */
                SFUI::Prop::Keyword trackAlign = "horizontal";

                /**
                 * @brief Width of the slider track.
                 */
                SFUI::Prop::Dimension trackWidth = 8.0f;

                /**
                 * @brief Corner radius of the slider track.
                 */
                SFUI::Prop::UniQuad trackCornerRadius = 4.0f;

                /**
                 * @brief Optional width of the unprogressed portion of the track.
                 */
                SFUI::Optional<SFUI::Prop::Dimension> trackProgressedWidth;

                /**
                 * @brief Width of the slider thumb.
                 */
                SFUI::Prop::Dimension thumbWidth = 15.0f;

                /**
                 * @brief Height of the slider thumb.
                 */
                SFUI::Prop::Dimension thumbHeight = 15.0f;

                /**
                 * @brief Corner radius of the slider thumb.
                 */
                SFUI::Prop::UniQuad thumbCornerRadius = 7.5f;

                /**
                 * @brief Fill color of the slider track.
                 */
                SFUI::Prop::Color trackFillColor = SFUI::Color(175, 175, 175, 255);

                /**
                 * @brief Border color of the slider track.
                 */
                SFUI::Prop::Color trackBorderColor = SFUI::Color(175, 175, 175, 255);

                /**
                 * @brief Fill color of the slider track when hovered.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackUnprogressedFillColor;

                /**
                 * @brief Border color of the slider track when hovered.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackUnprogressedBorderColor;

                /**
                 * @brief Fill color of the slider track when pressed.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackUnprogressedHoveredFillColor;

                /**
                 * @brief Border color of the slider track when pressed.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackUnprogressedHoveredBorderColor;

                /**
                 * @brief Fill color of the slider track when pressed.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackUnprogressedPressedFillColor;

                /**
                 * @brief Border color of the slider track when pressed.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackUnprogressedPressedBorderColor;

                /**
                 * @brief Fill color of the progressed portion of the slider track.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackProgressedFillColor;

                /**
                 * @brief Border color of the progressed portion of the slider track.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackProgressedBorderColor;

                /**
                 * @brief Fill color of the progressed portion of the slider track when hovered.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackProgressedHoveredFillColor;

                /**
                 * @brief Border color of the progressed portion of the slider track when hovered.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackProgressedHoveredBorderColor;

                /**
                 * @brief Fill color of the progressed portion of the slider track when pressed.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackProgressedPressedFillColor;

                /**
                 * @brief Border color of the progressed portion of the slider track when pressed.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackProgressedPressedBorderColor;

                /**
                 * @brief Fill color of the slider track when disabled.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackUnprogressedDisabledFillColor;

                /**
                 * @brief Border color of the slider track when disabled.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackUnprogressedDisabledBorderColor;

                /**
                 * @brief Fill color of the progressed portion of the slider track when disabled.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackProgressedDisabledFillColor;

                /**
                 * @brief Border color of the progressed portion of the slider track when disabled.
                 */
                SFUI::Optional<SFUI::Prop::Color> trackProgressedDisabledBorderColor;

                /**
                 * @brief Fill color of the slider thumb.
                 */
                SFUI::Prop::Color thumbFillColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Border color of the slider thumb.
                 */
                SFUI::Prop::Color thumbBorderColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Fill color of the slider thumb when hovered.
                 */
                SFUI::Optional<SFUI::Prop::Color> thumbHoveredFillColor;

                /**
                 * @brief Border color of the slider thumb when hovered.
                 */
                SFUI::Optional<SFUI::Prop::Color> thumbHoveredBorderColor;

                /**
                 * @brief Fill color of the slider thumb when pressed.
                 */
                SFUI::Optional<SFUI::Prop::Color> thumbPressedFillColor;

                /**
                 * @brief Border color of the slider thumb when pressed.
                 */
                SFUI::Optional<SFUI::Prop::Color> thumbPressedBorderColor;

                /**
                 * @brief Fill color of the slider thumb when disabled.
                 */
                SFUI::Optional<SFUI::Prop::Color> thumbDisabledFillColor;

                /**
                 * @brief Border color of the slider thumb when disabled.
                 */
                SFUI::Optional<SFUI::Prop::Color> thumbDisabledBorderColor;

                /**
                 * @brief Width of the focus outline.
                 */
                SFUI::Prop::Dimension focusWidth = 5.0f;

                /**
                 * @brief Offset of the focus outline.
                 */
                SFUI::Prop::Dimension focusOffset = 0.0f;

                /**
                 * @brief Corner radius of the focus outline.
                 */
                SFUI::Prop::UniQuad focusCornerRadius;

                /**
                 * @brief Fill color of the focus outline.
                 */
                SFUI::Prop::Color focusFillColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Padding for the tooltip.
                 */
                SFUI::Prop::UniQuad toolTipPadding = 10.0f;

                /**
                 * @brief Corner radius for the tooltip.
                 */
                SFUI::Prop::UniQuad toolTipCornerRadius;

                /**
                 * @brief Text content of the tooltip.
                 */
                SFUI::Prop::Keyword toolTipText = "";

                /**
                 * @brief Font used for the tooltip text.
                 */
                SFUI::Prop::Font toolTipFont = std::make_shared<SFUI::Font>();

                /**
                 * @brief Size of the tooltip text.
                 */
                SFUI::Prop::Numeric toolTipTextSize = 10.0f;

                /**
                 * @brief Fill color of the tooltip.
                 */
                SFUI::Prop::Color toolTipFillColor = SFUI::Color(150, 150, 150, 255);

                /**
                 * @brief Text color of the tooltip.
                 */
                SFUI::Prop::Color toolTipTextColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Equality operator for Slider style properties.
                 */
                bool operator==(const Style&) const = default;

                /**
                 * @brief Inequality operator for Slider style properties.
                 */
                bool operator!=(const Style&) const = default;
            };

            /**
             * @brief Slider state property group definitions.
             */
            struct State : public SFUI::PropGroup::Component::State {

                /**
                 * @brief Indicates whether the slider is disabled.
                 */
                SFUI::Prop::Binary isDisabled = false;

                /**
                 * @brief Indicates whether the slider is currently focused.
                 */
                SFUI::Prop::Binary isFocused = false;

                /**
                 * @brief Indicates whether the slider's direction is inverted.
                 */
                SFUI::Prop::Binary isInverted = false;

                /**
                 * @brief Current value of the slider.
                 */
                SFUI::Prop::Numeric value = 0.0f;

                /**
                 * @brief Minimum value of the slider.
                 */
                SFUI::Prop::Numeric minimumValue = 0.0f;

                /**
                 * @brief Maximum value of the slider.
                 */
                SFUI::Prop::Numeric maximumValue = 20.0f;

                /**
                 * @brief Step increment for the slider value.
                 */
                SFUI::Prop::Numeric step = 1.0f;

                /**
                 * @brief Equality operator for Slider state properties.
                 */
                bool operator==(const State&) const = default;

                /**
                 * @brief Inequality operator for Slider state properties.
                 */
                bool operator!=(const State&) const = default;
            };

            /**
             * @brief Slider behavior property group definitions.
             */
            struct Behavior : public SFUI::PropGroup::Component::Behavior {

                /**
                 * @brief Callback function triggered when the slider is enabled.
                 */
                SFUI::Prop::Callback onEnable;

                /**
                 * @brief Callback function triggered when the slider is disabled.
                 */
                SFUI::Prop::Callback onDisable;

                /**
                 * @brief Callback function triggered when the slider gains focus.
                 */
                SFUI::Prop::Callback onFocus;

                /**
                 * @brief Callback function triggered when the slider loses focus.
                 */
                SFUI::Prop::Callback onBlur;

                /**
                 * @brief Callback function triggered when the mouse hovers over the slider track.
                 */
                SFUI::Prop::Callback onTrackHoverIn;

                /**
                 * @brief Callback function triggered when the mouse stops hovering over the slider track.
                 */
                SFUI::Prop::Callback onTrackHoverOut;

                /**
                 * @brief Callback function triggered when the left mouse button is pressed down on the slider track.
                 */
                SFUI::Prop::Callback onTrackLeftPressIn;

                /**
                 * @brief Callback function triggered when the left mouse button is released on the slider track.
                 */
                SFUI::Prop::Callback onTrackLeftPress;

                /**
                 * @brief Callback function triggered when the right mouse button is pressed down on the slider track.
                 */
                SFUI::Prop::Callback onTrackRightPressIn;

                /**
                 * @brief Callback function triggered when the right mouse button is released on the slider track.
                 */
                SFUI::Prop::Callback onTrackRightPress;

                /**
                 * @brief Callback function triggered when the middle mouse button is pressed down on the slider track.
                 */
                SFUI::Prop::Callback onTrackMiddlePressIn;

                /**
                 * @brief Callback function triggered when the middle mouse button is released on the slider track.
                 */
                SFUI::Prop::Callback onTrackMiddlePress;

                /**
                 * @brief Callback function triggered when the mouse hovers over the slider thumb.
                 */
                SFUI::Prop::Callback onThumbHoverIn;

                /**
                 * @brief Callback function triggered when the mouse stops hovering over the slider thumb.
                 */
                SFUI::Prop::Callback onThumbHoverOut;

                /**
                 * @brief Callback function triggered when the left mouse button is pressed down on the slider thumb.
                 */
                SFUI::Prop::Callback onThumbLeftPressIn;

                /**
                 * @brief Callback function triggered when the left mouse button is released on the slider thumb.
                 */
                SFUI::Prop::Callback onThumbLeftPress;

                /**
                 * @brief Callback function triggered when the right mouse button is pressed down on the slider thumb.
                 */
                SFUI::Prop::Callback onThumbRightPressIn;

                /**
                 * @brief Callback function triggered when the right mouse button is released on the slider thumb.
                 */
                SFUI::Prop::Callback onThumbRightPress;

                /**
                 * @brief Callback function triggered when the middle mouse button is pressed down on the slider thumb.
                 */
                SFUI::Prop::Callback onThumbMiddlePressIn;

                /**
                 * @brief Callback function triggered when the middle mouse button is released on the slider thumb.
                 */
                SFUI::Prop::Callback onThumbMiddlePress;

                /**
                 * @brief Callback function triggered when the slider is double-clicked.
                 */
                SFUI::Prop::CallbackKey onKeyPress;

                /**
                 * @brief Callback function triggered when sliding starts.
                 */
                SFUI::Prop::CallbackNumeric onSlidingStart;

                /**
                 * @brief Callback function triggered when sliding ends.
                 */
                SFUI::Prop::CallbackNumeric onSlidingEnd;

                /**
                 * @brief Callback function triggered when the slider value changes.
                 */
                SFUI::Prop::CallbackNumeric onValueChange;
            };
        }

        /**
         * @brief TextField property group definitions.
         */
        namespace TextField {

            /**
             * @brief Text Field layout property group definitions.
             */
            struct Layout : public SFUI::PropGroup::Component::Layout {

            };

            /**
             * @brief Text Field style property group definitions.
             */
            struct Style : public SFUI::PropGroup::Component::Style {

                /**
                 * @brief Line mode of the text field.
                 */
                SFUI::Prop::Keyword lineMode = "single";

                /**
                 * @brief Inset padding for the text content.
                 */
                SFUI::Prop::UniQuad textInset = 0.0f;

                /**
                 * @brief Text content of the text field.
                 */
                SFUI::Prop::Keyword text;

                /**
                 * @brief Placeholder text when the field is empty.
                 */
                SFUI::Prop::Keyword placeholderText;

                /**
                 * @brief Font used for the text field.
                 */
                SFUI::Prop::Font font = std::make_shared<SFUI::Font>();

                /**
                 * @brief Size of the text field text.
                 */
                SFUI::Prop::Numeric textSize = 12.0f;

                /**
                 * @brief Style of the text field text.
                 */
                SFUI::Prop::TextStyle textStyle = "regular";

                /**
                 * @brief Optional letter spacing for the text field text.
                 */
                SFUI::Optional<SFUI::Prop::Numeric> letterSpacing;

                /**
                 * @brief Optional line spacing for the text field text.
                 */
                SFUI::Optional<SFUI::Prop::Numeric> lineSpacing;

                /**
                 * @brief Thickness of the text outline.
                 */
                SFUI::Prop::Numeric textOutlineThickness = 0.0f;

                /**
                 * @brief Horizontal alignment of the text field text.
                 */
                SFUI::Prop::Keyword textAlignHorizontal = "left";

                /**
                 * @brief Vertical alignment of the text field text.
                 */
                SFUI::Prop::Color textColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Color of the text outline.
                 */
                SFUI::Prop::Color textOutlineColor = SFUI::Color(255, 255, 255, 255);

                /**
                 * @brief Color of the placeholder text.
                 */
                SFUI::Prop::Color placeholderTextColor = SFUI::Color(100, 100, 100, 255);

                /**
                 * @brief Width of the focus outline.
                 */
                SFUI::Optional<SFUI::Prop::Color> disabledFillColor;

                /**
                 * @brief Border color when the text field is disabled.
                 */
                SFUI::Optional<SFUI::Prop::Color> disabledBorderColor;

                /**
                 * @brief Width of the focus outline.
                 */
                SFUI::Prop::UniQuad toolTipPadding = 10.0f;

                /**
                 * @brief Corner radius for the tooltip.
                 */
                SFUI::Prop::UniQuad toolTipCornerRadius;

                /**
                 * @brief Text content of the tooltip.
                 */
                SFUI::Prop::Keyword toolTipText = "";

                /**
                 * @brief Font used for the tooltip text.
                 */
                SFUI::Prop::Font toolTipFont = std::make_shared<SFUI::Font>();

                /**
                 * @brief Size of the tooltip text.
                 */
                SFUI::Prop::Numeric toolTipTextSize = 10.0f;

                /**
                 * @brief Fill color of the tooltip.
                 */
                SFUI::Prop::Color toolTipFillColor = SFUI::Color(150, 150, 150, 255);

                /**
                 * @brief Text color of the tooltip.
                 */
                SFUI::Prop::Color toolTipTextColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Shape of the caret.
                 */
                SFUI::Prop::Keyword caretShape = "line";

                /**
                 * @brief Width of the caret.
                 */
                SFUI::Prop::Numeric caretBlinkTime = 500.0f;

                /**
                 * @brief Blink ratio of the caret.
                 */
                SFUI::Prop::Numeric caretBlinkRatio = 1.0f;

                /**
                 * @brief Color of the caret.
                 */
                SFUI::Prop::Color caretFillColor = SFUI::Color(0, 0, 0, 255);

                /**
                 * @brief Equality operator for TextField style properties.
                 */
                bool operator==(const Style&) const = default;

                /**
                 * @brief Inequality operator for TextField style properties.
                 */
                bool operator!=(const Style&) const = default;
            };

            /**
             * @brief Text Field state property group definitions.
             */
            struct State : public SFUI::PropGroup::Component::State {

                /**
                 * @brief Indicates whether the text field is disabled.
                 */
                SFUI::Prop::Binary isDisabled = false;

                /**
                 * @brief Indicates whether the text field is currently focused.
                 */
                SFUI::Prop::Binary isFocused = false;

                /**
                 * @brief Current text content of the text field.
                 */
                bool operator==(const State&) const = default;

                /**
                 * @brief Inequality operator for TextField state properties.
                 */
                bool operator!=(const State&) const = default;
            };

            /**
             * @brief Text Field behavior property group definitions.
             */
            struct Behavior : public SFUI::PropGroup::Component::Behavior {

                /**
                 * @brief Callback function triggered when the text field is enabled.
                 */
                SFUI::Prop::Callback onEnable;

                /**
                 * @brief Callback function triggered when the text field is disabled.
                 */
                SFUI::Prop::Callback onDisable;

                /**
                 * @brief Callback function triggered when the text field gains focus.
                 */
                SFUI::Prop::Callback onFocus;

                /**
                 * @brief Callback function triggered when the text field loses focus.
                 */
                SFUI::Prop::Callback onBlur;

                /**
                 * @brief Callback function triggered when the mouse hovers over the text field.
                 */
                SFUI::Prop::Callback onHoverIn;

                /**
                 * @brief Callback function triggered when the mouse stops hovering over the text field.
                 */
                SFUI::Prop::Callback onHoverOut;

                /**
                 * @brief Callback function triggered when the left mouse button is pressed down on the text field.
                 */
                SFUI::Prop::Callback onLeftPressIn;

                /**
                 * @brief Callback function triggered when the left mouse button is released on the text field.
                 */
                SFUI::Prop::Callback onLeftPress;

                /**
                 * @brief Callback function triggered when the right mouse button is pressed down on the text field.
                 */
                SFUI::Prop::Callback onRightPressIn;

                /**
                 * @brief Callback function triggered when the right mouse button is released on the text field.
                 */
                SFUI::Prop::Callback onRightPress;

                /**
                 * @brief Callback function triggered when the middle mouse button is pressed down on the text field.
                 */
                SFUI::Prop::Callback onMiddlePressIn;

                /**
                 * @brief Callback function triggered when the middle mouse button is released on the text field.
                 */
                SFUI::Prop::Callback onMiddlePress;

                /**
                 * @brief Callback function triggered when the text field is double-clicked.
                 */
                SFUI::Prop::Callback onDoublePress;

                /**
                 * @brief Callback function triggered when the mouse button is released outside the text field.
                 */
                SFUI::Prop::Callback onPressOut;

                /**
                 * @brief Callback function triggered when a key is pressed while the text field is focused.
                 */
                SFUI::Prop::CallbackKey onKeyPress;

                /**
                 * @brief Callback function triggered when the text in the text field changes.
                 */
                SFUI::Prop::CallbackKeyword onTextChange;

                /**
                 * @brief Callback function triggered when the text field is submitted (e.g., Enter key pressed).
                 */
                SFUI::Prop::CallbackKeyword onSubmit;
            };
        }
    }
}
