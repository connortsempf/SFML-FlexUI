#pragma once
#include "Base/Component.hpp"
#include "Container.hpp"
#include "Label.hpp"




////////////////////////////////////////
// SFML-FlexUI Button Component Class //
////////////////////////////////////////

namespace SFUI {
    
    class Button : public Component {
        
        public:
            SFUI::Prop::Style::Button buttonStyle;
            SFUI::Prop::State::Button buttonState;
            SFUI::Prop::Behavior::Button buttonBehavior;

        public:
            Button() = default;
            Button(SFUI::String componentID);
            Button(SFUI::String componentID, SFUI::PropGroup::Button buttonPropGroup);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);
            SFUI::SharedPointer<SFUI::Font> getToolTipFont();
            
        private:
            static const SFUI::Time DOUBLE_PRESS_GAP_MS;
            static const SFUI::Time TOOL_TIP_THRESHOLD_MS;

        private:
            SFUI::Bool isHovered = false;
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
            SFUI::Bool dirtyEvent = false;
            SFUI::Prop::Style::Button dirtyButtonStyle;
            SFUI::Prop::State::Button dirtyButtonState;

        private:
            SFUI::Void computeDynamicColors();
            SFUI::Void computeFocus();
            SFUI::Void computeToolTip();
    };
}
