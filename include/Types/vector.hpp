/**
 * @file vector.hpp
 * @brief Defines vector structures (2D, 3D, 4D) used in SFUI for geometry, size, and color data.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.2
 *
 * This file contains structures like Vector2f, Vector3i, Vector4ui8, etc.,
 * with constructors, equality operators, and optional conversion to SFML types.
 */


#pragma once
#include "base.hpp"
#include "render.hpp"


namespace SFUI {

    /**
     * @brief Unsigned 2D vector type used throughout SFUI.
     *
     * Wraps two unsigned integers and provides an implicit conversion to
     * sf::Vector2u as well as equality comparison.
     */
    struct Vector2u {
        SFUI::UnsignedInt x, y;
        Vector2u(
            SFUI::UnsignedInt x = 0,
            SFUI::UnsignedInt y = 0
        ) : x(x), y(y) {}
        operator sf::Vector2u() const {
            return sf::Vector2u{x, y};
        }
        bool operator==(const Vector2u& other) const {
            return (x == other.x && y == other.y);
        }
    };

    /**
     * @brief Signed 2D vector type used throughout SFUI.
     *
     * Stores two integers and converts implicitly to sf::Vector2i.
     */
    struct Vector2i {
        SFUI::Int x, y;
        Vector2i(
            SFUI::Int x = 0,
            SFUI::Int y = 0
        ) : x(x), y(y) {}
        operator sf::Vector2i() const {
            return sf::Vector2i{x, y};
        }
        bool operator==(const Vector2i& other) const {
            return (x == other.x && y == other.y);
        }
    };

    /**
     * @brief Floating-point 2D vector type used by SFUI.
     *
     * Wraps two floats and converts implicitly to sf::Vector2f.
     */
    struct Vector2f {
        SFUI::Float x, y;
        Vector2f(
            SFUI::Float x = 0,
            SFUI::Float y = 0
        ) : x(x), y(y) {}
        operator sf::Vector2f() const {
            return sf::Vector2f{x, y};
        }
        bool operator==(const Vector2f& other) const {
            return (x == other.x && y == other.y);
        }
    };

    /**
     * @brief 3D vector of 8-bit unsigned integers.
     *
     * Useful for small RGB-style storage or compact data representations.
     */
    struct Vector3ui8 {
        SFUI::UnsignedInt8 x, y, z;
        Vector3ui8(
            SFUI::UnsignedInt8 x = 0,
            SFUI::UnsignedInt8 y = 0,
            SFUI::UnsignedInt8 z = 0
        ) : x(x), y(y), z(z) {}
        bool operator==(const Vector3ui8& other) const {
            return (x == other.x && y == other.y && z == other.z);
        }
    };

    /**
     * @brief 3D vector of unsigned integers.
     *
     * Used for integer-based 3D integer coordinates or sizes.
     */
    struct Vector3u {
        SFUI::UnsignedInt x, y, z;
        Vector3u(
            SFUI::UnsignedInt x = 0,
            SFUI::UnsignedInt y = 0,
            SFUI::UnsignedInt z = 0
        ) : x(x), y(y), z(z) {}
        bool operator==(const Vector3u& other) const {
            return (x == other.x && y == other.y && z == other.z);
        }
    };

    /**
     * @brief 3D vector of signed integers.
     *
     * Converts implicitly to sf::Vector3i and supports equality comparison.
     */
    struct Vector3i {
        SFUI::Int x, y, z;
        Vector3i(
            SFUI::Int x = 0,
            SFUI::Int y = 0,
            SFUI::Int z = 0
        ) : x(x), y(y), z(z) {}
        operator sf::Vector3i() const {
            return sf::Vector3i{x, y, z};
        }
        bool operator==(const Vector3i& other) const {
            return (x == other.x && y == other.y && z == other.z);
        }
    };

    /**
     * @brief 3D vector of floating-point values.
     *
     * Wraps three floats and converts implicitly to sf::Vector3f.
     */
    struct Vector3f {
        SFUI::Float x, y, z;
        Vector3f(
            SFUI::Float x = 0,
            SFUI::Float y = 0,
            SFUI::Float z = 0
        ) : x(x), y(y), z(z) {}
        operator sf::Vector3f() const {
            return sf::Vector3f{x, y, z};
        }
        bool operator==(const Vector3f& other) const {
            return (x == other.x && y == other.y && z == other.z);
        }
    };

    /**
     * @brief 4D vector of 8-bit unsigned integers.
     *
     * Useful for compact color or data packing (e.g., RGBA bytes).
     */
    struct Vector4ui8 {
        SFUI::UnsignedInt8 x, y, z, w;
        Vector4ui8(
            SFUI::UnsignedInt8 x = 0,
            SFUI::UnsignedInt8 y = 0,
            SFUI::UnsignedInt8 z = 0,
            SFUI::UnsignedInt8 w = 0
        ) : x(x), y(y), z(z), w(w) {}
        bool operator==(const Vector4ui8& other) const {
            return (x == other.x && y == other.y && z == other.z && w == other.w);
        }
    };

    /**
     * @brief 4D vector of unsigned integers.
     */
    struct Vector4u {
        SFUI::UnsignedInt x, y, z, w;
        Vector4u(
            SFUI::UnsignedInt x = 0,
            SFUI::UnsignedInt y = 0,
            SFUI::UnsignedInt z = 0,
            SFUI::UnsignedInt w = 0
        ) : x(x), y(y), z(z), w(w) {}
        bool operator==(const Vector4u& other) const {
            return (x == other.x && y == other.y && z == other.z && w == other.w);
        }
    };

    /**
     * @brief 4D vector of signed integers.
     */
    struct Vector4i {
        SFUI::Int x, y, z, w;
        Vector4i(
            SFUI::Int x = 0,
            SFUI::Int y = 0,
            SFUI::Int z = 0,
            SFUI::Int w = 0
        ) : x(x), y(y), z(z), w(w) {}
        bool operator==(const Vector4i& other) const {
            return (x == other.x && y == other.y && z == other.z && w == other.w);
        }
    };

    /**
     * @brief 4D vector of floating-point values.
     */
    struct Vector4f {
        SFUI::Float x, y, z, w;
        Vector4f(
            SFUI::Float x = 0,
            SFUI::Float y = 0,
            SFUI::Float z = 0,
            SFUI::Float w = 0
        ) : x(x), y(y), z(z), w(w) {}
        bool operator==(const Vector4f& other) const {
            return (x == other.x && y == other.y && z == other.z && w == other.w);
        }
    };
}
