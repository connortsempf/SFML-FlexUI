#pragma once
#include "Base/Component.hpp"




//////////////////////////////////////
// Scroll Container Component Class //
//////////////////////////////////////

namespace SFUI {
    
    class ScrollContainer : public Component {
        
        public:
            SFUI::Prop::Style::ScrollContainer scrollContainerStyle;
            SFUI::Prop::Behavior::ScrollContainer scrollContainerBehavior;

        public:
            ScrollContainer() = default;
            ScrollContainer(const SFUI::String& componentID);
            ScrollContainer(const SFUI::String& componentID, const SFUI::PropGroup::ScrollContainer& scrollContainerPropGroup);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);

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
            SFUI::ComputedProp::Style::ScrollContainer computedScrollContainerStyle;

        private:
            SFUI::Void computeAlignPrimary();
            SFUI::Void computeScrollDynamics();
            SFUI::Void computeChildrenScrollPosition();
    };
}
