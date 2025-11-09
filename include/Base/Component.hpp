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

        protected:
            struct ComputedChildLayout;

        public:
            SFUI::String componentID;
            SFUI::Prop::Layout::Component layout;
            SFUI::Prop::Style::Component style;

        public:
            Component() = default;
            Component(SFUI::String componentID);
            Component(SFUI::String componentID, SFUI::Prop::Layout::Component layout, SFUI::Prop::Style::Component style);
            virtual ~Component() = default;
            virtual SFUI::Void handleEvent(const SFUI::Event& event) = 0;
            virtual SFUI::Void update(const SFUI::Vector2u renderTargetSize) = 0;
            virtual SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) = 0;
            SFUI::Void setParent(SFUI::Component* newParent);
            SFUI::Void addChild(const SFUI::SharedPointer<SFUI::Component>& newChild);
            SFUI::Void addChildren(const SFUI::Vector<SFUI::SharedPointer<SFUI::Component>>& newChildren);
            SFUI::Void updateChildFromParent(SFUI::Component::ComputedChildLayout computedChildLayout);
            const SFUI::Vector<SFUI::SharedPointer<SFUI::Component>>& getChildren() const;
            const SFUI::Vector2f& getSize() const;
            const SFUI::Vector2i& getPosition() const;
            const SFUI::Vector4f& getPadding() const;
            const SFUI::Vector4f& getMargin() const;

        protected:
            enum class AlignDirection { Vertical, Horizontal };
            enum class AlignPrimary { Start, End, Center, SpaceBetween, SpaceAround, SpaceEvenly };
            enum class AlignSecondary { Start, End, Center };
        
        protected:
            struct ComputedLayout {
                SFUI::Component::AlignDirection alignDirection;
                SFUI::Component::AlignPrimary alignPrimary;
                SFUI::Component::AlignSecondary alignSecondary;
                SFUI::Vector2f size;
                SFUI::Vector2i position;
                SFUI::Vector4f padding;
                SFUI::Vector4f margin;
            };
            struct ComputedChildLayout {
                SFUI::Vector2f size;
                SFUI::Vector2i position;
                SFUI::Vector4f margin;
            };
            struct ComputedStyle {
                SFUI::Float borderWidth;
                SFUI::Vector4f cornerRadius;
                SFUI::Color fillColor;
                SFUI::Color borderColor;
                SFUI::Vector2f shadowOffset;
                SFUI::Float shadowRadius;
                SFUI::Color shadowFillColor;
            };

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
            SFUI::Prop::Layout::Component dirtyLayout;
            SFUI::Prop::Style::Component dirtyStyle;
            SFUI::Component::ComputedLayout computedLayout;
            SFUI::Component::ComputedStyle computedStyle;
            SFUI::Vector<SFUI::Component::ComputedChildLayout> computedChildrenLayout;
        
        protected:
            SFUI::Bool isMouseHovered(const SFUI::Vector2i& mousePosition);
            SFUI::Color resolveColorSubProp(const SFUI::SubProp::Color& color);
            SFUI::Vector4f resolveUniQuadSubProp(const SFUI::Vector2f& size, const SFUI::SubProp::UniQuad& subProp);
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
