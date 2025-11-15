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
            Toggle(Toggle&&) = default;
            Toggle& operator=(Toggle&&) = default;
            Toggle(SFUI::String componentID);
            template<typename... Children>
            Toggle(SFUI::String componentID, SFUI::PropGroup::Toggle togglePropGroup, Children&&... children) :
                Component(std::move(componentID), std::move(togglePropGroup.layout), std::move(togglePropGroup.style)),
                toggleStyle(std::move(togglePropGroup.toggleStyle)),
                toggleState(std::move(togglePropGroup.toggleState)),
                toggleBehavior(std::move(togglePropGroup.toggleBehavior)),
                toggle(componentID + "_InnerToggle") {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);

            }
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