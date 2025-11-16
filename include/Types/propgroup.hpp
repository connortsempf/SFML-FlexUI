/**
 * @file propgroup.hpp
 * @brief Defines grouped property structures for specific SFUI component types.
 * @author Connor Sempf
 * @date 2025-11-15
 * @version 1.0
 *
 * This file organizes component-specific property sets (layout,
 * style, state, behavior) for SFUI components such as Button,
 * Slider, Toggle, TextField, and others.
 */


#pragma once
#include "prop.hpp"


namespace SFUI {

    /**
     * @namespace PropGroup
     * @brief Defines grouped property structures for each SFUI component type.
     *
     * Each struct in this namespace represents the full set of layout, style,
     * state, and behavior sub-properties for a specific component type.
     */
    namespace PropGroup {

        /**
         * @brief Property group for a generic container component.
         */
        struct Container {
            /**
             * @brief Layout properties for the component.
             */
            SFUI::Prop::Layout::Component layout;

            /**
             * @brief Style properties for the component.
             */
            SFUI::Prop::Style::Component style;
        };

        /**
         * @brief Property group for a Label component.
         */
        struct Label {
            /**
             * @brief Layout properties for the component.
             */
            SFUI::Prop::Layout::Component layout;

            /**
             * @brief Style properties for the component.
             */
            SFUI::Prop::Style::Component style;
            
            /**
             * @brief Label-specific style properties.
             */
            SFUI::Prop::Style::Label labelStyle;
        };

        /**
         * @brief Property group for a Button component.
         */
        struct Button {
            /**
             * @brief Layout properties for the component.
             */
            SFUI::Prop::Layout::Component layout;
            
            /**
             * @brief Style properties for the component.
             */
            SFUI::Prop::Style::Component style;
            
            /**
             * @brief Button-specific style properties.
             */
            SFUI::Prop::Style::Button buttonStyle;
            
            /**
             * @brief Button-specific state properties.
             */
            SFUI::Prop::State::Button buttonState;
            
            /**
             * @brief Button-specific behavior properties.
             */
            SFUI::Prop::Behavior::Button buttonBehavior;
        };

        /**
         * @brief Property group for a Toggle component.
         */
        struct Toggle {
            /**
             * @brief Layout properties for the component.
             */
            SFUI::Prop::Layout::Component layout;
            
            /**
             * @brief Style properties for the component.
             */
            SFUI::Prop::Style::Component style;
            
            /**
             * @brief Toggle-specific style properties.
             */
            SFUI::Prop::Style::Toggle toggleStyle;
            
            /**
             * @brief Toggle-specific state properties.
             */
            SFUI::Prop::State::Toggle toggleState;
            
            /**
             * @brief Toggle-specific behavior properties.
             */
            SFUI::Prop::Behavior::Toggle toggleBehavior;
        };

        /**
         * @brief Property group for a Graphic component.
         */
        struct Graphic {
            /**
             * @brief Layout properties for the component.
             */
            SFUI::Prop::Layout::Component layout;
            
            /**
             * @brief Style properties for the component.
             */
            SFUI::Prop::Style::Component style;
            
            /**
             * @brief Graphic-specific style properties.
             */
            SFUI::Prop::Style::Graphic graphicStyle;
            
            /**
             * @brief Graphic-specific behavior properties.
             */
            SFUI::Prop::Behavior::Graphic graphicBehavior;
        };

        /**
         * @brief Property group for a ScrollContainer component.
         */
        struct ScrollContainer {
            /**
             * @brief Layout properties for the component.
             */
            SFUI::Prop::Layout::Component layout;
            
            /**
             * @brief Style properties for the component.
             */
            SFUI::Prop::Style::Component style;
            
            /**
             * @brief ScrollContainer-specific style properties.
             */
            SFUI::Prop::Style::ScrollContainer scrollContainerStyle;
            
            /**
             * @brief ScrollContainer-specific behavior properties.
             */
            SFUI::Prop::Behavior::ScrollContainer scrollContainerBehavior;
        };

        /**
         * @brief Property group for a TextField component.
         */
        struct TextField {
            /**
             * @brief Layout properties for the component.
             */
            SFUI::Prop::Layout::Component layout;
            
            /**
             * @brief Style properties for the component.
             */
            SFUI::Prop::Style::Component style;
            
            /**
             * @brief TextField-specific style properties.
             */
            SFUI::Prop::Style::TextField textFieldStyle;
            
            /**
             * @brief TextField-specific state properties.
             */
            SFUI::Prop::State::TextField textFieldState;
            
            /**
             * @brief TextField-specific behavior properties.
             */
            SFUI::Prop::Behavior::TextField textFieldBehavior;
        };

        /**
         * @brief Property group for a Slider component.
         */
        struct Slider {
            /**
             * @brief Layout properties for the component.
             */
            SFUI::Prop::Layout::Component layout;
            
            /**
             * @brief Style properties for the component.
             */
            SFUI::Prop::Style::Component style;
            
            /**
             * @brief Slider-specific style properties.
             */
            SFUI::Prop::Style::Slider sliderStyle;
            
            /**
             * @brief Slider-specific state properties.
             */
            SFUI::Prop::State::Slider sliderState;
            
            /**
             * @brief Slider-specific behavior properties.
             */
            SFUI::Prop::Behavior::Slider sliderBehavior;
        };
    }
}
