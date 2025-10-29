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
    
    using RenderTarget = sf::RenderTarget;
    
    using Texture = sf::Texture;
    
    using RenderTexture = sf::RenderTexture;
    
    using PrimitiveType = sf::PrimitiveType;

    using VertexArray = sf::VertexArray;
    
    using Sprite = sf::Sprite;

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

        using Callback = SFUI::Function<void(const SFUI::String&)>;

        using CallbackKey = SFUI::Function<void(const SFUI::String&, sf::Keyboard::Key)>;

        using CallbackString = SFUI::Function<void(const SFUI::String&, const SFUI::String&)>;
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
                SFUI::SubProp::Dimension padding = 0.0f;
                SFUI::SubProp::Dimension margin = 0.0f;
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
            
            struct Label {
                SFUI::SubProp::Keyword text;
                SFUI::SubProp::Font font;
                SFUI::SubProp::Numeric textSize = 12.0f;
                SFUI::SubProp::TextStyle textStyle = "regular";
                SFUI::Optional<SFUI::SubProp::Numeric> letterSpacing;
                SFUI::Optional<SFUI::SubProp::Numeric> lineSpacing;
                SFUI::SubProp::Numeric textOutlineThickness = 0.0f;
                SFUI::SubProp::Keyword textAlignHorizontal = "left";
                SFUI::SubProp::Keyword textAlignVertical = "center";
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
                SFUI::SubProp::Numeric toolTipPadding = 10.0f;
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

            struct ScrollContainer {
                SFUI::SubProp::Keyword scrollDirection = "vertical";
                SFUI::SubProp::Numeric scrollSpeedFactor = 15.0f;
                SFUI::SubProp::Binary usingScrollBar = false;
                SFUI::SubProp::Keyword scrollBarAlign = "right";
                SFUI::SubProp::Dimension trackOnAxisSize;
                SFUI::SubProp::Dimension trackOffAxisSize;
                SFUI::SubProp::UniQuad trackCornerRadius;
                SFUI::SubProp::Color trackFillColor;
                SFUI::Optional<SFUI::SubProp::Color> trackHoveredFillColor;
                SFUI::Optional<SFUI::SubProp::Color> trackPressedFillColor;
                SFUI::SubProp::UniQuad thumbCornerRadius;
                SFUI::SubProp::Color thumbFillColor;
                SFUI::Optional<SFUI::SubProp::Color> thumbHoveredFillColor;
                SFUI::Optional<SFUI::SubProp::Color> thumbPressedFillColor;
            };

            struct TextField {
                SFUI::SubProp::Keyword lineMode;
                SFUI::SubProp::UniQuad textInset;

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
                SFUI::SubProp::Numeric toolTipPadding = 10.0f;
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
        }

        // State Props //
        namespace State {

        }

        // Behavior Props //
        namespace Behavior {

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

            struct Graphic {
                SFUI::SubProp::Callback onLoad;
                SFUI::SubProp::Callback onLoadError;
            };
    
            struct ScrollContainer {
                SFUI::SubProp::Callback onScroll;
                SFUI::SubProp::Callback onScrollDragStart;
                SFUI::SubProp::Callback onScrollDragEnd;
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
                SFUI::SubProp::CallbackString onTextChange;
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
            SFUI::Prop::Behavior::Button buttonBehavior;
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
            SFUI::Prop::Behavior::TextField textFieldBehavior;
        };
    }
}




///////////////////////////////////////////////
// Custom Core Component Computed Prop Types //
///////////////////////////////////////////////

namespace SFUI {

    namespace ComputedProp {

        struct Layout {
            SFUI::String alignDirection;
            SFUI::String alignPrimary;
            SFUI::String alignSecondary;
            SFUI::Vector2f size;
            SFUI::Vector2i position;
            SFUI::Float padding;
            SFUI::Float margin;
        };

        struct ChildLayout {
            SFUI::Vector2f size;
            SFUI::Vector2i position;
            SFUI::Float margin;
        };

        struct Style {
            SFUI::Float borderWidth;
            SFUI::Vector4f cornerRadius;
            SFUI::Color fillColor;
            SFUI::Color borderColor;
            SFUI::Vector2f shadowOffset;
            SFUI::Float shadowRadius;
            SFUI::Color shadowFillColor;
        };

        struct LabelStyle {
            SFUI::String textAlignHorizontal;
            SFUI::String textAlignVertical;
        };

        struct ButtonStyle {
            SFUI::Color hoveredFillColor;
            SFUI::Color hoveredBorderColor;
            SFUI::Color pressedFillColor;
            SFUI::Color pressedBorderColor;
            SFUI::Color disabledFillColor;
            SFUI::Color disabledBorderColor;
            SFUI::Float focusWidth;
            SFUI::Float focusOffset;
            SFUI::Vector4f focusCornerRadius;
            SFUI::Color focusFillColor;
            SFUI::Float toolTipPadding;
            SFUI::Vector4f toolTipCornerRadius;
            SFUI::Float toolTipTextSize;
            SFUI::Color toolTipFillColor;
            SFUI::Color toolTipTextColor;
        };

        struct GraphicStyle {
            SFUI::String graphicPath;
            SFUI::String graphicAlign;
            SFUI::Vector2f originalTextureSize;
        };

        struct ScrollContainerStyle {
            SFUI::String scrollDirection;
            SFUI::Float scrollSpeedFactor;
            SFUI::Bool usingScrollBar;
            SFUI::String scrollBarAlign;
            SFUI::Float trackOnAxisSize;
            SFUI::Float trackOffAxisSize;
            SFUI::Float trackCornerRadius;
            SFUI::Color trackFillColor;
            SFUI::Color trackHoveredFillColor;
            SFUI::Color trackPressedFillColor;
            SFUI::Float thumbCornerRadius;
            SFUI::Color thumbFillColor;
            SFUI::Color thumbHoveredFillColor;
            SFUI::Color thumbPressedFillColor;
        };

        struct TextFieldStyle {
            SFUI::String lineMode;
            SFUI::Vector4f textInset;
            SFUI::String placeholderText;
            SFUI::Color placeholderTextColor;
            SFUI::String caretShape;
            SFUI::Float caretOnTime;
            SFUI::Float caretOffTime;
            SFUI::Color caretFillColor;
        };
    }
}




//////////////////////////////////////////
// Custom Extended Component Prop Types //
//////////////////////////////////////////

namespace SFUI {

    namespace Prop {

        // Style Props //
        namespace Style {

        }

        // State Props //
        namespace State {

        }

        // Behavior Props //
        namespace Behavior {

        }
    }
}




////////////////////////////////////////////////
// Custom Extended Component Prop Group Types //
////////////////////////////////////////////////

namespace SFUI {

    namespace PropGroup {

    }
}




///////////////////////////////////////////////////
// Custom Extended Component Computed Prop Types //
///////////////////////////////////////////////////

namespace SFUI {

    namespace ComputedProp {

    }
}
