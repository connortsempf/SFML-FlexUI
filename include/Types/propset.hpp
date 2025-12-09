/**
 * @file propset.hpp
 * @brief Defines grouped property set structures for specific SFUI component types.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.1
 *
 * This file organizes component-specific property group sets (layout,
 * style, state, behavior) for SFUI components such as Button,
 * Slider, Toggle, TextField, and others.
 */


#pragma once
#include "prop.hpp"
#include "propgroup.hpp"


namespace SFUI {

    /**
     * @brief Defines grouped property-group set structures for each SFUI component type.
     *
     * Each struct in this namespace represents the full set of layout, style,
     * state, and behavior sub-properties for a specific component type.
     */
    namespace PropSet {

        /**
         * @brief Component property-group sets.
         */
        struct Component {

            /**
             * @brief The layout prop-group of the property set for this component.
             */
            SFUI::PropGroup::Component::Layout layout;

            /**
             * @brief The style prop-group of the property set for this component.
             */
            SFUI::PropGroup::Component::Style style;

            /**
             * @brief The state prop-group of the property set for this component.
             */
            SFUI::PropGroup::Component::State state;

            /**
             * @brief The behavior prop-group of the property set for this component.
             */
            SFUI::PropGroup::Component::Behavior behavior;
        };

        /**
         * @brief Container property-group sets.
         */
        struct Container {

            /**
             * @brief The layout prop-group of the property set for this component.
             */
            SFUI::PropGroup::Container::Layout layout;

            /**
             * @brief The style prop-group of the property set for this component.
             */
            SFUI::PropGroup::Container::Style style;

            /**
             * @brief The state prop-group of the property set for this component.
             */
            SFUI::PropGroup::Container::State state;

            /**
             * @brief The behavior prop-group of the property set for this component.
             */
            SFUI::PropGroup::Container::Behavior behavior;
        };

        /**
         * @brief ScrollContainer property-group sets.
         */
        struct ScrollContainer {

            /**
             * @brief The layout prop-group of the property set for this component.
             */
            SFUI::PropGroup::ScrollContainer::Layout layout;

            /**
             * @brief The style prop-group of the property set for this component.
             */
            SFUI::PropGroup::ScrollContainer::Style style;

            /**
             * @brief The state prop-group of the property set for this component.
             */
            SFUI::PropGroup::ScrollContainer::State state;

            /**
             * @brief The behavior prop-group of the property set for this component.
             */
            SFUI::PropGroup::ScrollContainer::Behavior behavior;
        };

        /**
         * @brief Label property-group sets.
         */
        struct Label {

            /**
             * @brief The layout prop-group of the property set for this component.
             */
            SFUI::PropGroup::Label::Layout layout;

            /**
             * @brief The style prop-group of the property set for this component.
             */
            SFUI::PropGroup::Label::Style style;

            /**
             * @brief The state prop-group of the property set for this component.
             */
            SFUI::PropGroup::Label::State state;

            /**
             * @brief The behavior prop-group of the property set for this component.
             */
            SFUI::PropGroup::Label::Behavior behavior;
        };

        /**
         * @brief Graphic property-group sets.
         */
        struct Graphic {

            /**
             * @brief The layout prop-group of the property set for this component.
             */
            SFUI::PropGroup::Graphic::Layout layout;

            /**
             * @brief The style prop-group of the property set for this component.
             */
            SFUI::PropGroup::Graphic::Style style;

            /**
             * @brief The state prop-group of the property set for this component.
             */
            SFUI::PropGroup::Graphic::State state;

            /**
             * @brief The behavior prop-group of the property set for this component.
             */
            SFUI::PropGroup::Graphic::Behavior behavior;
        };

        /**
         * @brief Button property-group sets.
         */
        struct Button {

            /**
             * @brief The layout prop-group of the property set for this component.
             */
            SFUI::PropGroup::Button::Layout layout;

            /**
             * @brief The style prop-group of the property set for this component.
             */
            SFUI::PropGroup::Button::Style style;

            /**
             * @brief The state prop-group of the property set for this component.
             */
            SFUI::PropGroup::Button::State state;

            /**
             * @brief The behavior prop-group of the property set for this component.
             */
            SFUI::PropGroup::Button::Behavior behavior;
        };

        /**
         * @brief Toggle property-group sets.
         */
        struct Toggle {

            /**
             * @brief The layout prop-group of the property set for this component.
             */
            SFUI::PropGroup::Toggle::Layout layout;

            /**
             * @brief The style prop-group of the property set for this component.
             */
            SFUI::PropGroup::Toggle::Style style;

            /**
             * @brief The state prop-group of the property set for this component.
             */
            SFUI::PropGroup::Toggle::State state;

            /**
             * @brief The behavior prop-group of the property set for this component.
             */
            SFUI::PropGroup::Toggle::Behavior behavior;
        };

        /**
         * @brief Slider property-group sets.
         */
        struct Slider {

            /**
             * @brief The layout prop-group of the property set for this component.
             */
            SFUI::PropGroup::Slider::Layout layout;

            /**
             * @brief The style prop-group of the property set for this component.
             */
            SFUI::PropGroup::Slider::Style style;

            /**
             * @brief The state prop-group of the property set for this component.
             */
            SFUI::PropGroup::Slider::State state;

            /**
             * @brief The behavior prop-group of the property set for this component.
             */
            SFUI::PropGroup::Slider::Behavior behavior;
        };

        /**
         * @brief Text Field property-group sets.
         */
        struct TextField {

            /**
             * @brief The layout prop-group of the property set for this component.
             */
            SFUI::PropGroup::TextField::Layout layout;

            /**
             * @brief The style prop-group of the property set for this component.
             */
            SFUI::PropGroup::TextField::Style style;

            /**
             * @brief The state prop-group of the property set for this component.
             */
            SFUI::PropGroup::TextField::State state;

            /**
             * @brief The behavior prop-group of the property set for this component.
             */
            SFUI::PropGroup::TextField::Behavior behavior;
        };
    }
}
