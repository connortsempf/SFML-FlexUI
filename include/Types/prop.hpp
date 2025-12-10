/**
 * @file prop.hpp
 * @brief Defines property types used in SFUI component propgroups.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.2
 *
 * This file provides basic property aliases and structures such
 * as Binary, Numeric, Keyword, Dimension, Color, Font, Texture,
 * and various Callback types for flexible component customization.
 */


#pragma once
#include "base.hpp"
#include "vector.hpp"


namespace SFUI {

    /**
     * @namespace Prop
     * @brief Defines "properties" used by SFUI component propgroups for layout, style, state, and behavior.
     *
     * These types are building blocks for the higher-level PropGroup structures.
     * Properties include simple types (binary, numeric, keyword), complex
     * variants (Dimension, Color, TextStyle), and callback function types.
     */
    namespace Prop {

        /**
         * @brief Boolean type used for component properties.
         */
        using Binary = SFUI::Bool;

        /**
         * @brief Numeric type used for component properties.
         */
        using Numeric = SFUI::Float;

        /**
         * @brief Keyword type used for component properties.
         */
        using Keyword = SFUI::String;

        /**
         * @brief Dimension type used for component properties.
         *
         * Can be either a float (absolute dimension) or a string (e.g., percentage).
         */
        using Dimension = SFUI::Variant<SFUI::Float, SFUI::String>;

        /**
         * @brief 4D vector of Dimension sub-properties.
         */
        struct Vector4dim {
            SFUI::Prop::Dimension x, y, z, w;
            Vector4dim(
                SFUI::Prop::Dimension x = 0.0f,
                SFUI::Prop::Dimension y = 0.0f,
                SFUI::Prop::Dimension z = 0.0f,
                SFUI::Prop::Dimension w = 0.0f
            ) : x(x), y(y), z(z), w(w) {}
            bool operator==(const Vector4dim& other) const {
                return (x == other.x && y == other.y && z == other.z && w == other.w);
            }
        };

        /**
         * @brief Text style type used for component properties.
         *
         * Can be either a string (e.g., "bold") or an unsigned integer (bitmask).
         */
        using TextStyle = SFUI::Variant<SFUI::String, SFUI::UnsignedInt32>;

        /**
         * @brief Color type used for component properties.
         *
         * Can be a Vector3ui8 (RGB), Vector4ui8 (RGBA), or a string (e.g., hex code).
         */
        using Color = SFUI::Variant<SFUI::Vector3ui8, SFUI::Vector4ui8, SFUI::String, SFUI::Color>;

        /**
         * @brief UniQuad type used for component properties.
         *
         * Can be either a Dimension or a Vector4dim.
         */
        using UniQuad = SFUI::Variant<SFUI::Prop::Dimension, SFUI::Prop::Vector4dim>;

        /**
         * @brief Duplex type used for component properties.
         *
         * A 2D vector of floats.
         */
        using Duplex = SFUI::Vector2f;

        /**
         * @brief Font type used for component properties.
         *
         * A shared pointer to an SFUI Font.
         */
        using Font = SFUI::SharedPointer<SFUI::Font>;

        /**
         * @brief Texture type used for component properties.
         *
         * A shared pointer to an SFUI Texture.
         */
        using Texture = SFUI::SharedPointer<SFUI::Texture>;

        /**
         * @brief Callback type used for component properties.
         *
         * Various function signatures for event callbacks.
         */
        using Callback = SFUI::Function<SFUI::Void(const SFUI::String&)>;

        /**
         * @brief Binary callback type used for component properties.
         *
         * Called with a string identifier and a boolean value.
         */
        using CallbackBinary = SFUI::Function<SFUI::Void(const SFUI::String&, const SFUI::Bool)>;

        /**
         * @brief Numeric callback type used for component properties.
         *
         * Called with a string identifier and a float value.
         */
        using CallbackNumeric = SFUI::Function<SFUI::Void(const SFUI::String&, const SFUI::Float)>;

        /**
         * @brief Keyword callback type used for component properties.
         *
         * Called with a string identifier and a keyword value.
         */
        using CallbackKeyword = SFUI::Function<SFUI::Void(const SFUI::String&, const SFUI::String&)>;

        /**
         * @brief Key callback type used for component properties.
         *
         * Called with a string identifier and a keyboard key value.
         */
        using CallbackKey = SFUI::Function<SFUI::Void(const SFUI::String&, sf::Keyboard::Key)>;
    }
}
