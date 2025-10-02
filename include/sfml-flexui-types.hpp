#pragma once




#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cctype>
#include <variant>
#include <memory>
#include <functional>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>




////////////////////////
// Debug Mode Logging //
////////////////////////

#if SFUI_DEBUG
    #define DEBUG_PRINT(x) do { std::cout << x << "\n"; } while(0)
#else
    #define DEBUG_PRINT(x) do {} while(0)
#endif




///////////////////////////
// Standard Type Aliases //
///////////////////////////

namespace SFUI {

    using Void = void;

    using Bool = bool;

    using UnsignedInt8 = uint8_t;

    using UnsignedInt = unsigned int;

    using Int = int;

    using Float = float;

    using Double = double;

    using String = std::string;

    template <typename... T>
    using Variant = std::variant<T ...>;

    template <typename... T>
    using Optional = std::optional<T ...>;

    template <typename... T>
    using Vector = std::vector<T ...>;

    template <typename... T>
    using Array = std::array<T ...>;

    template <typename... T>
    using WeakPointer = std::weak_ptr<T ...>;

    template <typename... T>
    using SharedPointer = std::shared_ptr<T ...>;

    template <typename... T>
    using UniquePointer = std::unique_ptr<T ...>;
}




/////////////////////////////////////////////
// Rendering Framework (SFML) Type Aliases //
/////////////////////////////////////////////

namespace SFUI {

    using Color = sf::Color;
    
    using Text = sf::Text;
    
    using Font = sf::Font;
    
    using Clock = sf::Clock;
    
    using Time = sf::Time;
    
    using Texture = sf::Texture;
    
    using RenderTexture = sf::RenderTexture;
    
    using Sprite = sf::Sprite;
    
    using RectangleShape = sf::RectangleShape;
    
    using CircleShape = sf::CircleShape;
    
    using VertexArray = sf::VertexArray;

    using Event = sf::Event;

    using RenderTarget = sf::RenderTarget;

    using VertexArray = sf::VertexArray;

    using PrimitiveType = sf::PrimitiveType;
}




/////////////////////////////////////
// Custom SFML-FlexUI Vector Types //
/////////////////////////////////////

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