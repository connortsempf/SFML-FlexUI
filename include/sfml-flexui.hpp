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
            SFUI::String componentID;
            SFUI::Prop::Layout layout;
            SFUI::Prop::Style style;

        public:
            Component() = default;
            Component(const SFUI::String& componentID);
            Component(const SFUI::String& componentID, const SFUI::Prop::Layout& layout);
            Component(const SFUI::String& componentID, const SFUI::Prop::Style& style);
            Component(const SFUI::String& componentID, const SFUI::Prop::Layout& layout, const SFUI::Prop::Style& style);
            SFUI::Void setParent(const SFUI::SharedPointer<Component>& newParent);
            SFUI::Void addChild(const SFUI::SharedPointer<SFUI::Component>& newChild);
            SFUI::Void addChildren(const SFUI::Vector<SFUI::SharedPointer<SFUI::Component>>& newChildren);
            SFUI::Vector<SFUI::SharedPointer<SFUI::Component>> getChildren() const;
            SFUI::Void updateChildFromParent(SFUI::ComputedProp::ChildLayout childComputedLayout);
            virtual ~Component() = default;
            virtual SFUI::Void update(const SFUI::Vector2u parentComponentSize) = 0;
            virtual SFUI::Void handleEvent(const SFUI::Event& event) = 0;
            virtual SFUI::Void draw(SFUI::RenderTarget& renderTarget) = 0;

        protected:
            SFUI::ComputedProp::Style computedStyle;
            SFUI::ComputedProp::Layout computedLayout;
            SFUI::Vector<SFUI::ComputedProp::ChildLayout> childrenComputedLayout;
            SFUI::Vector2u parentComponentSize;
            SFUI::VertexArray backgroundRects;
            SFUI::VertexArray backgroundArcs;
            SFUI::VertexArray borderRects;
            SFUI::VertexArray borderArcs;
        
        protected:
            SFUI::Color resolveColorSubProp(const SFUI::SubProp::Color& color);
            SFUI::Vector4f resolveCornerRadiusSubPro(
                SFUI::Vector2f size,
                SFUI::SubProp::Dimension cornerRadius,
                SFUI::Optional<SFUI::SubProp::Dimension> cornerRadiusTopLeft,
                SFUI::Optional<SFUI::SubProp::Dimension> cornerRadiusTopRight,
                SFUI::Optional<SFUI::SubProp::Dimension> cornerRadiusBottomLeft,
                SFUI::Optional<SFUI::SubProp::Dimension> cornerRadiusBottomRight
            );
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
            View(const SFUI::String& componentID, const SFUI::Prop::Layout& layout);
            View(const SFUI::String& componentID, const SFUI::Prop::Style& style);
            View(const SFUI::String& componentID, const SFUI::Prop::Layout& layout, const SFUI::Prop::Style& style);
        
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
            SFUI::Prop::LabelStyle labelStyle;

        public:
            Label() = default;
            Label(const SFUI::String& componentID);
            Label(const SFUI::String& componentID, const SFUI::Prop::Layout& layout);
            Label(const SFUI::String& componentID, const SFUI::Prop::Style& style);
            Label(const SFUI::String& componentID, const SFUI::Prop::LabelStyle& labelStyle);
            Label(const SFUI::String& componentID, const SFUI::Prop::Layout& layout, const SFUI::Prop::Style& style, const SFUI::Prop::LabelStyle& labelStyle);

        private:
            static SFUI::Float VERTICAL_CENTER_OFFSET_FACTOR;
            static SFUI::Float VERTICAL_BOTTOM_OFFSET_FACTOR;            

        private:
            SFUI::ComputedProp::LabelStyle computedLabelStyle;
            SFUI::Text textObject;

        private:
            SFUI::Void computeTextSize();
            SFUI::Void computeTextAlignHorizontal();
            SFUI::Void computeTextAlignVertical();
            SFUI::Void computeTextColor();
            SFUI::Void computeText();
            SFUI::Void update(const SFUI::Vector2u parentComponentSize);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);
    };
}




////////////////////////////////////////
// SFML-FlexUI Button Component Class //
////////////////////////////////////////

namespace SFUI {

    class Button : public Component {

        public:
            SFUI::Prop::ButtonStyle buttonStyle;
            SFUI::Prop::ButtonBehavior buttonBehavior;

        public:
            Button() = default;
            Button(const SFUI::String& componentID);
            Button(const SFUI::String& componentID, const SFUI::Prop::Layout& layout);
            Button(const SFUI::String& componentID, const SFUI::Prop::Style& style);
            Button(const SFUI::String& componentID, const SFUI::Prop::ButtonStyle& buttonStyle);
            Button(const SFUI::String& componentID, const SFUI::Prop::Layout& layout, const SFUI::Prop::Style& style, const SFUI::Prop::ButtonStyle& buttonStyle);

        private:
            static SFUI::Float TEXT_VERTICAL_OFFSET_FACTOR;
            static SFUI::UnsignedInt LONG_PRESS_THRESHOLD_MS;

        private:
            SFUI::ComputedProp::ButtonStyle computedButtonStyle;
            SFUI::Text toolTipTextObject;
        
        private:
            SFUI::Void computeAlternateColors();
            SFUI::Void computeFocusWidth();
            SFUI::Void computeFocusOffset();
            SFUI::Void computeFocusCornerRadius();
            SFUI::Void computeFocusFillColor();
            SFUI::Void computeToolTipPadding();
            SFUI::Void computeToolTipCornerRadius();
            SFUI::Void computeToolTipTextSize();
            SFUI::Void computeToolTipFillColor();
            SFUI::Void computeToolTipTextColor();
            SFUI::Void computeToolTipText();
            SFUI::Void update(const SFUI::Vector2u parentComponentSize);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);
    };
}
