#pragma once
#include "base.hpp"
#include "render.hpp"




//////////////////////////////
// Custom SFML-FlexUI Types //
//////////////////////////////

namespace SFUI {

    struct Vector2u {
        SFUI::UnsignedInt x, y;
        Vector2u(
            SFUI::UnsignedInt x = 0,
            SFUI::UnsignedInt y = 0
        ) : x(x), y(y) {}
        operator sf::Vector2u() const {
            return sf::Vector2u{x, y};
        }
    };
    
    struct Vector2i {
        SFUI::Int x, y;
        Vector2i(
            SFUI::Int x = 0,
            SFUI::Int y = 0
        ) : x(x), y(y) {}
        operator sf::Vector2i() const {
            return sf::Vector2i{x, y};
        }
    };
    
    struct Vector2f {
        SFUI::Float x, y;
        Vector2f(
            SFUI::Float x = 0,
            SFUI::Float y = 0
        ) : x(x), y(y) {}
        operator sf::Vector2f() const {
            return sf::Vector2f{x, y};
        }
    };

    struct Vector3ui8 {
        SFUI::UnsignedInt8 x, y, z;
        Vector3ui8(
            SFUI::UnsignedInt8 x = 0,
            SFUI::UnsignedInt8 y = 0,
            SFUI::UnsignedInt8 z = 0
        ) : x(x), y(y), z(z) {}
    };

    struct Vector3u {
        SFUI::UnsignedInt x, y, z;
        Vector3u(
            SFUI::UnsignedInt x = 0,
            SFUI::UnsignedInt y = 0,
            SFUI::UnsignedInt z = 0
        ) : x(x), y(y), z(z) {}
    };
    
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
    };

    struct Vector4ui8 {
        SFUI::UnsignedInt8 x, y, z, w;
        Vector4ui8(
            SFUI::UnsignedInt8 x = 0,
            SFUI::UnsignedInt8 y = 0,
            SFUI::UnsignedInt8 z = 0,
            SFUI::UnsignedInt8 w = 0
        ) : x(x), y(y), z(z), w(w) {}
    };
    
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
    };

    struct Vector4u {
        SFUI::UnsignedInt x, y, z, w;
        Vector4u(
            SFUI::UnsignedInt x = 0,
            SFUI::UnsignedInt y = 0,
            SFUI::UnsignedInt z = 0,
            SFUI::UnsignedInt w = 0
        ) : x(x), y(y), z(z), w(w) {}
    };
    
    struct Vector4i {
        SFUI::Int x, y, z, w;
        Vector4i(
            SFUI::Int x = 0,
            SFUI::Int y = 0,
            SFUI::Int z = 0,
            SFUI::Int w = 0
        ) : x(x), y(y), z(z), w(w) {}
    };
    
    struct Vector4f {
        SFUI::Float x, y, z, w;
        Vector4f(
            SFUI::Float x = 0,
            SFUI::Float y = 0,
            SFUI::Float z = 0,
            SFUI::Float w = 0
        ) : x(x), y(y), z(z), w(w) {}
    };
}
