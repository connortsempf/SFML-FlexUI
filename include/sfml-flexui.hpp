#pragma once
#include <sfml-flexui-types.hpp>




///////////////////////////////////////
// SFML-FlexUI Root UI Manager Class //
///////////////////////////////////////

namespace SFUI {

    class Component;

    class UIRoot {

        public:
            UIRoot() = default;
            UIRoot(const SFUI::SharedPointer<SFUI::Component>& rootComponent);
            SFUI::Void setRootComponent(const SFUI::SharedPointer<SFUI::Component>& rootComponent);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);

        private:
            SFUI::SharedPointer<SFUI::Component> rootComponent;
    };
}




////////////////////////////////////////
// SFML-FlexUI Parent Component Class //
////////////////////////////////////////

namespace SFUI {

    class Component : public std::enable_shared_from_this<Component> {

        friend class UIRoot;

        public:
            struct Style {
                using Dimension = SFUI::Variant<SFUI::Float, SFUI::String>;
                using OptionalDimension = SFUI::Optional<Dimension>;
                using Color = SFUI::Variant<SFUI::Vector3ui8, SFUI::Vector4ui8, SFUI::String, SFUI::Color>;

                Dimension borderWidth = 0.0f;
                Dimension cornerRadius = 0.0f;
                OptionalDimension cornerRadiusTopLeft;
                OptionalDimension cornerRadiusTopRight;
                OptionalDimension cornerRadiusBottomLeft;
                OptionalDimension cornerRadiusBottomRight;
                Color fillColor = SFUI::Color(255, 255, 255, 255);
                Color borderColor = SFUI::Color(0, 0, 0, 255);
            };

            struct Layout {
                using Flex = SFUI::Float;
                using Alignment = SFUI::String;
                using OptionalAlignment = SFUI::Optional<Alignment>;
                using OptionalPosition = SFUI::Optional<SFUI::Int>;
                using Dimension = SFUI::Variant<SFUI::Float, SFUI::String>;

                Alignment alignDirection = "vertical";
                Alignment alignPrimary = "start";
                Alignment alignSecondary = "start";
                Dimension width = 0.0f;
                Dimension height = 0.0f;
                Dimension padding = 0.0f;
                Dimension margin = 0.0f;
                OptionalPosition xPosition;
                OptionalPosition yPosition;
            };

        public:
            SFUI::String componentID;
            Style style;
            Layout layout;

        public:
            Component() = default;
            Component(const SFUI::String& componentID);
            Component(const SFUI::String& componentID, const Style& style);
            Component(const SFUI::String& componentID, const Layout& layout);
            Component(const SFUI::String& componentID, const Style& style, const Layout& layout);
            virtual ~Component() = default;
        
        public:
            virtual SFUI::Void update(const SFUI::Vector2u parentComponentSize) = 0;
            virtual SFUI::Void handleEvent(const SFUI::Event& event) = 0;
            virtual SFUI::Void draw(SFUI::RenderTarget& renderTarget) = 0;
            SFUI::Void setParent(const SFUI::SharedPointer<Component>& newParent);
            SFUI::Void addChild(const SFUI::SharedPointer<SFUI::Component>& newChild);
            SFUI::Void addChildren(const SFUI::Vector<SFUI::SharedPointer<SFUI::Component>>& newChildren);
            SFUI::Vector<SFUI::SharedPointer<SFUI::Component>> getChildren() const;

        protected:
            struct ComputedStyle {
                SFUI::Float borderWidth;
                SFUI::Vector4f cornerRadius;
                SFUI::Color fillColor;
                SFUI::Color borderColor;
            };
            struct ComputedLayout {
                SFUI::String alignDirection;
                SFUI::String alignPrimary;
                SFUI::String alignSecondary;
                SFUI::Vector2f size;
                SFUI::Vector2i position;
                SFUI::Float padding;
                SFUI::Float margin;
            };
            struct ChildComputedLayout {
                SFUI::Vector2f size;
                SFUI::Vector2i position;
                SFUI::Float margin;
            };
            ComputedStyle computedStyle;
            ComputedLayout computedLayout;
            SFUI::Vector<ChildComputedLayout> childrenComputedLayout;
            SFUI::Vector2u parentComponentSize;
            SFUI::VertexArray backgroundRects;
            SFUI::VertexArray backgroundArcs;
            SFUI::VertexArray borderRects;
            SFUI::VertexArray borderArcs;
        
        public:
            SFUI::Void updateChildFromParent(ChildComputedLayout childComputedLayout);

        protected:
            SFUI::Void computeAlignDirection();
            SFUI::Void computeAlignPrimary();
            SFUI::Void computeAlignSecondary();
            SFUI::Void computeMargin();
            SFUI::Void computeSize();
            SFUI::Void computePadding();
            SFUI::Void computePosition();
            SFUI::Void computeBorderWidth();
            SFUI::Void computeCornerRadius();
            SFUI::Void computeFillColor();
            SFUI::Void computeBorderColor();
            SFUI::Void computeGraphics();
            SFUI::Void computeChildrenMargin();
            SFUI::Void computeChildrenSize();
            SFUI::Void computeChildrenPosition();
            SFUI::Void updateChildren();

        private:
            SFUI::WeakPointer<SFUI::Component> parent;
            SFUI::Vector<SFUI::SharedPointer<SFUI::Component>> children;
        
        private:
            SFUI::Void computeBackgroundRectGeometry(SFUI::Vector2f position, SFUI::Vector2f size);
            SFUI::Void computeBackgroundArcGeometry(SFUI::Vector2f center, SFUI::Float outerRadius, SFUI::Float startAngleDeg, SFUI::Float endAngleDeg);
            SFUI::Void computeBorderRectGeometry(SFUI::Vector2f position, SFUI::Vector2f size);
            SFUI::Void computeBorderArcGeometry(SFUI::Vector2f center, SFUI::Float outerRadius, SFUI::Float innerRadius, SFUI::Float startAngleDeg, SFUI::Float endAngleDeg);
    };
}




//////////////////////////////////////
// SFML-FlexUI View Component Class //
//////////////////////////////////////

namespace SFUI {

    class View : public Component {

        public:
            View() = default;
            View(const SFUI::String& componentID);
            View(const SFUI::String& componentID, const Style& style);
            View(const SFUI::String& componentID, const Layout& layout);
            View(const SFUI::String& componentID, const Style& style, const Layout& layout);
        
        private:
            SFUI::Void update(const SFUI::Vector2u parentComponentSize);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);
    };
}




///////////////////////////////////////
// SFML-FlexUI Label Component Class //
///////////////////////////////////////

namespace SFUI {

    class Label : public Component {

        public:
            struct LabelStyle {
                using Color = SFUI::Variant<SFUI::Vector3ui8, SFUI::Vector4ui8, SFUI::String, SFUI::Color>;

                SFUI::String text;
                SFUI::SharedPointer<SFUI::Font> font;
                SFUI::Float textSize = 12.0f;
                SFUI::String textAlignHorizontal = "center";
                SFUI::String textAlignVertical = "center";
                Color textColor = SFUI::Color(0, 0, 0, 255);
            };

        public:
            LabelStyle labelStyle;

        public:
            Label() = default;
            Label(const SFUI::String& componentID);
            Label(const SFUI::String& componentID, const Style& style);
            Label(const SFUI::String& componentID, const LabelStyle& labelStyle);
            Label(const SFUI::String& componentID, const Layout& layout);
            Label(const SFUI::String& componentID, const Style& style, const LabelStyle& labelStyle, const Layout& layout);

        private:
            static SFUI::Float VERTICAL_CENTER_OFFSET_FACTOR;
            static SFUI::Float VERTICAL_BOTTOM_OFFSET_FACTOR;
            struct ComputedLabelStyle {
                SFUI::Float textSize;
                SFUI::String textAlignHorizontal;
                SFUI::String textAlignVertical;
                SFUI::Color textColor;
            };

        private:
            ComputedLabelStyle computedLabelStyle;
            SFUI::Text textObject;

        private:
            SFUI::Void computeTextSize();
            SFUI::Void computeTextAlignHorizontal();
            SFUI::Void computeTextAlignVertical();
            SFUI::Void computeTextColor();
            SFUI::Void computeText();

        private:
            SFUI::Void update(const SFUI::Vector2u parentComponentSize);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);
    };
}