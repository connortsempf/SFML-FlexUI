#pragma once
#include "Base/Component.hpp"
#include "Button.hpp"




////////////////////////////
// Toggle Component Class //
////////////////////////////

namespace SFUI {
    
    class Toggle : public Component {
        
        public:
            SFUI::Prop::Style::Toggle toggleStyle;
            SFUI::Prop::State::Toggle toggleState;
            SFUI::Prop::Behavior::Toggle toggleBehavior;

        public:
            Toggle() = default;
            Toggle(const SFUI::String& componentID);
            Toggle(const SFUI::String& componentID, const SFUI::PropGroup::Toggle& togglePropGroup);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);

        private:
            SFUI::Bool dirtyEvent = false;
            SFUI::Button toggle;
            SFUI::Prop::Style::Toggle dirtyToggleStyle;
            SFUI::Prop::State::Toggle dirtyToggleState;

        private:
            SFUI::Void computeDynamicColors();
            SFUI::Void computeToggle();
    };
}