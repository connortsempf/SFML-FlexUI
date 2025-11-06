#pragma once
#include "Types/base.hpp"
#include "Types/render.hpp"
#include "Types/vector.hpp"
#include "Types/prop.hpp"
#include <algorithm>
#include <cmath>




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
            virtual SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) = 0;
            SFUI::Void setParent(SFUI::Component* newParent);
            SFUI::Void addChild(const SFUI::SharedPointer<SFUI::Component>& newChild);
            SFUI::Void addChildren(const SFUI::Vector<SFUI::SharedPointer<SFUI::Component>>& newChildren);
            SFUI::Void updateChildFromParent(SFUI::ComputedProp::Layout::ComponentChild childComputedLayout);
            SFUI::Vector<SFUI::SharedPointer<SFUI::Component>> getChildren() const;
            SFUI::Vector2f getSize();
            SFUI::Vector2i getPosition();
            SFUI::Vector4f getPadding();
            SFUI::Vector4f getMargin();

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
            SFUI::Void computeAlignment();
            SFUI::Void computeLayoutBox();
            SFUI::Void computeStyles();
            SFUI::Void computeColors();
            SFUI::Void computeShadows();
            SFUI::Void computeGraphics();
            SFUI::Void computeChildrenLayoutBox();
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
