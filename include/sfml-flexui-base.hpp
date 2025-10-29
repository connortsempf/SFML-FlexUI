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
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);

        private:
            SFUI::SharedPointer<SFUI::Component> rootComponent;
        
        private:
            SFUI::Void drawRecursive(SFUI::SharedPointer<SFUI::Component> component, SFUI::RenderTarget& renderTarget);
    };
}




//////////////////////////////////////
// SFML-FlexUI Component Base Class //
//////////////////////////////////////

namespace SFUI {

    class Component {

        friend class UIRoot;

        public:
            SFUI::String componentID;
            SFUI::Prop::Layout::Component layout;
            SFUI::Prop::Style::Component style;

        public:
            Component() = default;
            Component(const SFUI::String& componentID);
            Component(const SFUI::String& componentID, const SFUI::Prop::Layout::Component& layout, const SFUI::Prop::Style::Component& style);
            virtual ~Component() = default;
            virtual SFUI::Void handleEvent(const SFUI::Event& event) = 0;
            virtual SFUI::Void update(const SFUI::Vector2u renderTargetSize) = 0;
            virtual SFUI::Void draw(SFUI::RenderTarget& renderTarget) = 0;
            SFUI::Void setParent(SFUI::Component* newParent);
            SFUI::Void addChild(const SFUI::SharedPointer<SFUI::Component>& newChild);
            SFUI::Void addChildren(const SFUI::Vector<SFUI::SharedPointer<SFUI::Component>>& newChildren);
            SFUI::Void updateChildFromParent(SFUI::ComputedProp::Layout::ComponentChild childComputedLayout);
            SFUI::Vector<SFUI::SharedPointer<SFUI::Component>> getChildren() const;
            SFUI::String getAlignDirection();
            SFUI::String getAlignPrimary();
            SFUI::String getAlignSecondary();
            SFUI::Vector2f getSize();
            SFUI::Vector2i getPosition();
            SFUI::Vector4f getPadding();
            SFUI::Vector4f getMargin();
            SFUI::Float getBorderWidth();
            SFUI::Vector4f getCornerRadius();
            SFUI::Color getFillColor();
            SFUI::Color getBorderColor();

        protected:
            SFUI::Vector2u renderTargetSize;
            SFUI::Component* parent;
            SFUI::Vector<SFUI::SharedPointer<SFUI::Component>> children;
            SFUI::VertexArray backgroundRects;
            SFUI::VertexArray backgroundArcs;
            SFUI::VertexArray borderRects;
            SFUI::VertexArray borderArcs;
            SFUI::VertexArray shadowRects;
            SFUI::VertexArray shadowArcs;
            SFUI::ComputedProp::Layout::Component computedLayout;
            SFUI::ComputedProp::Style::Component computedStyle;
            SFUI::Vector<SFUI::ComputedProp::Layout::ComponentChild> childrenComputedLayout;
        
        protected:
            SFUI::Bool isMouseHovered(const SFUI::Vector2i& mousePosition);
            SFUI::Color resolveColorSubProp(const SFUI::SubProp::Color& color);
            SFUI::Vector4f resolveUniQuadSubProp(SFUI::Vector2f size, SFUI::SubProp::UniQuad cornerRadius);
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
            SFUI::Void computeShadow();
            SFUI::Void computeGraphics();
            SFUI::Void computeChildrenMargin();
            SFUI::Void computeChildrenSize();
            SFUI::Void computeChildrenPosition();
            SFUI::Void updateChildren();
        
        private:
            SFUI::Void computeBackgroundRectGeometry(SFUI::Vector2f position, SFUI::Vector2f size);
            SFUI::Void computeBackgroundArcGeometry(SFUI::Vector2f center, SFUI::Float outerRadius, SFUI::Float startAngleDeg, SFUI::Float endAngleDeg);
            SFUI::Void computeBorderRectGeometry(SFUI::Vector2f position, SFUI::Vector2f size);
            SFUI::Void computeBorderArcGeometry(SFUI::Vector2f center, SFUI::Float outerRadius, SFUI::Float innerRadius, SFUI::Float startAngleDeg, SFUI::Float endAngleDeg);
            SFUI::Void computeShadowRectGeometry(SFUI::Vector2f position, SFUI::Vector2f size, SFUI::Color modifiedShadowColor);
            SFUI::Void computeShadowArcGeometry(SFUI::Vector2f center, SFUI::Float outerRadius, SFUI::Float startAngleDeg, SFUI::Float endAngleDeg, SFUI::Color modifiedShadowColor);
    };
}




///////////////////////////////////////////////
// SFML-FlexUI Extended Component Base Class //
///////////////////////////////////////////////

// namespace SFUI {

//     class ExtendedComponent {

//         public:
//             SFUI::String componentID;

//         public:
//             ExtendedComponent() = default;
//             ExtendedComponent(const SFUI::String& componentID);
//             virtual ~ExtendedComponent() = default;
//             virtual SFUI::Void handleEvent(const SFUI::Event& event) = 0;
//             virtual SFUI::Void update(const SFUI::Vector2u renderTargetSize) = 0;
//             virtual SFUI::Void draw(SFUI::RenderTarget& renderTarget) = 0;

//         private:
//             SFUI::SharedPointer<SFUI::Component> baseComponent;
//     };
// }
