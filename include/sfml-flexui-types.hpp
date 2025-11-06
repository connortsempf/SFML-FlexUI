#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <functional>
#include <algorithm>
#include <variant>
#include <string>
#include <cctype>
#include <vector>
#include <queue>
#include <stack>




///////////////////////////
// Standard Type Aliases //
///////////////////////////

namespace SFUI {

    using Void = void;

    using Bool = bool;

    using UnsignedInt8 = uint8_t;

    using UnsignedInt32 = uint32_t;

    using UnsignedInt = unsigned int;

    using Int = int;

    using Float = float;

    using Double = double;

    using Size = std::size_t;

    using String = std::string;

    template <typename... T>
    using Optional = std::optional<T ...>;

    template <typename... T>
    using Function = std::function<T ...>;

    template <typename... T>
    using Variant = std::variant<T ...>;

    template <typename... T>
    using Vector = std::vector<T ...>;

    template <typename T, std::size_t N>
    using Array = std::array<T, N>;

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

    using FloatRect = sf::FloatRect;

    using Color = sf::Color;
    
    using Text = sf::Text;
    
    using Font = sf::Font;
    
    using Clock = sf::Clock;
    
    using Time = sf::Time;

    using Event = sf::Event;

    using RectangleShape = sf::RectangleShape;
    
    using CircleShape = sf::CircleShape;

    using Cursor = sf::Cursor;
    
    using RenderTarget = sf::RenderTarget;

    using RenderWindow = sf::RenderWindow;

    using Image = sf::Image;
    
    using Texture = sf::Texture;
    
    using Sprite = sf::Sprite;
    
    using RenderTexture = sf::RenderTexture;
    
    using PrimitiveType = sf::PrimitiveType;

    using VertexArray = sf::VertexArray;

    using Shader = sf::Shader;
}




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




/////////////////////////////////////
// Custom Component Sub-Prop Types //
/////////////////////////////////////

namespace SFUI {

    namespace SubProp {

        using Binary = SFUI::Bool;
        
        using Numeric = SFUI::Float;

        using Keyword = SFUI::String;

        using Dimension = SFUI::Variant<SFUI::Float, SFUI::String>;

        struct Vector4dim {
            SFUI::SubProp::Dimension x, y, z, w;
            Vector4dim(
                SFUI::SubProp::Dimension x = 0.0f,
                SFUI::SubProp::Dimension y = 0.0f,
                SFUI::SubProp::Dimension z = 0.0f,
                SFUI::SubProp::Dimension w = 0.0f
            ) : x(x), y(y), z(z), w(w) {}
        };

        using TextStyle = SFUI::Variant<SFUI::String, SFUI::UnsignedInt32>;

        using Color = SFUI::Variant<SFUI::Vector3ui8, SFUI::Vector4ui8, SFUI::String, SFUI::Color>;
        
        using UniQuad = SFUI::Variant<SFUI::SubProp::Dimension, SFUI::SubProp::Vector4dim>;

        using Duplex = SFUI::Vector2f;

        using Font = SFUI::SharedPointer<SFUI::Font>;

        using Texture = SFUI::SharedPointer<SFUI::Texture>;

        using Callback = SFUI::Function<SFUI::Void(const SFUI::String&)>;
        
        using CallbackBinary = SFUI::Function<SFUI::Void(const SFUI::String&, const SFUI::Bool)>;
        
        using CallbackNumeric = SFUI::Function<SFUI::Void(const SFUI::String&, const SFUI::Float)>;
        
        using CallbackKeyword = SFUI::Function<SFUI::Void(const SFUI::String&, const SFUI::String&)>;

        using CallbackKey = SFUI::Function<SFUI::Void(const SFUI::String&, sf::Keyboard::Key)>;
    }
}




//////////////////////////////////////
// Custom Core Component Prop Types //
//////////////////////////////////////

namespace SFUI {

    namespace Prop {

        // Layout Props //
        namespace Layout {

            struct Component {
                SFUI::SubProp::Keyword alignDirection = "vertical";
                SFUI::SubProp::Keyword alignPrimary = "start";
                SFUI::SubProp::Keyword alignSecondary = "start";
                SFUI::SubProp::Dimension width = 0.0f;
                SFUI::SubProp::Dimension height = 0.0f;
                SFUI::SubProp::UniQuad padding = 0.0f;
                SFUI::SubProp::UniQuad margin = 0.0f;
                SFUI::Optional<SFUI::SubProp::Numeric> xPosition;
                SFUI::Optional<SFUI::SubProp::Numeric> yPosition;
            };
        }

        // Style Props //
        namespace Style {

            struct Component {
                SFUI::SubProp::Dimension borderWidth = 0.0f;
                SFUI::SubProp::UniQuad cornerRadius;
                SFUI::SubProp::Color fillColor = SFUI::Color(0, 0, 0, 0);
                SFUI::SubProp::Color borderColor = SFUI::Color(0, 0, 0, 0);
                SFUI::SubProp::Duplex shadowOffset = {0.0f, 0.0f};
                SFUI::SubProp::Numeric shadowRadius = 1.0f;
                SFUI::SubProp::Color shadowFillColor = SFUI::Color(0, 0, 0, 0);
            };

            struct ScrollContainer {
                SFUI::SubProp::Keyword scrollDirection = "vertical";
                SFUI::SubProp::Numeric scrollSpeedFactor = 15.0f;
            };
            
            struct Label {
                SFUI::SubProp::Keyword text;
                SFUI::SubProp::Font font = nullptr;
                SFUI::SubProp::Numeric textSize = 12.0f;
                SFUI::SubProp::TextStyle textStyle = "regular";
                SFUI::Optional<SFUI::SubProp::Numeric> letterSpacing;
                SFUI::Optional<SFUI::SubProp::Numeric> lineSpacing;
                SFUI::SubProp::Numeric textOutlineThickness = 0.0f;
                SFUI::SubProp::Keyword textAlignHorizontal = "left";
                SFUI::SubProp::Keyword textAlignVertical = "center";
                SFUI::SubProp::Duplex textOffset = {0.0f, 0.0f};
                SFUI::SubProp::Color textColor = SFUI::Color(0, 0, 0, 255);
                SFUI::SubProp::Color textOutlineColor = SFUI::Color(255, 255, 255, 255);
            };
    
            struct Button {
                SFUI::Optional<SFUI::SubProp::Color> hoveredFillColor;
                SFUI::Optional<SFUI::SubProp::Color> hoveredBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> pressedFillColor;
                SFUI::Optional<SFUI::SubProp::Color> pressedBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> disabledFillColor;
                SFUI::Optional<SFUI::SubProp::Color> disabledBorderColor;
                SFUI::SubProp::Dimension focusWidth = 10.0f;
                SFUI::SubProp::Dimension focusOffset = 0.0f;
                SFUI::SubProp::UniQuad focusCornerRadius;
                SFUI::SubProp::Color focusFillColor = SFUI::Color(0, 0, 0, 255);
                SFUI::SubProp::UniQuad toolTipPadding = 10.0f;
                SFUI::SubProp::UniQuad toolTipCornerRadius;
                SFUI::SubProp::Keyword toolTipText = "";
                SFUI::SubProp::Font toolTipFont;
                SFUI::SubProp::Numeric toolTipTextSize = 10.0f;
                SFUI::SubProp::Color toolTipFillColor = SFUI::Color(150, 150, 150, 255);
                SFUI::SubProp::Color toolTipTextColor = SFUI::Color(0, 0, 0, 255);
            };

            struct Toggle {
                SFUI::SubProp::Color offFillColor = SFUI::Color(0, 0, 0, 255);
                SFUI::SubProp::Color offBorderColor = SFUI::Color(0, 0, 0, 255);
                SFUI::SubProp::Color onFillColor = SFUI::Color(0, 0, 0, 255);
                SFUI::SubProp::Color onBorderColor = SFUI::Color(0, 0, 0, 255);
                SFUI::Optional<SFUI::SubProp::Color> hoveredOffFillColor;
                SFUI::Optional<SFUI::SubProp::Color> hoveredOffBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> hoveredOnFillColor;
                SFUI::Optional<SFUI::SubProp::Color> hoveredOnBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> pressedOffFillColor;
                SFUI::Optional<SFUI::SubProp::Color> pressedOffBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> pressedOnFillColor;
                SFUI::Optional<SFUI::SubProp::Color> pressedOnBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> disabledFillColor;
                SFUI::Optional<SFUI::SubProp::Color> disabledBorderColor;
                SFUI::SubProp::Dimension focusWidth = 10.0f;
                SFUI::SubProp::Dimension focusOffset = 0.0f;
                SFUI::SubProp::UniQuad focusCornerRadius;
                SFUI::SubProp::Color focusFillColor = SFUI::Color(0, 0, 0, 255);
                SFUI::SubProp::UniQuad toolTipPadding = 10.0f;
                SFUI::SubProp::UniQuad toolTipCornerRadius;
                SFUI::SubProp::Keyword toolTipText = "";
                SFUI::SubProp::Font toolTipFont;
                SFUI::SubProp::Numeric toolTipTextSize = 10.0f;
                SFUI::SubProp::Color toolTipFillColor = SFUI::Color(150, 150, 150, 255);
                SFUI::SubProp::Color toolTipTextColor = SFUI::Color(0, 0, 0, 255);
            };

            struct Graphic {
                SFUI::Optional<SFUI::SubProp::Texture> loadedGraphic;
                SFUI::Optional<SFUI::SubProp::Keyword> graphicPath;
                SFUI::SubProp::Keyword graphicAlign;
            };

            struct TextField {
                SFUI::SubProp::Keyword lineMode = "single";
                SFUI::SubProp::UniQuad textInset = 0.0f;
                SFUI::SubProp::Keyword text;
                SFUI::SubProp::Keyword placeholderText;
                SFUI::SubProp::Font font;
                SFUI::SubProp::Numeric textSize = 12.0f;
                SFUI::SubProp::TextStyle textStyle = "regular";
                SFUI::Optional<SFUI::SubProp::Numeric> letterSpacing;
                SFUI::Optional<SFUI::SubProp::Numeric> lineSpacing;
                SFUI::SubProp::Numeric textOutlineThickness = 0.0f;
                SFUI::SubProp::Keyword textAlignHorizontal = "left";
                SFUI::SubProp::Color textColor = SFUI::Color(0, 0, 0, 255);
                SFUI::SubProp::Color textOutlineColor = SFUI::Color(255, 255, 255, 255);
                SFUI::SubProp::Color placeholderTextColor = SFUI::Color(100, 100, 100, 255);
                SFUI::Optional<SFUI::SubProp::Color> hoveredFillColor;
                SFUI::Optional<SFUI::SubProp::Color> hoveredBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> pressedFillColor;
                SFUI::Optional<SFUI::SubProp::Color> pressedBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> disabledFillColor;
                SFUI::Optional<SFUI::SubProp::Color> disabledBorderColor;
                SFUI::SubProp::Dimension focusWidth = 10.0f;
                SFUI::SubProp::Dimension focusOffset = 0.0f;
                SFUI::SubProp::UniQuad focusCornerRadius;
                SFUI::SubProp::Color focusFillColor = SFUI::Color(0, 0, 0, 255);
                SFUI::SubProp::UniQuad toolTipPadding = 10.0f;
                SFUI::SubProp::UniQuad toolTipCornerRadius;
                SFUI::SubProp::Keyword toolTipText = "";
                SFUI::SubProp::Font toolTipFont;
                SFUI::SubProp::Numeric toolTipTextSize = 10.0f;
                SFUI::SubProp::Color toolTipFillColor = SFUI::Color(150, 150, 150, 255);
                SFUI::SubProp::Color toolTipTextColor = SFUI::Color(0, 0, 0, 255);
                SFUI::SubProp::Keyword caretShape = "line";
                SFUI::SubProp::Numeric caretBlinkTime = 500.0f;
                SFUI::SubProp::Numeric caretBlinkRatio = 1.0f;
                SFUI::SubProp::Color caretFillColor = SFUI::Color(0, 0, 0, 255);
            };

            struct Slider {
                SFUI::SubProp::Duplex shadowOffset = {0.0f, 0.0f};
                SFUI::SubProp::Numeric shadowRadius = 1.0f;
                SFUI::SubProp::Color shadowFillColor = SFUI::Color(0, 0, 0, 0);
                SFUI::SubProp::Keyword trackAlign = "horizontal";
                SFUI::SubProp::Dimension trackWidth = 8.0f;
                SFUI::SubProp::UniQuad trackCornerRadius = 4.0f;
                SFUI::Optional<SFUI::SubProp::Dimension> trackProgressedWidth;
                SFUI::SubProp::Dimension thumbWidth = 15.0f;
                SFUI::SubProp::Dimension thumbHeight = 15.0f;
                SFUI::SubProp::UniQuad thumbCornerRadius = 7.5f;
                SFUI::SubProp::Color trackFillColor = SFUI::Color(175, 175, 175, 255);
                SFUI::SubProp::Color trackBorderColor = SFUI::Color(175, 175, 175, 255);
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedFillColor;
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedHoveredFillColor;
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedHoveredBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedPressedFillColor;
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedPressedBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedFillColor;
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedHoveredFillColor;
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedHoveredBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedPressedFillColor;
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedPressedBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedDisabledFillColor;
                SFUI::Optional<SFUI::SubProp::Color> trackUnprogressedDisabledBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedDisabledFillColor;
                SFUI::Optional<SFUI::SubProp::Color> trackProgressedDisabledBorderColor;
                SFUI::SubProp::Color thumbFillColor = SFUI::Color(0, 0, 0, 255);
                SFUI::SubProp::Color thumbBorderColor = SFUI::Color(0, 0, 0, 255);
                SFUI::Optional<SFUI::SubProp::Color> thumbHoveredFillColor;
                SFUI::Optional<SFUI::SubProp::Color> thumbHoveredBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> thumbPressedFillColor;
                SFUI::Optional<SFUI::SubProp::Color> thumbPressedBorderColor;
                SFUI::Optional<SFUI::SubProp::Color> thumbDisabledFillColor;
                SFUI::Optional<SFUI::SubProp::Color> thumbDisabledBorderColor;
                SFUI::SubProp::Dimension focusWidth = 10.0f;
                SFUI::SubProp::Dimension focusOffset = 0.0f;
                SFUI::SubProp::UniQuad focusCornerRadius;
                SFUI::SubProp::Color focusFillColor = SFUI::Color(0, 0, 0, 255);
                SFUI::SubProp::UniQuad toolTipPadding = 10.0f;
                SFUI::SubProp::UniQuad toolTipCornerRadius;
                SFUI::SubProp::Keyword toolTipText = "";
                SFUI::SubProp::Font toolTipFont;
                SFUI::SubProp::Numeric toolTipTextSize = 10.0f;
                SFUI::SubProp::Color toolTipFillColor = SFUI::Color(150, 150, 150, 255);
                SFUI::SubProp::Color toolTipTextColor = SFUI::Color(0, 0, 0, 255);
            };
        }

        // State Props //
        namespace State {

            struct Button {
                SFUI::SubProp::Binary isDisabled = false;
                SFUI::SubProp::Binary isFocused = false;
            };

            struct Toggle {
                SFUI::SubProp::Binary isDisabled = false;
                SFUI::SubProp::Binary isFocused = false;
                SFUI::SubProp::Binary isOn = false;
            };

            struct TextField {
                SFUI::SubProp::Binary isDisabled = false;
                SFUI::SubProp::Binary isFocused = false;
            };

            struct Slider {
                SFUI::SubProp::Binary isDisabled = false;
                SFUI::SubProp::Binary isFocused = false;
                SFUI::SubProp::Binary isInverted = false;
                SFUI::SubProp::Numeric value = 0.0f;
                SFUI::SubProp::Numeric minimumValue = 0.0f;
                SFUI::SubProp::Numeric maximumValue = 20.0f;
                SFUI::SubProp::Numeric step = 1.0f;
            };
        }

        // Behavior Props //
        namespace Behavior {
    
            struct ScrollContainer {
                SFUI::SubProp::Callback onScroll;
            };

            struct Button {
                SFUI::SubProp::Callback onEnable;
                SFUI::SubProp::Callback onDisable;
                SFUI::SubProp::Callback onFocus;
                SFUI::SubProp::Callback onBlur;
                SFUI::SubProp::Callback onHoverIn;
                SFUI::SubProp::Callback onHoverOut;
                SFUI::SubProp::Callback onLeftPressIn;
                SFUI::SubProp::Callback onLeftPress;
                SFUI::SubProp::Callback onRightPressIn;
                SFUI::SubProp::Callback onRightPress;
                SFUI::SubProp::Callback onMiddlePressIn;
                SFUI::SubProp::Callback onMiddlePress;
                SFUI::SubProp::Callback onDoublePress;
                SFUI::SubProp::CallbackKey onKeyPress;
            };

            struct Toggle {
                SFUI::SubProp::Callback onEnable;
                SFUI::SubProp::Callback onDisable;
                SFUI::SubProp::Callback onFocus;
                SFUI::SubProp::Callback onBlur;
                SFUI::SubProp::Callback onHoverIn;
                SFUI::SubProp::Callback onHoverOut;
                SFUI::SubProp::Callback onLeftPressIn;
                SFUI::SubProp::Callback onLeftPress;
                SFUI::SubProp::Callback onRightPressIn;
                SFUI::SubProp::Callback onRightPress;
                SFUI::SubProp::Callback onMiddlePressIn;
                SFUI::SubProp::Callback onMiddlePress;
                SFUI::SubProp::Callback onDoublePress;
                SFUI::SubProp::CallbackKey onKeyPress;
                SFUI::SubProp::CallbackBinary onToggledState;
            };

            struct Graphic {
                SFUI::SubProp::Callback onLoad;
                SFUI::SubProp::Callback onLoadError;
            };

            struct TextField {
                SFUI::SubProp::Callback onEnable;
                SFUI::SubProp::Callback onDisable;
                SFUI::SubProp::Callback onFocus;
                SFUI::SubProp::Callback onBlur;
                SFUI::SubProp::Callback onHoverIn;
                SFUI::SubProp::Callback onHoverOut;
                SFUI::SubProp::Callback onLeftPressIn;
                SFUI::SubProp::Callback onLeftPress;
                SFUI::SubProp::Callback onRightPressIn;
                SFUI::SubProp::Callback onRightPress;
                SFUI::SubProp::Callback onMiddlePressIn;
                SFUI::SubProp::Callback onMiddlePress;
                SFUI::SubProp::Callback onDoublePress;
                SFUI::SubProp::Callback onPressOut;
                SFUI::SubProp::CallbackKey onKeyPress;
                SFUI::SubProp::CallbackKeyword onTextChange;
                SFUI::SubProp::CallbackKeyword onSubmit;
            };

            struct Slider {
                SFUI::SubProp::Callback onEnable;
                SFUI::SubProp::Callback onDisable;
                SFUI::SubProp::Callback onFocus;
                SFUI::SubProp::Callback onBlur;
                SFUI::SubProp::Callback onTrackHoverIn;
                SFUI::SubProp::Callback onTrackHoverOut;
                SFUI::SubProp::Callback onTrackLeftPressIn;
                SFUI::SubProp::Callback onTrackLeftPress;
                SFUI::SubProp::Callback onTrackRightPressIn;
                SFUI::SubProp::Callback onTrackRightPress;
                SFUI::SubProp::Callback onTrackMiddlePressIn;
                SFUI::SubProp::Callback onTrackMiddlePress;
                SFUI::SubProp::Callback onThumbHoverIn;
                SFUI::SubProp::Callback onThumbHoverOut;
                SFUI::SubProp::Callback onThumbLeftPressIn;
                SFUI::SubProp::Callback onThumbLeftPress;
                SFUI::SubProp::Callback onThumbRightPressIn;
                SFUI::SubProp::Callback onThumbRightPress;
                SFUI::SubProp::Callback onThumbMiddlePressIn;
                SFUI::SubProp::Callback onThumbMiddlePress;
                SFUI::SubProp::CallbackNumeric onSlidingStart;
                SFUI::SubProp::CallbackNumeric onSlidingEnd;
                SFUI::SubProp::CallbackNumeric onValueChange;
            };
        }
    }
}




////////////////////////////////////////////
// Custom Core Component Prop Group Types //
////////////////////////////////////////////

namespace SFUI {

    namespace PropGroup {

        struct Container {
            SFUI::Prop::Layout::Component layout;
            SFUI::Prop::Style::Component style;
        };

        struct Label {
            SFUI::Prop::Layout::Component layout;
            SFUI::Prop::Style::Component style;
            SFUI::Prop::Style::Label labelStyle;
        };

        struct Button {
            SFUI::Prop::Layout::Component layout;
            SFUI::Prop::Style::Component style;
            SFUI::Prop::Style::Button buttonStyle;
            SFUI::Prop::State::Button buttonState;
            SFUI::Prop::Behavior::Button buttonBehavior;
        };

        struct Toggle {
            SFUI::Prop::Layout::Component layout;
            SFUI::Prop::Style::Component style;
            SFUI::Prop::Style::Toggle toggleStyle;
            SFUI::Prop::State::Toggle toggleState;
            SFUI::Prop::Behavior::Toggle toggleBehavior;
        };

        struct Graphic {
            SFUI::Prop::Layout::Component layout;
            SFUI::Prop::Style::Component style;
            SFUI::Prop::Style::Graphic graphicStyle;
            SFUI::Prop::Behavior::Graphic graphicBehavior;
        };

        struct ScrollContainer {
            SFUI::Prop::Layout::Component layout;
            SFUI::Prop::Style::Component style;
            SFUI::Prop::Style::ScrollContainer scrollContainerStyle;
            SFUI::Prop::Behavior::ScrollContainer scrollContainerBehavior;
        };

        struct TextField {
            SFUI::Prop::Layout::Component layout;
            SFUI::Prop::Style::Component style;
            SFUI::Prop::Style::TextField textFieldStyle;
            SFUI::Prop::State::TextField textFieldState;
            SFUI::Prop::Behavior::TextField textFieldBehavior;
        };

        struct Slider {
            SFUI::Prop::Layout::Component layout;
            SFUI::Prop::Style::Component style;
            SFUI::Prop::Style::Slider sliderStyle;
            SFUI::Prop::State::Slider sliderState;
            SFUI::Prop::Behavior::Slider sliderBehavior;
        };
    }
}




///////////////////////////////////////////////
// Custom Core Component Computed Prop Types //
///////////////////////////////////////////////

namespace SFUI {

    namespace ComputedProp {

        // Computed Layout Props //
        namespace Layout {

            struct Component {
                SFUI::String alignDirection;
                SFUI::String alignPrimary;
                SFUI::String alignSecondary;
                SFUI::Vector2f size;
                SFUI::Vector2i position;
                SFUI::Vector4f padding;
                SFUI::Vector4f margin;
            };
    
            struct ComponentChild {
                SFUI::Vector2f size;
                SFUI::Vector2i position;
                SFUI::Vector4f margin;
            };
        }

        // Computed Style Props //
        namespace Style {
            
            struct Component {
                SFUI::Float borderWidth;
                SFUI::Vector4f cornerRadius;
                SFUI::Color fillColor;
                SFUI::Color borderColor;
                SFUI::Vector2f shadowOffset;
                SFUI::Float shadowRadius;
                SFUI::Color shadowFillColor;
            };
    
            struct ScrollContainer {
                SFUI::String scrollDirection;
                SFUI::Float scrollSpeedFactor;
            };

            struct Graphic {
                SFUI::String graphicPath;
                SFUI::Vector2f originalTextureSize;
            };
    
            struct TextField {
                SFUI::String lineMode;
            };

            struct Slider {
                SFUI::String trackAlign;
                SFUI::Float trackLength;
                SFUI::Vector2f thumbSize;
            };
        }
    }
}
