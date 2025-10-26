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
        
        private:
            SFUI::Void drawRecursive(SFUI::SharedPointer<SFUI::Component> component, SFUI::RenderTarget& renderTarget);
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
            Component(const SFUI::String& componentID, const SFUI::Prop::Layout& layout, const SFUI::Prop::Style& style);
            SFUI::Void setParent(const SFUI::SharedPointer<Component>& newParent);
            SFUI::Void addChild(const SFUI::SharedPointer<SFUI::Component>& newChild);
            SFUI::Void addChildren(const SFUI::Vector<SFUI::SharedPointer<SFUI::Component>>& newChildren);
            SFUI::Void updateChildFromParent(SFUI::ComputedProp::ChildLayout childComputedLayout);
            SFUI::Vector<SFUI::SharedPointer<SFUI::Component>> getChildren() const;
            SFUI::String getAlignDirection();
            SFUI::String getAlignPrimary();
            SFUI::String getAlignSecondary();
            SFUI::Vector2f getSize();
            SFUI::Vector2i getPosition();
            SFUI::Float getPadding();
            SFUI::Float getMargin();
            SFUI::Float getBorderWidth();
            SFUI::Vector4f getCornerRadius();
            SFUI::Color getFillColor();
            SFUI::Color getBorderColor();
            virtual ~Component() = default;
            virtual SFUI::Void update(const SFUI::Vector2u renderTargetSize) = 0;
            virtual SFUI::Void handleEvent(const SFUI::Event& event) = 0;
            virtual SFUI::Void draw(SFUI::RenderTarget& renderTarget) = 0;

        protected:
            SFUI::Vector2u renderTargetSize;
            SFUI::VertexArray backgroundRects;
            SFUI::VertexArray backgroundArcs;
            SFUI::VertexArray borderRects;
            SFUI::VertexArray borderArcs;
            SFUI::ComputedProp::Layout computedLayout;
            SFUI::ComputedProp::Style computedStyle;
            SFUI::Vector<SFUI::ComputedProp::ChildLayout> childrenComputedLayout;
        
        protected:
            SFUI::Bool isMouseHovered(const SFUI::Vector2i& mousePosition);
            SFUI::Color resolveColorSubProp(const SFUI::SubProp::Color& color);
            SFUI::Vector4f resolveCornerRadiusSubProp(
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




///////////////////////////////////////////
// SFML-FlexUI Container Component Class //
///////////////////////////////////////////

namespace SFUI {

    class Container : public Component {

        public:
            Container() = default;
            Container(const SFUI::String& componentID);
            Container(const SFUI::String& componentID, const SFUI::PropGroup::Container& containerPropGroup);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
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
            Label(const SFUI::String& componentID, const SFUI::PropGroup::Label& labelPropGroup);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);
            SFUI::Float getTextSize();
            SFUI::String getTextAlignHorizontal();
            SFUI::String getTextAlignVertical();
            SFUI::Color getTextColor();
            SFUI::FloatRect getTextBounds();

        private:
            static const SFUI::Float VERTICAL_CENTER_OFFSET_FACTOR;
            static const SFUI::Float VERTICAL_BOTTOM_OFFSET_FACTOR;

        private:
            SFUI::Text textObject;
            SFUI::ComputedProp::LabelStyle computedLabelStyle;

        private:
            SFUI::Void computeTextSize();
            SFUI::Void computeTextAlignHorizontal();
            SFUI::Void computeTextAlignVertical();
            SFUI::Void computeTextColor();
            SFUI::Void computeText();
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
            Button(const SFUI::String& componentID, const SFUI::PropGroup::Button& buttonPropGroup);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);
            
        private:
            static const SFUI::Time DOUBLE_PRESS_GAP_MS;
            static const SFUI::Time TOOL_TIP_THRESHOLD_MS;

        private:
            SFUI::Bool isDisabled = false;
            SFUI::Bool isHovered = false;
            SFUI::Bool isFocused = false;
            SFUI::Bool isLeftPressed = false;
            SFUI::Bool isRightPressed = false;
            SFUI::Bool isMiddlePressed = false;
            SFUI::Bool isShowingToolTip = false;
            SFUI::Vector2f previousHoverPosition;
            SFUI::Vector2i previousPressPosition;
            SFUI::Clock doublePressClock;
            SFUI::Time previousPressTime;
            SFUI::Clock toolTipClock;
            SFUI::Time toolTipTime;
            SFUI::Container focus;
            SFUI::Label toolTip;
            SFUI::ComputedProp::ButtonStyle computedButtonStyle;
        
        private:
            SFUI::Void computeDynamicFillColor();
            SFUI::Void computeDynamicBorderColor();
            SFUI::Void computeFocusWidth();
            SFUI::Void computeFocusOffset();
            SFUI::Void computeFocusCornerRadius();
            SFUI::Void computeFocusFillColor();
            SFUI::Void computeFocus();
            SFUI::Void computeToolTipPadding();
            SFUI::Void computeToolTipCornerRadius();
            SFUI::Void computeToolTipTextSize();
            SFUI::Void computeToolTipFillColor();
            SFUI::Void computeToolTipTextColor();
            SFUI::Void computeToolTip();
            SFUI::Void computeToolTipLifetime();
    };
}




/////////////////////////////////////////
// SFML-FlexUI Graphic Component Class //
/////////////////////////////////////////

namespace SFUI {
    
    class Graphic : public Component {
        
        public:
            SFUI::Prop::GraphicStyle graphicStyle;
            SFUI::Prop::GraphicBehavior graphicBehavior;

        public:
            Graphic() = default;
            Graphic(const SFUI::String& componentID);
            Graphic(const SFUI::String& componentID, const SFUI::PropGroup::Graphic& graphicPropGroup);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);

        private:
            enum class LoadType { PRE_LOAD, SELF_LOAD };
            enum class LoadState { PRE_LOAD_UNLOADED, PRE_LOAD_LOADED, SELF_LOAD_UNLOADED, SELF_LOAD_LOADED, SELF_LOAD_ERROR };

        private:
            SFUI::Texture graphicSource;
            SFUI::Sprite graphic;
            SFUI::Graphic::LoadType loadType = SFUI::Graphic::LoadType::PRE_LOAD;
            SFUI::Graphic::LoadState loadState = SFUI::Graphic::LoadState::PRE_LOAD_UNLOADED;
            SFUI::ComputedProp::GraphicStyle computedGraphicStyle;
        
        private:
            SFUI::Void computeGraphicSource();
            SFUI::Void computeGraphicAlign();
            SFUI::Void computeGraphic();
    };
}




//////////////////////////////////////////////////
// SFML-FlexUI Scroll Container Component Class //
//////////////////////////////////////////////////

namespace SFUI {
    
    class ScrollContainer : public Component {
        
        public:
            SFUI::Prop::ScrollContainerStyle scrollContainerStyle;
            SFUI::Prop::ScrollContainerBehavior scrollContainerBehavior;

        public:
            ScrollContainer() = default;
            ScrollContainer(const SFUI::String& componentID);
            ScrollContainer(const SFUI::String& componentID, const SFUI::PropGroup::ScrollContainer& scrollContainerPropGroup);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);

        private:
            SFUI::Bool isHovered = false;
            SFUI::Bool isTrackHovered = false;
            SFUI::Bool isThumbHovered = false;
            SFUI::Bool isTrackPressed = false;
            SFUI::Bool isThumbPressed = false;
            SFUI::Vector2f scrollOffset = {0.0f, 0.0f};
            SFUI::Vector2f maxScrollOffset = {0.0f, 0.0f};
            SFUI::Vector2f contentSize;
            SFUI::Float dragStartPosition;
            SFUI::ComputedProp::ScrollContainerStyle computedScrollContainerStyle;

        private:
            SFUI::Void computeAlignPrimary();
            SFUI::Void computeScrollDirection();
            SFUI::Void computeScrollSpeedFactor();
            SFUI::Void computeChildrenScrollPosition();
            SFUI::Void computeMaxScrollOffset();
    };
}
