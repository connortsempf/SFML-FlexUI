/**
 * @file computedprop.hpp
 * @brief Defines computed properties for SFUI components.
 * @author Connor Sempf
 * @date 2025-11-15
 * @version 1.0
 *
 * This file contains structures and helpers that store calculated
 * layout and style values for SFUI components after processing
 * raw property inputs.
 */


#pragma once
#include "base.hpp"
#include "vector.hpp"


namespace SFUI {

    /**
     * @namespace ComputedProp
     * @brief Defines "computed properties" used by SFUI components for layout and style.
     *
     * These types represent properties that are derived or calculated
     * based on other property values, rather than being directly set.
     */
    namespace ComputedProp {

        /**
         * @brief Style-related computed properties for components.
         *
         * This namespace contains structs and types that define
         * computed style properties for components, such as
         * specific style configurations derived from base styles.
         */
        namespace Style {
    
            /**
             * @brief Style computed properties specific to ScrollContainer components.
             */
            struct ScrollContainer {
                /**
                 * @brief Scroll direction of the container.
                 */
                SFUI::String scrollDirection;

                /**
                 * @brief Scroll speed factor of the container.
                 */
                SFUI::Float scrollSpeedFactor;
            };

            /**
             * @brief Style computed properties specific to Label components.
             */
            struct Graphic {
                /**
                 * @brief Path to the graphic resource.
                 */
                SFUI::String graphicPath;
                
                /**
                 * @brief Original size of the loaded texture.
                 */
                SFUI::Vector2f originalTextureSize;
            };
    
            /**
             * @brief Style computed properties specific to TextField components.
             */
            struct TextField {
                /**
                 * @brief Line mode of the text field.
                 */
                SFUI::String lineMode;
            };

            /**
             * @brief Style computed properties specific to Slider components.
             */
            struct Slider {
                /**
                 * @brief Track alignment of the slider.
                 */
                SFUI::String trackAlign;
                
                /**
                 * @brief Length of the slider track.
                 */
                SFUI::Float trackLength;
                
                /**
                 * @brief Size of the slider thumb.
                 */
                SFUI::Vector2f thumbSize;
            };
        }
    }
}
